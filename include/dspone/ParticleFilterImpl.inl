#include <dspone/dsplogger.h>

#include <wipp/wipputils.h>
#include <wipp/wippstats.h>

      namespace dsp {

#define DSPONE_BASIC_PARTICLE_SET_TMP template <class T_Particle>
#define DSPONE_BASIC_PARTICLE_SET_DCL BasicParticleSet<T_Particle>

        DSPONE_BASIC_PARTICLE_SET_TMP
        DSPONE_BASIC_PARTICLE_SET_DCL::BasicParticleSet(boost::shared_array<T_Particle> set, int size, T_Particle min, T_Particle max) :
          IParticleSet<T_Particle> (size),
          _set(set),
          _randState(NULL),
          _range(std::make_pair(min, max))
        {
          init();
        }

        DSPONE_BASIC_PARTICLE_SET_TMP
        DSPONE_BASIC_PARTICLE_SET_DCL::BasicParticleSet(int size, T_Particle p, T_Particle min, T_Particle max) :
          IParticleSet<T_Particle> (size),
          _set(new T_Particle[size]),
          _randState(NULL),
          _range(std::make_pair(min, max))
        {
          init();
	  wipp::set(p, _set.get(), IParticleSet<T_Particle>::_size);
        }

        DSPONE_BASIC_PARTICLE_SET_TMP
        DSPONE_BASIC_PARTICLE_SET_DCL::BasicParticleSet(const BasicParticleSet<T_Particle> &particleSet) :
          IParticleSet<T_Particle>(particleSet._size),
          _set(new T_Particle[particleSet._size]),
          _randState(NULL),
          _range(particleSet._range)
        {
	  wipp::copyBuffer(particleSet._set.get(), _set.get(), particleSet._size);
        }

        DSPONE_BASIC_PARTICLE_SET_TMP
        void DSPONE_BASIC_PARTICLE_SET_DCL::init()
        {
	  _randomValues.reset(new double[IParticleSet<T_Particle>::_size]);
          _randomParticles.reset(new T_Particle[IParticleSet<T_Particle>::_size]);
          if (!_randState)  // for the assignment operator
          {
	      //            IppStatus status = ippsRandGaussInitAlloc_32f(&_randState, _randMean, _randStdDev, _seed);
	      wipp::init_rand_gaussian(_randState, _randMean, _randStdDev);
	      //	      wipp::checkStatus(status);
          }
        }

        DSPONE_BASIC_PARTICLE_SET_TMP
        DSPONE_BASIC_PARTICLE_SET_DCL::~BasicParticleSet()
        {
          if (_randState)
          {
	      //            ippsRandGaussFree_32f(_randState);
	    wipp::delete_rand(_randState);
	    _randState = NULL;
          }
        }

        DSPONE_BASIC_PARTICLE_SET_TMP
        int DSPONE_BASIC_PARTICLE_SET_DCL::set(T_Particle p, unsigned  int pos)
        {
          if (pos < static_cast<unsigned int>(IParticleSet<T_Particle>::_size))
          {
            _set[pos] = p;
            return pos;
          }
          else
          {
            return -1;
          }
        }

        DSPONE_BASIC_PARTICLE_SET_TMP
        const T_Particle& DSPONE_BASIC_PARTICLE_SET_DCL::at(unsigned int pos) const
        {
          if (pos < static_cast<unsigned int>(IParticleSet<T_Particle>::_size))
          {
            return _set[pos];
          }
          else
          {
            throw(std::exception());
          }
        }

        DSPONE_BASIC_PARTICLE_SET_TMP
        void DSPONE_BASIC_PARTICLE_SET_DCL::normalise()
        {
          // Here I do minmax normalisation to assure that the values of the weight are all
          // possitive.
          T_Particle min, max;
	  wipp::minmax(_set.get(), IParticleSet<T_Particle>::_size, &min, &max);
	  wipp::subC(min, _set.get(), IParticleSet<T_Particle>::_size);
	  wipp::divC(max, _set.get(), IParticleSet<T_Particle>::_size);
        }

        DSPONE_BASIC_PARTICLE_SET_TMP
        T_Particle DSPONE_BASIC_PARTICLE_SET_DCL::sum() const
        {
          T_Particle value;
	  wipp::sum(_set.get(), IParticleSet<T_Particle>::_size, &value);
          return value;
        }

        DSPONE_BASIC_PARTICLE_SET_TMP
        void DSPONE_BASIC_PARTICLE_SET_DCL::addNoise(T_Particle mean, T_Particle std)
        {
//          IppStatus status = ippsRandGauss_32f(_randomValues.get(), IParticleSet<T_Particle>::_size, _randState);
	    wipp::rand(_randState, _randomValues.get(), IParticleSet<T_Particle>::_size);
	    //	    wipp::checkStatus(status);
	    wipp::multC(std, _randomValues.get(), IParticleSet<T_Particle>::_size);
	    wipp::addC(mean, _randomValues.get(), IParticleSet<T_Particle>::_size);
	    wipp::copyBuffer(_randomValues.get(), _randomParticles.get(), IParticleSet<T_Particle>::_size);
	    wipp::add(_randomParticles.get(), _set.get(), IParticleSet<T_Particle>::_size);
        }

        DSPONE_BASIC_PARTICLE_SET_TMP
        T_Particle DSPONE_BASIC_PARTICLE_SET_DCL::getRandomParticle() const
        {
          T_Particle value = (static_cast<double>(rand())/RAND_MAX)*(_range.second - _range.first) + _range.first;
          return value;
        }

        DSPONE_BASIC_PARTICLE_SET_TMP
        T_Particle DSPONE_BASIC_PARTICLE_SET_DCL::getStandardDeviation() const
        {
	  double stddev;
	  wipp::stddev(_set.get(), IParticleSet<T_Particle>::_size, &stddev);
          return stddev;
        }

        DSPONE_BASIC_PARTICLE_SET_TMP
        T_Particle* DSPONE_BASIC_PARTICLE_SET_DCL::get() const
        {
          return _set.get();
        }

        DSPONE_BASIC_PARTICLE_SET_TMP
        BasicParticleSet<T_Particle>& DSPONE_BASIC_PARTICLE_SET_DCL::operator =(const BasicParticleSet<T_Particle> &rset)
        {
          IParticleSet<T_Particle>::_size = rset._size;
          _set.reset(new T_Particle[IParticleSet<T_Particle>::_size]);
          _range.first  = rset._range.first;
          _range.second = rset._range.second;
	  wipp::copyBuffer(rset._set.get(), _set.get(), rset.size());
          init();
          return *this;
        }

#undef DSPONE_BASIC_PARTICLE_SET_TMP
#undef DSPONE_BASIC_PARTICLE_SET_DCL

        /* --------------------------------------------------------------*/
        /* -- Operators implementation between T_Particle and T_Weight --*/
        /* --------------------------------------------------------------*/

      template <class T_Particle, class T_Weight>
        inline BasicParticleSet<T_Particle> operator *(const BasicParticleSet<T_Particle> &pset, const BasicParticleSet<T_Weight> &wset)
        {
          BasicParticleSet<T_Particle> ret(pset.size());
          if (pset.size() >= wset.size())
	    wipp::mult(wset.get(), pset.get(), ret.get(), wset.size());
          return ret;
        }

      template <class T_Particle>
        inline BasicParticleSet<T_Particle> operator +(BasicParticleSet<T_Particle> pset, T_Particle p)
        {
          int size = pset.size();
          BasicParticleSet<T_Particle> ret(size);
	  //          ippsAddC_64f(pset.get(), p, ret.get(), size);
	  wipp::addC(p, pset.get(), ret.get(), size);
          return ret;
        }


        /* ---------------------------------------------------------------- */
        /* -- Default model implementation for Resampling and Prediction    -- */
        /* ---------------------------------------------------------------- */

#define DSPONE_PREDICTION_MODEL_TMP \
  template <class T_Particle, class T_ParticleSet>
#define DSPONE_PREDICTION_MODEL_DCL \
  PredictionModelImpl<T_Particle, T_ParticleSet>

        // factors MUST be < 1
#define MIN_VELOCITY_UPDATE_FACTOR 0.4
#define MAX_VELOCITY_UPDATE_FACTOR 0.7
#define INITIAL_VELOCITY_UPDATE_FACTOR 0.9

        DSPONE_PREDICTION_MODEL_TMP
        DSPONE_PREDICTION_MODEL_DCL::PredictionModelImpl(T_Particle inititalState, T_Particle initialVelocity, T_Particle mean, T_Particle std) :
          _prevParticle(inititalState),
          _velocity(initialVelocity),
          _velocityUpdateFactor(INITIAL_VELOCITY_UPDATE_FACTOR),
          _randMean(mean),
          _randStdDev(std)
        {
          // convert dyanmic convertions into compilation asserts, to avoid
          // hard to debug errors
          BOOST_STATIC_ASSERT_MSG( (boost::is_base_and_derived<IParticleSet<T_Particle>, T_ParticleSet>::value) ,
                                   "Ther particle set class provided do no inherit from IParticleSet");
        }

        DSPONE_PREDICTION_MODEL_TMP
        void DSPONE_PREDICTION_MODEL_DCL::update(T_ParticleSet &particles)
        {
          particles.addNoise(_velocity, 3*_velocity);
        }

        DSPONE_PREDICTION_MODEL_TMP
        void DSPONE_PREDICTION_MODEL_DCL::updateModel(T_Particle solution)
        {
          // Low pass iir filter to the velocity
          // v[n+1] = factor*v_estimated + (1-factor)*v[n-1]
          T_Particle prevVel = _velocity;
          _velocity *= (1 - _velocityUpdateFactor);
          _velocity += _velocityUpdateFactor * (solution - _prevParticle);
	  TRACE_STREAM("Vel: " << _velocity << " = (" << solution << " - " << _prevParticle << ")" << " update factor: " << _velocityUpdateFactor);
          _prevParticle = solution;
          if (_velocity > prevVel)
            _velocityUpdateFactor = sqrt(_velocityUpdateFactor);
          else
            _velocityUpdateFactor *= _velocityUpdateFactor;
          _velocityUpdateFactor = std::max(_velocityUpdateFactor, MIN_VELOCITY_UPDATE_FACTOR);
          _velocityUpdateFactor = std::min(_velocityUpdateFactor, MAX_VELOCITY_UPDATE_FACTOR);
        }

#undef DSPONE_PREDICTION_MODEL_TMP
#undef DSPONE_PREDICTION_MODEL_DCL



#define DSPONE_REASMPLING_MODEL_TMP \
  template <class T_Particle, class T_ParticleSet, class T_Weight, class T_WeightSet>
#define DSPONE_RESAMPLING_MODEL_DCL \
  ResamplingModelImpl< T_Particle, T_ParticleSet, T_Weight, T_WeightSet>


        DSPONE_REASMPLING_MODEL_TMP
        DSPONE_RESAMPLING_MODEL_DCL::ResamplingModelImpl()
        {
          // convert dyanmic convertions into compilation asserts, to avoid
          // hard to debug errors
          BOOST_STATIC_ASSERT_MSG( (boost::is_base_and_derived<IParticleSet<T_Weight>, T_WeightSet>::value),
                                   "Ther weight set class provided do no inherit from IParticleSet");
          BOOST_STATIC_ASSERT_MSG( (boost::is_base_and_derived<IParticleSet<T_Particle>, T_ParticleSet>::value) ,
                                   "Ther particle set class provided do no inherit from IParticleSet");

        }

        DSPONE_REASMPLING_MODEL_TMP
        void DSPONE_RESAMPLING_MODEL_DCL::resample(const T_ParticleSet &currentSet, T_ParticleSet &resampledSet, const T_WeightSet &weights)
        {

        int size = currentSet.size();
          if (size != resampledSet.size() || weights.size() != size)
          {
	    ERROR_STREAM("You provided sets with different sizes, so the particle set has not been resampled");
            return;
          }

          T_Weight total = weights.sum();
          int resampledSize = size*0.99;
          //          int randomSize = size - resampledSize;

          // This plus one, is to avoid step to be equal to cumSum, bacause if so
          // then the last particle in the set is always included despite the value
          // of its corresponding weight.
          T_Weight step = total / (resampledSize + 1);
          if (step <= 0)
          {
	    ERROR_STREAM("Step in particle filter resampling is negative or zero, particle filter is not realiable now");
            return;
          }

          T_Weight cumSum = 0;
          T_Weight steps = step;
          int istep = 0;
          for (int iparticle = 0; iparticle < size; ++iparticle)
          {
            cumSum += weights.at(iparticle);
            while (steps < cumSum)
            {
              resampledSet.set(currentSet.at(iparticle), istep);
              steps += step;
              ++istep;
            }
          }

          for (; istep < resampledSize; ++istep)
          {
            resampledSet.set(currentSet.at(currentSet.size() -1), istep);
          }

          for (; istep < size; ++istep)
          {
            resampledSet.set(resampledSet.getRandomParticle() ,istep);
          }
        }

#undef DSPONE_REASMPLING_MODEL_TMP
#undef DSPONE_RESAMPLING_MODEL_DCL



#define DSPONE_OBSERVATION_MODEL_TMP \
  template <class T_Particle, class T_Weight>

#define DSPONE_OBSERVATION_MODEL_DCL \
  ObservationModelImpl<T_Particle, T_Weight>


        DSPONE_OBSERVATION_MODEL_TMP
        DSPONE_OBSERVATION_MODEL_DCL::ObservationModelImpl() : _variance(100)
        {
        }

        DSPONE_OBSERVATION_MODEL_TMP
        DSPONE_OBSERVATION_MODEL_DCL::~ObservationModelImpl()
        {
        }

        DSPONE_OBSERVATION_MODEL_TMP
        BasicParticleSet<T_Weight> DSPONE_OBSERVATION_MODEL_DCL::getWeights(BasicParticleSet<T_Particle> &particles)
        {
          size_t size = particles.size();
          BasicParticleSet<T_Weight> weights(particles.size());
          for (size_t i = 0; i < size; ++i)
          {
            T_Weight w = exp(-(particles.at(i)*particles.at(i))/_variance) + 1e-10;
            weights.set(w, i);
          }
          return weights;
        }

        DSPONE_OBSERVATION_MODEL_TMP
        void DSPONE_OBSERVATION_MODEL_DCL::updateModel() {}

#undef DSPONE_OBSERVATION_MODEL_TMP
#undef DSPONE_OBSERVATION_MODEL_DCL

      }





