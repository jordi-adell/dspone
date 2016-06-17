/*
* IResamplingModel.h
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
#ifndef __IRESAMPLING_MODEL_H_
#define __IRESAMPLING_MODEL_H_


namespace dsp {

/**
   * @brief The IResamplingModel class
   * This class is an interface for a model that selects the most
   * relevant particles in a set to create a new set. The relevance is based on the
   * weights assigned to each particle.
   */
template<class T_ParticleSet, class T_WeightSet = T_ParticleSet>
class IResamplingModel
{
    public:
	IResamplingModel() {}
	virtual ~IResamplingModel() {}
	/**
     * @brief resample   function that resamples the currentSet and generates
     * a new resmapledSet with the most relevant particles in the previous set.
     * @param currentSet  current set of particles
     * @param resampledSet   new set with the chosen particles.
     * @param weights   weights assigned by the observation model to each particle in the current set.
     */
	virtual void resample(const T_ParticleSet &currentSet,T_ParticleSet  &resampledSet, const T_WeightSet &weights) = 0;
};



}

#endif // __IPARTICLEFILTER_H_
