/*
* ParticleSet.h
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
#ifndef __PARTICLE_F_BASIC_PARTICLE_SET_H_
#define __PARTICLE_F_BASIC_PARTICLE_SET_H_

#include <dspone/pf/IParticleSet.h>

#include <dspone/dspMath.h>

namespace dsp {


/* --------------------------------------------------------- */
/* -- Implementation for basic type (float, double, etc ...) */
/* --------------------------------------------------------- */

template <class T_Particle=double>
/**
	 * @brief The ParticleSet class
	 * This is an implementation of IParticleSet
	 * needed for the ParticleFilter that will work
	 * with particles that are of a basic type: int, float, double, ...
	 * The default is double
	 */
class ParticleSet : public IParticleSet<T_Particle>
{
    public:
	/**
	   * @brief ParticleSet constructs a BasicParticle set
	   * of the specified size
	   * @param set  shared array of particles (just the particles)
	   * @param size  the length of the array.
	   */
	ParticleSet(boost::shared_array<T_Particle> set, int size, T_Particle min = 0, T_Particle max = 1);
	/**
	   * @brief ParticleSet Constructs a particle set
	   * of the specified size.
	   * @param size  of the particle set
	   */

	ParticleSet(int size = 0, T_Particle p = 0, T_Particle min = 0, T_Particle = 1);
	/**
	   * @brief ParticleSet
	   * Copy constructore
	   * @param particleSet
	   */
	ParticleSet(const ParticleSet<T_Particle> &particleSet);

	virtual ~ParticleSet();

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
	inline ParticleSet<T_Particle>& operator =(const ParticleSet<T_Particle> &rset);

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
	math::RandState *_randState;  /**< DSPONE statet used to generate random variables **/

	std::pair<T_Particle, T_Particle> _range; /**< Stores the possible range for the values in the set (min, max) **/
	static constexpr int _randMean   = 0;  /**< mean of the random distribution **/
	static constexpr int _randStdDev = 1;  /**< standard deviation of the random distribution **/
	static constexpr unsigned int _seed = 10;  /**< seed used in the random distribution **/
};

/* --------------------------------------------------------------*/
/* -- Operators implementation between T_Particle and T_Weight --*/
/* --------------------------------------------------------------*/

template <class T_Particle=double, class T_Weight=double>
inline ParticleSet<T_Particle> operator *(const ParticleSet<T_Particle> &pset, const ParticleSet<T_Weight> &wset);

template <class T_Particle=double>
inline ParticleSet<T_Particle> operator +(ParticleSet<T_Particle> pset, T_Particle p);


#define DSPONE_BASIC_PARTICLE_SET_TMP template <class T_Particle>
#define DSPONE_BASIC_PARTICLE_SET_DCL ParticleSet<T_Particle>

DSPONE_BASIC_PARTICLE_SET_TMP
DSPONE_BASIC_PARTICLE_SET_DCL::ParticleSet(boost::shared_array<T_Particle> set, int size, T_Particle min, T_Particle max) :
    IParticleSet<T_Particle> (size),
    _set(set),
    _randState(NULL),
    _range(std::make_pair(min, max))
{
    init();
}

DSPONE_BASIC_PARTICLE_SET_TMP
DSPONE_BASIC_PARTICLE_SET_DCL::ParticleSet(int size, T_Particle p, T_Particle min, T_Particle max) :
    IParticleSet<T_Particle> (size),
    _set(new T_Particle[size]),
    _randState(NULL),
    _range(std::make_pair(min, max))
{
    init();
    math::set(p, _set.get(), IParticleSet<T_Particle>::_size);
}

DSPONE_BASIC_PARTICLE_SET_TMP
DSPONE_BASIC_PARTICLE_SET_DCL::ParticleSet(const ParticleSet<T_Particle> &particleSet) :
    IParticleSet<T_Particle>(particleSet._size),
    _set(new T_Particle[particleSet._size]),
    _randState(NULL),
    _range(particleSet._range)
{
    math::copy(particleSet._set.get(), _set.get(), particleSet._size);
}

DSPONE_BASIC_PARTICLE_SET_TMP
void DSPONE_BASIC_PARTICLE_SET_DCL::init()
{
    _randomValues.reset(new double[IParticleSet<T_Particle>::_size]);
    _randomParticles.reset(new T_Particle[IParticleSet<T_Particle>::_size]);
    if (!_randState)  // for the assignment operator
    {
	math::init_rand_state(&_randState, _randMean, _randStdDev);
    }
}

DSPONE_BASIC_PARTICLE_SET_TMP
DSPONE_BASIC_PARTICLE_SET_DCL::~ParticleSet()
{
    if (_randState)
    {
	math::delete_rand_state(&_randState);
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
    math::minmax(_set.get(), IParticleSet<T_Particle>::_size, &min, &max);
    math::subC(min, _set.get(), IParticleSet<T_Particle>::_size);
    math::divC(max, _set.get(), IParticleSet<T_Particle>::_size);
}

DSPONE_BASIC_PARTICLE_SET_TMP
T_Particle DSPONE_BASIC_PARTICLE_SET_DCL::sum() const
{
    T_Particle value;
    math::sum(_set.get(), IParticleSet<T_Particle>::_size, &value);
    return value;
}

DSPONE_BASIC_PARTICLE_SET_TMP
void DSPONE_BASIC_PARTICLE_SET_DCL::addNoise(T_Particle mean, T_Particle std)
{
    math::rand(_randState, _randomValues.get(), IParticleSet<T_Particle>::_size);
    math::multC(std, _randomValues.get(), IParticleSet<T_Particle>::_size);
    math::addC(mean, _randomValues.get(), IParticleSet<T_Particle>::_size);
    math::copy(_randomValues.get(), _randomParticles.get(), IParticleSet<T_Particle>::_size);
    math::add(_randomParticles.get(), _set.get(), IParticleSet<T_Particle>::_size);
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
    math::stddev(_set.get(), IParticleSet<T_Particle>::_size, &stddev);
    return stddev;
}

DSPONE_BASIC_PARTICLE_SET_TMP
T_Particle* DSPONE_BASIC_PARTICLE_SET_DCL::get() const
{
    return _set.get();
}

DSPONE_BASIC_PARTICLE_SET_TMP
ParticleSet<T_Particle>& DSPONE_BASIC_PARTICLE_SET_DCL::operator =(const ParticleSet<T_Particle> &rset)
{
    IParticleSet<T_Particle>::_size = rset._size;
    _set.reset(new T_Particle[IParticleSet<T_Particle>::_size]);
    _range.first  = rset._range.first;
    _range.second = rset._range.second;
    math::copy(rset._set.get(), _set.get(), rset.size());
    init();
    return *this;
}

#undef DSPONE_BASIC_PARTICLE_SET_TMP
#undef DSPONE_BASIC_PARTICLE_SET_DCL

/* --------------------------------------------------------------*/
/* -- Operators implementation between T_Particle and T_Weight --*/
/* --------------------------------------------------------------*/

template <class T_Particle, class T_Weight>
inline ParticleSet<T_Particle> operator *(const ParticleSet<T_Particle> &pset, const ParticleSet<T_Weight> &wset)
{
    ParticleSet<T_Particle> ret(pset.size());
    if (pset.size() >= wset.size())
	math::mult(wset.get(), pset.get(), ret.get(), wset.size());
    return ret;
}

template <class T_Particle>
inline ParticleSet<T_Particle> operator +(ParticleSet<T_Particle> pset, T_Particle p)
{
    int size = pset.size();
    ParticleSet<T_Particle> ret(size);
    math::addC(p, pset.get(), ret.get(), size);
    return ret;
}

}



#endif // __PARTICLEFILTERIMPL_H_
