/*
* IObservationModel.h
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
#ifndef __IOBSERVATION_MODEL_H_
#define __IOBSERVATION_MODEL_H_


namespace dsp {

/**
   * @class IObservationModel
   * @brief This class template is an interface to implement Observation models
   * for the particle filter. An obersvation model is a class that is intended
   * to assign a probability to each particle of a set, given the current observations.
   */
template <class T_ParticleSet, class T_WeightSet=T_ParticleSet>
class IObservationModel
{
    public:

	IObservationModel() {}
	virtual ~IObservationModel() {}

	/**
     * @brief getWeights  calcualte the weight that has to be
     * applied to each particle in the given set.
     * @param particle  the particle set to calcualte weights for.
     * @return  a set of weights
     */
	virtual T_WeightSet getWeights(const T_ParticleSet &particle) const = 0;
	/**
     * @brief updateModel
     * If the observation model need to get new observation to recalculate
     * some parameters of the model, you have to by this code here.
     * This function will be called at each iteraction of the particle
     * filter.
     */
	virtual void updateModel() = 0; // by Getting observations
};





}

#endif // __IOBSERVATION_MODEL_H_
