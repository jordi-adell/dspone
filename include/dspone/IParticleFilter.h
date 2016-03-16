/*
* IParticleFilter.h
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
#ifndef __IPARTICLEFILTER_H_
#define __IPARTICLEFILTER_H_


#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <vector>
#include <ostream>

namespace dsp {

  /* -- Interfaces for particle and weights ------- */
  /**
   * @class IParticleSet
   * @brief This interface represents a particle set.
   * This is a group of particles that can be modified.
   * You also need to provide a way to add noise to it.
   * No special restrictions are set for the type of probability
   * distribution of the additive noise.
   */

  template <class T_Particle>
    class IParticleSet
    {
    public:
    IParticleSet(int size) : _size(size) {}
      virtual ~IParticleSet() {}

      /**
       * @brief set  sets the value of a particle for a specific index
       * @param p   value to be set
       * @param pos  index of the particle to be set.
       * @return   the position at which the particle has been actually set.
       */
      virtual int set(T_Particle p, unsigned int pos) = 0;
      /**
       * @brief at  returns the value of a particle in a specifcic position
       * @param pos  position of the particle we want to know the value for
       * @return   the value of the requested particle
       */
      virtual const T_Particle &at(unsigned int pos) const = 0;

      /**
       * @brief size  number of particles in the set
       * @return   the number of particles in the set.
       */
      size_t size() const {return _size;}
      /**
       * @brief empty  Returns whether the set is empty or not.
       * @return true when there are no particles in the set.
       */
      virtual bool empty() const { return (size() == 0);}
      /**
       * @brief normalise Normalises the set of particles
       * in a way that the sum of all of them is one.
       * Is is mainly necessary for set of weights.
       * @return  return the summ of all the particles.
       *
       */
      virtual void normalise() = 0;
      /**
       * @brief normalise Normalises the set of particles
       * in a way that the sum of all of them is one.
       * Is is mainly necessary for set of weights.
       * @return  return the summ of all the particles.
       *
       */
      virtual T_Particle sum() const = 0;
      /**
       * @brief addNoise  the function is called when some noise needs to be added
       * to the particle set. The mean and standard deviation are given, but no restrictions
       * are specified for the shape of the probability distribution.
       * @param mean  mean of the distribution
       * @param std   standard deviation of the distribution
       */
      virtual void addNoise(T_Particle mean, T_Particle std) = 0;

      /**
       * @brief getRandomValue
       * This function returns a random particle within the valid range of the
       * set.
       * @return  Random value for a partcile.
       */
      virtual T_Particle getRandomParticle() const = 0;

      /**
       * @brief getStandardDeviation
       * @return  The standard deviatino of the set.
       */
      virtual T_Particle getStandardDeviation() const = 0;


    protected:
      int _size;  /**< number of elements in the set */
    };


  /************ Interfaces for models: Obersvation, Prediction and Resmapling **********************/


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
    IObservationModel(){}
    virtual ~IObservationModel() {}
    /**
     * @brief getWeights  calcualte the weight that has to be
     * applied to each particle in the given set.
     * @param particle  the particle set to calcualte weights for.
     * @return  a set of weights
     */
    virtual T_WeightSet getWeights(T_ParticleSet &particle) = 0;
    /**
     * @brief updateModel
     * If the observation model need to get new observation to recalculate
     * some parameters of the model, you have to by this code here.
     * This function will be called at each iteraction of the particle
     * filter.
     */
    virtual void updateModel() = 0; // by Getting observations
    };



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
    IResamplingModel(){}
    virtual ~IResamplingModel(){}
    /**
     * @brief resample   function that resamples the currentSet and generates
     * a new resmapledSet with the most relevant particles in the previous set.
     * @param currentSet  current set of particles
     * @param resampledSet   new set with the chosen particles.
     * @param weights   weights assigned by the observation model to each particle in the current set.
     */
    virtual void resample(const T_ParticleSet &currentSet,T_ParticleSet  &resampledSet, const T_WeightSet &weights) = 0;
    };



  template <class T_Particle>
    /**
     * @brief The IParticleFilterCallback class is an interface to
     * implement a callback for the particle filter. If a callback is passed
     * to the particle filter it will be called with the solution found
     * at each iteration of the particle filter.
     */
    class IParticleFilterCallback
    {
    public:
      IParticleFilterCallback(){}
      virtual ~IParticleFilterCallback(){}
      /**
       * @brief setResult   Function that will be called periodically
       * by the particle filter.
       * @param p
       */
      void setResult(T_Particle p) = 0;
    };


  template<class T_Particle>
    /**
     * @brief operator <<
     * Used to write the value of particle sets.
     * It will only work for T_Particle types for which the
     * << oprator is defined.
     * @param os   stream to write the value of the particle into.
     * @param set  the set of particles to write.
     * @return  returns the ostream used.
     */
    std::ostream&  operator<<(std::ostream &os, const IParticleSet<T_Particle> &set)
    {
      size_t size = set.size();
      if (size > 0)
	os << std::string("[") << set.at(0);
      for (size_t i = 0; i < set.size(); ++i)
        {
          os << std::string(", ") << set.at(i);
        }
      os << std::string("]");
      return os;
    }


}

#endif // __IPARTICLEFILTER_H_
