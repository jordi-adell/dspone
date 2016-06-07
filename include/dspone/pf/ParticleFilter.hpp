/*
* ParticleFilter.h
* Copyright 2016 (c) Jordi Adell
* Created on: 2015
* 	Author: Jordi Adell - adellj@gmail.com
*
* This file is part of DSPONE
*
* DSPONE is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DSPONE is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DSPONE.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __PARTICLEFILTER_H_
#define __PARTICLEFILTER_H_

#include <dspone/pf/IParticleSet.h>
#include <dspone/pf/IObservationModel.h>
#include <dspone/pf/IResamplingModel.h>
#include <dspone/pf/IPredictionModel.h>
#include <dspone/pf/IParticleFilterCallback.h>

#include <dspone/dsplogger.h>

#ifdef BOOST_FOUND
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#endif

namespace dsp {

/**
 * @brief The ParticleFilter class
 * This class runs a particle filter. At each iteration (a call to
 * the function updateFilter() )
 * You can also set a callback to recieve the solution condensed
 * at every iteration or get the particle set stored in each iteration.
 * The particle filter is implemented using templates so that you can pass
 * any ParticleSet that inherits from IParticleSet containing T_Particles
 * go an arbitrary type. The same happens for the weights.
 *
 * You also need to implement you ObservationModel, ResamplingModel and PredictionModel.
 * They must inherit from IObservationModel, IResamplingModel and IPredictionModel.
 *
 * If any of them are not passed to the constructor, then the default implementation
 * is used. The default implementation is defined in IPartcielFilterImpl.h For Particle
 * and weight sets as well as for models.
*/
template<class T_Particle,
	 class T_ID,
	 class T_ParticleSet,
	 class T_WeightSet>
class ParticleFilter
{
    private:
	/***** Interfaces renaming to clear the code. *****/

	/**
	   * @brief ObservationModel
	   * Type definition for the ObservationModel used in the construction of the particle filter.
	   */
	typedef IObservationModel<T_ParticleSet, T_WeightSet> ObservationModel_t;
	/**
	   * @brief PredictionModel
	   * Type definition for the definition used in the construction of the particle filter.
	   */
	typedef IPredictionModel<T_Particle, T_ParticleSet > PredictionModel_t;
	/**
	   * @brief ResamplingModel
	   * Type definition for the ResamplingModel used in the construction of the particle filter.
	   */
	typedef IResamplingModel<T_ParticleSet> ResamplingModel_t;

    public:
	/**
	   * @brief ParticleFilter Constructor with the initial state of the particle
	   * filter, the amount of particles to be used and the models
	   * It RETAINS ownership of the model pointers.
	   * @param initialState the initial state for the filter.
	   * @param numParticles  the number of particles to be used for the tracking
	   * @param obsModel    Observation model
	   * @param predModel   Prediction model
	   * @param resModel  Resampling model
	   */
	ParticleFilter(T_Particle initialState,
		       int numParticles,
		       T_ID id,
		       std::pair<T_Particle, T_Particle> range,
		       ObservationModel_t *obsModel,
		       PredictionModel_t *predModel,
		       ResamplingModel_t *resModel);

	~ParticleFilter();

	/**
	   * @brief updateFilter   This function has to be called at each
	   * iteration of the filter.
	   * @return   the condensed solution
	   */
	T_Particle updateFilter();


	/**
	   * @brief setCallback  Use this function if you want callback to be
	   * called at each iteration of the particle functino with the value of the
	   * condensed solution.
	   * @param callback
	   */
	void setCallback(IParticleFilterCallback<T_Particle> *callback);

	/**
	   * @brief getParticles   Return the set of particles in the current iteration.
	   * This might be usefull for drawing the current state of the filter, o just for tacking
	   * some statistics about the evloution of the particle filter.
	   * @return the particles
	   */
	T_ParticleSet getParticles() const;

	/**
	   * @brief getWeights  Return the set of wweights in the current iteration.
	   * This might be usefull for drawing the current state of the filter, o just for taking
	   * some statistics about the evloution of the particle filter.
	   * @return the weights
	   */
	T_WeightSet getWeights() const;


	T_ID getId() const;


    private:

	const int _order;  /**< order of the filter (i.e., the number of particles used) **/

	T_Particle _initialState;  /**< Particle used to initialise the filter **/
	std::pair<T_Particle, T_Particle> _range; /**< range of valid values for a particle set **/
	T_ParticleSet _particles; /**< current set of particles **/
	T_ParticleSet  _resampledParticles; /**< particles resulting from the resampling process **/
	T_ParticleSet _weights; /**< weights assigned to the set of particles in the current iteration **/
	IParticleFilterCallback<T_Particle> *_callback; /**< callback to call at each iteration with the condensed solution **/
	T_ID _id;

	// Pointer to model interfaces
	ObservationModel_t *_observationModel; /**< ptr to the Observation model **/
	PredictionModel_t *_predictionModel; /**< ptr to the prediction model **/
	ResamplingModel_t *_resamplingModel; /**< ptr to the resampling model **/
	// Default implementation
	//	boost::shared_ptr<DefaultPredictionModel> _predictionModelImpl; /**< default predictiom model (used if non given) **/
	//	boost::shared_ptr<DefaultResamplingModel> _resamplingModelImpl; /**< default resampling model (used if non given) **/

	/**
	   * @brief init   Initialisation function
	   * Allocates space for the particle and weight sets

	   * and creates the default models that have not been
	   * passed in the constructor.
	   */
	void init();

	/**
	   * @brief resamplingStep
	   * This function performes the resampling of the particle set, and
	   * sets de value of particle in _resmapledParticles.
	   * The resampling process is done by the resmapling model
	   * and is intended to select the most relevant particle given the weights
	   * to improve the accuracy of the observation probability distribution sampling.
	   */
	void resamplingStep();
	/**
	   * @brief predictionStep
	   * This function performed the prediction for the next iteration.
	   * Is done using the preduction models and is intended to generate the expected
	   * position of the current particles in the next iteration. The models is expected also to add
	   * some noise to the prediction itself.
	   */
	void predictionStep();
	/**
	   * @brief measurmentStep
	   * Performed the mesurement and asigns weights to the particle set.
	   * It uses the observation model and assign a weight to each particle
	   * that has to be related to the probability of the current particle being the correct
	   * hiden state (i.e., solution) given the current observations measured by the model.
	   */
	void measurmentStep();

};


#define DSPONE_PARTICLE_FILTER_TMPL \
    template<class T_Particle, class T_ID, class T_ParticleSet, class T_WeightSet>

#define DSPONE_PARTICLE_FILTER_DCL \
    ParticleFilter<T_Particle, T_ID, T_ParticleSet, T_WeightSet>

/**
   * @brief make_particle_filter
   * Function to create a particle filter with
   * out needing to specify the full template declaration.
   * @param initialState    (same as in constructor)
   * @param numParticles (same as in constructor)
   * @param obsModel (same as in constructor)
   * @param predModel (same as in constructor)
   * @param resModel (same as in constructor)
   * @return
   */
DSPONE_PARTICLE_FILTER_TMPL
DSPONE_PARTICLE_FILTER_DCL
make_particle_filter(T_Particle initialState,
		     int numParticles,
		     T_ID id,
		     std::pair<T_Particle, T_Particle> range,
		     IObservationModel<T_ParticleSet, T_WeightSet> *obsModel,
		     IPredictionModel<T_Particle, T_ParticleSet> *predModel,
		     IResamplingModel<T_ParticleSet> *resModel)
{
    return DSPONE_PARTICLE_FILTER_DCL(initialState,  numParticles, id, range, obsModel, predModel, resModel);
}



DSPONE_PARTICLE_FILTER_TMPL
DSPONE_PARTICLE_FILTER_DCL::ParticleFilter(T_Particle initialState,
					   int numParticles,
					   T_ID id,
					   std::pair<T_Particle, T_Particle> range,
					   ObservationModel_t *obsModel,
					   PredictionModel_t *predModel,
					   ResamplingModel_t *resModel) :
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
DSPONE_PARTICLE_FILTER_DCL::~ParticleFilter()
{
    delete _observationModel;
    delete _predictionModel;
    delete _resamplingModel;
}


DSPONE_PARTICLE_FILTER_TMPL
void DSPONE_PARTICLE_FILTER_DCL::init()
{
    // convert dyanmic convertions into compilation asserts, to avoid
    // hard to debug errors
    //    BOOST_STATIC_ASSERT_MSG( (boost::is_base_and_derived<IParticleSet<T_Weight>, T_WeightSet>::value),
    //			     "Ther weight set class provided do no inherit from IParticleSet");
#ifdef BOOST_FOUND
    BOOST_STATIC_ASSERT_MSG( (boost::is_base_and_derived<IParticleSet<T_Particle>, T_ParticleSet>::value) ,
			     "Ther particle set class provided do no inherit from IParticleSet");
#endif
    // Variable initialisation
    _resampledParticles = T_ParticleSet(_order, _initialState, _range.first, _range.second);
    _particles = T_ParticleSet(_order, _initialState,  _range.first, _range.second);
    _particles.addNoise(0,1);
    _weights = T_WeightSet(_order, static_cast<double>(1)/_order);

}

DSPONE_PARTICLE_FILTER_TMPL
T_Particle DSPONE_PARTICLE_FILTER_DCL::updateFilter()
{

    predictionStep();
    resamplingStep();
    measurmentStep();


    // This probably could be replaced by a condesantion model
    // in a similar way to the observation and resampling models
    T_Particle solution = ((_particles * _weights).sum());
    auto sum = _weights.sum();
    if (sum != 0)
	solution = solution / sum;
    else
	ERROR_STREAM("Particle weights are zero, so you are really far away of the target");
    TRACE_STREAM("Estimation: " << (_particles * _weights).sum() << " " << _weights.sum() << " " << solution);
    _predictionModel->updateModel(solution);

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
#endif // PARTICLEFILTER_H_
