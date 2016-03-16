/*
* ParticleFilterImpl.h
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
#ifndef __PARTICLEFILTERIMPL_H_
#define __PARTICLEFILTERIMPL_H_

#include "IParticleFilter.h"
#include <boost/type_traits.hpp>
#include <wipp/wipputils.h>
#include <wipp/wippstats.h>

namespace dsp {

/* --------------------------------------------------------- */
/* -- Implementation for basic type (float, double, etc ...) */
/* --------------------------------------------------------- */

template <class T_Particle=double>
/**
	 * @brief The BasicParticleSet class
	 * This is an implementation of IParticleSet
	 * needed for the ParticleFilter that will work
	 * with particles that are of a basic type: int, float, double, ...
	 * The default is double
	 */
class BasicParticleSet : public IParticleSet<T_Particle>
{
    public:
	/**
	   * @brief BasicParticleSet constructs a BasicParticle set
	   * of the specified size
	   * @param set  shared array of particles (just the particles)
	   * @param size  the length of the array.
	   */
	BasicParticleSet(boost::shared_array<T_Particle> set, int size, T_Particle min = 0, T_Particle max = 1);
	/**
	   * @brief BasicParticleSet Constructs a particle set
	   * of the specified size.
	   * @param size  of the particle set
	   */

	BasicParticleSet(int size = 0, T_Particle p = 0, T_Particle min = 0, T_Particle = 1);
	/**
	   * @brief BasicParticleSet
	   * Copy constructore
	   * @param particleSet
	   */
	BasicParticleSet(const BasicParticleSet<T_Particle> &particleSet);

	virtual ~BasicParticleSet();

	/**
	   * @brief init   Allocates internal variables
	   * and initialises the random number generator.
	   */
	void init();

	/**
	   * @brief get   Returns a pointer to the particle set.
	   * The class retains ownership of the pointer.
	   * This can be usefull to use IPP functions for examples.
	   * @return   a pointer to a particles
	   */
	T_Particle *get() const;

	/**
	   * @brief Assignement operator
	   * @param rset the right side of the operator.
	   * @return
	   */
	inline BasicParticleSet<T_Particle>& operator =(const BasicParticleSet<T_Particle> &rset);

	/*** Implemented abstract functions ***/
	virtual int set(T_Particle p, unsigned  int pos);
	virtual const T_Particle &at(unsigned int pos) const;
	virtual void normalise();
	virtual T_Particle sum() const;
	virtual void addNoise(T_Particle mean, T_Particle std);
	virtual T_Particle getRandomParticle() const;
	virtual T_Particle getStandardDeviation() const;
	/*******                         *******/


    private:
	boost::shared_array<T_Particle> _set; /**< The particle set **/
	boost::shared_array<double> _randomValues;  /**<  a temporal variable to store random values **/
	boost::shared_array<T_Particle> _randomParticles; /**< the random values converted into particles are stored here **/
	wipp::wipp_rand_t *_randState;  /**< DSPONE statet used to generate random variables **/

	std::pair<T_Particle, T_Particle> _range; /**< Stores the possible range for the values in the set (min, max) **/
	static constexpr int _randMean   = 0;  /**< mean of the random distribution **/
	static constexpr int _randStdDev = 1;  /**< standard deviation of the random distribution **/
	static constexpr unsigned int _seed = 10;  /**< seed used in the random distribution **/
};

/* --------------------------------------------------------------*/
/* -- Operators implementation between T_Particle and T_Weight --*/
/* --------------------------------------------------------------*/

template <class T_Particle=double, class T_Weight=double>
inline BasicParticleSet<T_Particle> operator *(const BasicParticleSet<T_Particle> &pset, const BasicParticleSet<T_Weight> &wset);

template <class T_Particle=double>
inline BasicParticleSet<T_Particle> operator +(BasicParticleSet<T_Particle> pset, T_Particle p);

/* ---------------------------------------------------------------- */
/* -- Default model implementation for Resampling and Prediction -- */
/* ---------------------------------------------------------------- */

/**
	 * @brief The PredictionModelImpl class
	 * Implements a prediction model. This models keeps the velocity information measured
	 * as velocity[n] = a*veloctiy[n-1] + (1-a)*(Particle[n] - Particle[n-1])
	 * and the prediction of the next particle is done projecting this velocity to the next iteration
	 * by doing: Particle[n] = Particle[n-1] + velocity[n]
	 */
template <class T_Particle=double, class T_ParticleSet=BasicParticleSet<T_Particle> >
class PredictionModelImpl : public IPredictionModel<T_Particle, T_ParticleSet>
{
    public:
	/**
	   * @brief PredictionModelImpl   constructs a model from a initial states
	   * that is used as Particle[n-1] and a initial velocity. This velocity will
	   * be used in the first prediction.
	   * @param inititalState   initial particle
	   * @param initialVelocity    initial velocity of the solution to track.
	   */
	PredictionModelImpl(T_Particle inititalState, T_Particle initialVelocity,
			    T_Particle mean=0, T_Particle std=0.1);

	/*****    implemented functino of the abstract class ******/
	virtual void update(T_ParticleSet &particles);
	virtual void updateModel(T_Particle solution);
	/*****                                                *****/

    private:

	T_Particle _prevParticle;/**< previous solution used to estimaate the veloctity **/
	T_Particle _velocity; /**< current estimate velocity **/
	double _velocityUpdateFactor;  /**< update factor (i.e., 'a' in the equation above **/
	static constexpr double _velocityMinUpdateFactor = 0.2;  /**< update factor (i.e., 'a' in the equation above **/
	const double _randMean; /**< mean of the random distribution added to the updated particles **/
	const double _randStdDev; /**< standard deviation of the randoim distribution added to the updated particles **/
};




/**
	 * @brief The ResamplingModelImpl class
	 * This class implements a resampling algorimth for the IResamplingModel
	 * the resampling algorithm consists on sampling at equiespaced periods the
	 * the cummulative sum of the wegiths in a way that most probable samples
	 * (i.e., higher waits) appear more times.
	 */
template <class T_Particle=double,
	  class T_ParticleSet=BasicParticleSet<T_Particle> ,
	  class T_Weight=T_Particle,
	  class T_WeightSet=BasicParticleSet<T_Weight> >
class ResamplingModelImpl : public IResamplingModel<T_ParticleSet>
{
    public:
	ResamplingModelImpl();
	/**
	   * @brief resample  Implements the Low Vriance resmapling method.
	   * @param currentSet   current set of particles
	   * @param resampledSet   resampled set of particles.
	   * @param weights   weights for the current particle set
	   */
	virtual void resample(const T_ParticleSet &currentSet, T_ParticleSet &resampledSet, const T_WeightSet &weights);
};




template <class T_Particle=double, class T_Weight=T_Particle>
/**
	 * @brief The ObservationModelImpl class
	 * Dummy class for testing purposes. The observation model depens on the taks to be performed and
	 * needs measurement of sensors to estimage the weights of the particles.
	 * This class sets the weight of all the samples to the same weight 1/size
	 */
class ObservationModelImpl : public IObservationModel<BasicParticleSet<T_Particle>, BasicParticleSet<T_Weight> >
{

    public:

	ObservationModelImpl();
	virtual ~ObservationModelImpl();
	/**
	   * @brief getWeights   Returns the weights of a set of particles.
	   * The weights are set given a gaussian distribution of zero mean and a certaing
	   * variance, this implides that the particle filter must converge to zero when
	   * used with this observation model.
	   * @param particles   particles for whose the weights must be calculated.
	   * @return  a set of weights.
	   */
	virtual BasicParticleSet<T_Weight> getWeights(BasicParticleSet<T_Particle> &particles);
	/**
	   * @brief updateModel
	   * This function does nothing for this implementation.
	   */
	virtual void updateModel();
    private:
	const double _variance; /**< of the gaussian distribution used to set the weights. **/
};


}


#include <dspone/ParticleFilterImpl.inl>

#endif // __PARTICLEFILTERIMPL_H_
