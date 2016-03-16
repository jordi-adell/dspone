#define DSPONE_PARTICLE_FILTER_DEBUG
#undef DSPONE_PARTICLE_FILTER_DEBUG

#include <dspone/ParticleFilter.h>

#ifdef DSPONE_PARTICLE_FILTER_DEBUG
#include "gnuplot_i.h"
#endif


    namespace dsp {

#define DSPONE_PARTICLE_FILTER_TMPL \
  template<class T_Particle, class T_ID, class T_ParticleSet, class T_Weight, class T_WeightSet>

#define DSPONE_PARTICLE_FILTER_DCL \
  ParticleFilter<T_Particle, T_ID, T_ParticleSet, T_Weight, T_WeightSet>


      DSPONE_PARTICLE_FILTER_TMPL
      DSPONE_PARTICLE_FILTER_DCL::ParticleFilter(T_Particle initialState,
                                              int numParticles,
                                              T_ID id,
                                              std::pair<T_Particle, T_Particle> range,
                                              boost::shared_ptr<ObservationModel> obsModel,
                                              boost::shared_ptr<PredictionModel> predModel,
                                              boost::shared_ptr<ResamplingModel> resModel) :
        _order(numParticles),
        _initialState(initialState),
        _range(range),
        _callback(NULL),
        _id(id),
        _observationModel(obsModel),
        _predictionModel(predModel),
        _resamplingModel(resModel)
      {
        init();
      }

      DSPONE_PARTICLE_FILTER_TMPL
      DSPONE_PARTICLE_FILTER_DCL::ParticleFilter(T_Particle initialState,
                                              int numParticles,
                                              T_ID id,
                                              std::pair<T_Particle, T_Particle> range,
                                              boost::shared_ptr<ObservationModel> obsModel,
                                              boost::shared_ptr<PredictionModel> predModel) :
        _order(numParticles),
        _initialState(initialState),
        _range(range),
        _callback(NULL),
        _id(id),
        _observationModel(boost::shared_dynamic_cast<ObservationModel>(obsModel)),
        _predictionModel(boost::shared_dynamic_cast<PredictionModel>(predModel))
      {
        init();
      }

      DSPONE_PARTICLE_FILTER_TMPL
      DSPONE_PARTICLE_FILTER_DCL::ParticleFilter(T_Particle initialState,
                                              int numParticles,
                                              T_ID id,
                                              std::pair<T_Particle, T_Particle> range,
                                              boost::shared_ptr<ObservationModel> obsModel) :
        _order(numParticles),
        _initialState(initialState),
        _range(range),
        _callback(NULL),
        _id(id),
        _observationModel(obsModel)
      {
        init();
      }

      DSPONE_PARTICLE_FILTER_TMPL
      void DSPONE_PARTICLE_FILTER_DCL::init()
      {
        // convert dyanmic convertions into compilation asserts, to avoid
        // hard to debug errors
        BOOST_STATIC_ASSERT_MSG( (boost::is_base_and_derived<IParticleSet<T_Weight>, T_WeightSet>::value),
                                 "Ther weight set class provided do no inherit from IParticleSet");
        BOOST_STATIC_ASSERT_MSG( (boost::is_base_and_derived<IParticleSet<T_Particle>, T_ParticleSet>::value) ,
                                 "Ther particle set class provided do no inherit from IParticleSet");

        // Variable initialisation
        _resampledParticles = T_ParticleSet(_order, _initialState, _range.first, _range.second);
        _particles = T_ParticleSet(_order, _initialState,  _range.first, _range.second);
        _particles.addNoise(0,1);
        _weights = T_WeightSet(_order, static_cast<double>(1)/_order);

        // Default models construction if not provided
        if (!_predictionModel)
        {
          _predictionModel.reset(new DefaultPredictionModelImpl(_initialState,0));
        }

        if (!_resamplingModel)
        {
          _resamplingModel.reset(new DefaultResamplingModelImpl());
        }
      }


      DSPONE_PARTICLE_FILTER_TMPL
      DSPONE_PARTICLE_FILTER_DCL
      DSPONE_PARTICLE_FILTER_DCL::make_particle_filter(T_Particle initialState,
                                                    int numParticles,
                                                    T_ID id,
                                                    std::pair<T_Particle, T_Particle> range,
                                                    boost::shared_ptr<ObservationModel> obsModel,
                                                    boost::shared_ptr<PredictionModel> predModel,
                                                    boost::shared_ptr<ResamplingModel> resModel)
      {
        return DSPONE_PARTICLE_FILTER_DCL(initialState,  numParticles, id, range, obsModel, predModel, resModel);
      }

      DSPONE_PARTICLE_FILTER_TMPL
      T_Particle DSPONE_PARTICLE_FILTER_DCL::updateFilter()
      {

#ifdef DSPONE_PARTICLE_FILTER_DEBUG
        static Gnuplot plot;
        plot.reset_plot();
        plot.set_xrange(0,0.1);
        plot.set_yrange(-0.05,0.05);
        plot.set_style("impulses");
        plot.set_polar();
#endif

        predictionStep();
        resamplingStep();
        measurmentStep();


        // This probably could be replaced by a condesantion model
        // in a similar way to the observation and resampling models
        T_Particle solution = ((_particles * _weights).sum());
        T_Weight sum = _weights.sum();
        if (sum != 0)
          solution = solution / sum;
        else
          ERROR_STREAM("Particle weights are zero, so you are really far away of the target");
        TRACE_STREAM("Estimation: " << solution);
        _predictionModel->updateModel(solution);

#ifdef DSPONE_PARTICLE_FILTER_DEBUG
        plot.plot_xy(_particles.get(), _weights.get(), _particles.size(), "measured");
        T_Particle a = 0.1;
        plot.plot_xy(&solution, &a, 1);
        util::Timer::sleepMs(50);
#endif

        return solution;
      }

      DSPONE_PARTICLE_FILTER_TMPL
      void DSPONE_PARTICLE_FILTER_DCL::setCallback(IParticleFilterCallback<T_Particle> *callback)
      {
        _callback = callback;
      }

      DSPONE_PARTICLE_FILTER_TMPL
      T_ParticleSet DSPONE_PARTICLE_FILTER_DCL::getParticles() const
      {
        return _particles;
      }

      DSPONE_PARTICLE_FILTER_TMPL
      T_WeightSet DSPONE_PARTICLE_FILTER_DCL::getWeights() const
      {
        return _weights;
      }

      DSPONE_PARTICLE_FILTER_TMPL
      void DSPONE_PARTICLE_FILTER_DCL::resamplingStep()
      {
        _resamplingModel->resample(_particles, _resampledParticles, _weights);
        _particles = _resampledParticles;
        T_Particle stddev = _particles.getStandardDeviation();
        _particles.addNoise(0, stddev/10);
      }

      DSPONE_PARTICLE_FILTER_TMPL
      void DSPONE_PARTICLE_FILTER_DCL::predictionStep()
      {
        _predictionModel->update(_particles);
      }

      DSPONE_PARTICLE_FILTER_TMPL
      void DSPONE_PARTICLE_FILTER_DCL::measurmentStep()
      {
        _weights = _observationModel->getWeights(_particles);
      }

      DSPONE_PARTICLE_FILTER_TMPL
      T_ID DSPONE_PARTICLE_FILTER_DCL::getId() const
      {
        return _id;
      }

#undef DSPONE_PARTICLE_FILTER_TMPL
#undef DSPONE_PARTICLE_FILTER_DCL

    }

