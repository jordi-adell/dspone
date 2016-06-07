/*
* ResamplingModel.hpp
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
#ifndef __PARTICLE_RESAMPLING_MODEL_H_
#define __PARTICLE_RESAMPLING_MODEL_H_

#include <dspone/pf/ParticleSet.hpp>
#include <dspone/dsplogger.h>


namespace dsp {

/**
	 * @brief The ResamplingModel class
	 * This class implements a resampling algorimth for the IResamplingModel
	 * the resampling pf consists on sampling at equiespaced periods the
	 * the cummulative sum of the wegiths in a way that most probable samples
	 * (i.e., higher waits) appear more times.
	 */

template <class T_Particle,
	  class T_Weight,
	  class T_ParticleSet=ParticleSet<T_Particle>,
	  class T_WeightSet=ParticleSet<T_Weight> >
class ResamplingModel : public IResamplingModel<T_ParticleSet>
{
    public:
	ResamplingModel();
	/**
	   * @brief resample  Implements the Low Vriance resmapling method.
	   * @param currentSet   current set of particles
	   * @param resampledSet   resampled set of particles.
	   * @param weights   weights for the current particle set
	   */
	virtual void resample(const T_ParticleSet &currentSet, T_ParticleSet &resampledSet, const T_WeightSet &weights);
};



#define DSPONE_REASMPLING_MODEL_TMP \
    template <class T_Particle, class T_Weight, class T_ParticleSet, class T_WeightSet>
#define DSPONE_RESAMPLING_MODEL_DCL \
    ResamplingModel< T_Particle,  T_Weight, T_ParticleSet, T_WeightSet>


template <class T_Weight, class T_Particle>
ResamplingModel<T_Weight, T_Particle>*
make_resampling_model()
{
    return new ResamplingModel<T_Weight, T_Particle>();
}


DSPONE_REASMPLING_MODEL_TMP
DSPONE_RESAMPLING_MODEL_DCL::ResamplingModel()
{
#ifdef BOOST_FOUND
    // convert dyanmic convertions into compilation asserts, to avoid
    // hard to debug errors
    BOOST_STATIC_ASSERT_MSG( (boost::is_base_and_derived<IParticleSet<T_Weight>, T_WeightSet>::value),
			     "Ther weight set class provided do no inherit from IParticleSet");
    BOOST_STATIC_ASSERT_MSG( (boost::is_base_and_derived<IParticleSet<T_Particle>, T_ParticleSet>::value) ,
			     "Ther particle set class provided do no inherit from IParticleSet");
#endif
}

DSPONE_REASMPLING_MODEL_TMP
void DSPONE_RESAMPLING_MODEL_DCL::resample(const T_ParticleSet &currentSet,
					   T_ParticleSet &resampledSet,
					   const T_WeightSet &weights)
{

    int size = currentSet.size();
    if (size != resampledSet.size() || weights.size() != size)
    {
	ERROR_STREAM("You provided sets with different sizes, so the particle set has not been resampled");
	return;
    }

    T_Weight total = weights.sum();
    int resampledSize = size*0.99;

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


}


#endif // __PARTICLEFILTERIMPL_H_
