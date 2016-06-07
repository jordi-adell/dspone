/*
* IPredictionModel.h
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
#ifndef __IPREDICTION_MODEL_H_
#define __IPREDICTION_MODEL_H_



namespace dsp {

template <class T_Particle, class T_ParticleSet>
/**
     * @brief The IPredictionModel class
     * Generates a prediction of particles for the next iteration of the particle
     * filtes given the previous results.
     */
class IPredictionModel
{
    public:
	IPredictionModel(){}
	virtual ~IPredictionModel(){}
	/**
     * @brief update   This fucntion is intended to generate a new particle set
     * based on the given particle set and previous solutions. It might for example
     * use previous velocity to predict the new position of each of the particles.
     * @param particles  the particle set to update.
     */
	virtual void update(T_ParticleSet &particles) = 0;
	/**
     * @brief updateModel  At each iteration of the particle filter
     * the condensed solution is passed to this class so that the model parameters
     * can be updated.
     * @param solution   The solution in the current iteration of the particle filter.
     */
	virtual void updateModel(T_Particle solution) = 0;
};


}

#endif // __IPARTICLEFILTER_H_
