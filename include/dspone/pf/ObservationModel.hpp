/*
* ObservationModel.hpp
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

#ifndef __PARTICLE_F_OBSERVATION_MODEL_H_
#define __PARTICLE_F_OBSERVATION_MODEL_H_

#include <dspone/pf/IObservationModel.h>
#include <dspone/pf/ParticleSet.hpp>

namespace dsp {

template <class T_Weight, class T_Particle>
/**
	 * @brief The ObservationModel class
	 * Dummy class for testing purposes. The observation model depens on the taks to be performed and
	 * needs measurement of sensors to estimage the weights of the particles.
	 * This class sets the weight of all the samples to the same weight 1/size
	 */
class ObservationModel : public IObservationModel<ParticleSet<T_Particle>, ParticleSet<T_Weight> >
{

    public:

	ObservationModel(T_Particle mean, T_Particle std);
	virtual ~ObservationModel();
	/**
	   * @brief getWeights   Returns the weights of a set of particles.
	   * The weights are set given a gaussian distribution of certain mean and a certaing
	   * variance, this implides that the particle filter must converge to the specified
	   * mean when using with this observation model.
	   * @param particles   particles for whose the weights must be calculated.
	   * @return  a set of weights.
	   */
	virtual ParticleSet<T_Weight> getWeights(const ParticleSet<T_Particle> &particles) const;
	/**
	   * @brief updateModel
	   * This function does nothing for this implementation.
	   */
	virtual void updateModel();
    private:
	const T_Particle _variance; /**< of the gaussian distribution used to set the weights. **/
	const T_Particle _mean; /**< of the gaussian distribution used to set the weights. **/
};



#define DSPONE_OBSERVATION_MODEL_TMP \
  template <class T_Weight, class T_Particle>

#define DSPONE_OBSERVATION_MODEL_DCL \
  ObservationModel<T_Weight,T_Particle>


template <class T_Weight, class T_Particle>
ObservationModel<T_Weight, T_Particle>*
make_observation_model(T_Particle mean, T_Particle stddev)
{
    return new ObservationModel<T_Weight, T_Particle>(mean, stddev);
}

	DSPONE_OBSERVATION_MODEL_TMP
	DSPONE_OBSERVATION_MODEL_DCL::ObservationModel(T_Particle mean, T_Particle stddev) :
	    _mean (mean) , _variance(stddev*stddev)
	{
	}

	DSPONE_OBSERVATION_MODEL_TMP
	DSPONE_OBSERVATION_MODEL_DCL::~ObservationModel()
	{
	}

	DSPONE_OBSERVATION_MODEL_TMP
	ParticleSet<T_Weight> DSPONE_OBSERVATION_MODEL_DCL::getWeights(const ParticleSet<T_Particle> &particles) const
	{
	  size_t size = particles.size();
	  ParticleSet<T_Weight> weights(particles.size());
	  for (size_t i = 0; i < size; ++i)
	  {
	    T_Weight w = exp(-pow(particles.at(i)-_mean,2)/_variance) + 1e-10;
	    weights.set(w, i);
	  }
	  return weights;
	}

	DSPONE_OBSERVATION_MODEL_TMP
	void DSPONE_OBSERVATION_MODEL_DCL::updateModel() {}

#undef DSPONE_OBSERVATION_MODEL_TMP
#undef DSPONE_OBSERVATION_MODEL_DCL

}


#endif // __PARTICLEFILTERIMPL_H_
