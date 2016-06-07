/*
* PredictionModel.hpp
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
#ifndef __PARTICLE_F_PREDICTION_MODEL_H_
#define __PARTICLE_F_PREDICTION_MODEL_H_

#include <dspone/pf/IPredictionModel.h>
#include <dspone/pf/ParticleSet.hpp>

namespace dsp {


/* ---------------------------------------------------------------- */
/* -- Default model implementation for Resampling and Prediction -- */
/* ---------------------------------------------------------------- */

/**
	 * @brief The PredictionModel class
	 * Implements a prediction model. This models keeps the velocity information measured
	 * as velocity[n] = a*veloctiy[n-1] + (1-a)*(Particle[n] - Particle[n-1])
	 * and the prediction of the next particle is done projecting this velocity to the next iteration
	 * by doing: Particle[n] = Particle[n-1] + velocity[n]
	 */
template <class T_Particle,
	  class T_ParticleSet=ParticleSet<T_Particle> >
class PredictionModel : public IPredictionModel<T_Particle, T_ParticleSet>
{
    public:
	/**
	   * @brief PredictionModel   constructs a model from a initial states
	   * that is used as Particle[n-1] and a initial velocity. This velocity will
	   * be used in the first prediction.
	   * @param inititalState   initial particle
	   * @param initialVelocity    initial velocity of the solution to track.
	   */
	PredictionModel(T_Particle inititalState, T_Particle initialVelocity,
			    T_Particle mean=0, T_Particle std=1);

	/*****    implemented function of the abstract class ******/
	virtual void update(T_ParticleSet &particles);
	virtual void updateModel(T_Particle solution);
	/*****                                                *****/

	typedef IPredictionModel<T_Particle, T_ParticleSet> base_type;

    private:

	T_Particle _prevParticle;/**< previous solution used to estimaate the veloctity **/
	T_Particle _velocity; /**< current estimate velocity **/
	double _velocityUpdateFactor;  /**< update factor (i.e., 'a' in the equation above **/
	static constexpr double _velocityMinUpdateFactor = 0.2;  /**< update factor (i.e., 'a' in the equation above **/
	const double _randMean; /**< mean of the random distribution added to the updated particles **/
	const double _randStdDev; /**< standard deviation of the randoim distribution added to the updated particles **/
};



#define DSPONE_PREDICTION_MODEL_TMP \
  template <class T_Particle, class T_ParticleSet>
#define DSPONE_PREDICTION_MODEL_DCL \
  PredictionModel<T_Particle, T_ParticleSet>

template <class T_Particle>
PredictionModel<T_Particle>*
make_prediction_model(T_Particle initialState,
		      T_Particle initialVelocity,
		      T_Particle mean = 0,
		      T_Particle stddev  = 1)
{
    return new PredictionModel<T_Particle>(initialState, initialVelocity, mean, stddev);
}


// factors MUST be < 1
#define MIN_VELOCITY_UPDATE_FACTOR 0.4
#define MAX_VELOCITY_UPDATE_FACTOR 0.7
#define INITIAL_VELOCITY_UPDATE_FACTOR 0.9

	DSPONE_PREDICTION_MODEL_TMP
	DSPONE_PREDICTION_MODEL_DCL::PredictionModel(T_Particle inititalState, T_Particle initialVelocity, T_Particle mean, T_Particle std) :
	  _prevParticle(inititalState),
	  _velocity(initialVelocity),
	  _velocityUpdateFactor(INITIAL_VELOCITY_UPDATE_FACTOR),
	  _randMean(mean),
	  _randStdDev(std)
	{
#ifdef BOOST_FOUND
	  // convert dyanmic convertions into compilation asserts, to avoid
	  // hard to debug errors
	  BOOST_STATIC_ASSERT_MSG( (boost::is_base_and_derived<IParticleSet<T_Particle>, T_ParticleSet>::value) ,
				   "Ther particle set class provided do no inherit from IParticleSet");
#endif
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



}

#endif // __PARTICLEFILTERIMPL_H_
