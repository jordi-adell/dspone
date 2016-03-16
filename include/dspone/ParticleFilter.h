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
#ifndef PARTICLEFILTER_H_
#define PARTICLEFILTER_H_

#include <dspone/IParticleFilter.h>
#include <dspone/ParticleFilterImpl.h>
#include <boost/static_assert.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits.hpp>

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
      template<class T_Particle=double,
               class T_ID = int,
               class T_ParticleSet=BasicParticleSet<T_Particle>,
               class T_Weight=T_Particle,
               class T_WeightSet=BasicParticleSet<T_Weight> >
      class ParticleFilter
      {
      private:

        /***** Interfaces renaming to clear the code. *****/

        /**
           * @brief ObservationModel
           * Type definition for the ObservationModel used in the construction of the particle filter.
           */
        typedef IObservationModel<T_ParticleSet, T_WeightSet> ObservationModel;
        /**
           * @brief PredictionModel
           * Type definition for the definition used in the construction of the particle filter.
           */
        typedef IPredictionModel<T_Particle, T_ParticleSet > PredictionModel;
        /**
           * @brief ResamplingModel
           * Type definition for the ResamplingModel used in the construction of the particle filter.
           */
        typedef IResamplingModel<T_ParticleSet> ResamplingModel;


        /***** Default Implementations renamed to clear the code *****/

        /**
           * @brief DefaultPredictionModelImpl
           * A type definition for the default prediction model
           */
        typedef PredictionModelImpl<T_Particle, T_ParticleSet> DefaultPredictionModelImpl;
        /**
           * @brief DefaultResamplingModelImpl
           * A type definition for the default resampling model
           */
        typedef ResamplingModelImpl<T_Particle, T_ParticleSet> DefaultResamplingModelImpl;

      public:
        /**
           * @brief ParticleFilter Constructor with the initial state of the particle
           * filter, the amount of particles to be used and the models
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
                       boost::shared_ptr<ObservationModel> obsModel,
                       boost::shared_ptr<PredictionModel> predModel,
                       boost::shared_ptr<ResamplingModel> resModel);

        ParticleFilter(T_Particle initialState,
                       int numParticles,
                       T_ID id,
                       std::pair<T_Particle, T_Particle> range,
                       boost::shared_ptr<ObservationModel> obsModel,
                       boost::shared_ptr<PredictionModel> predModel);

        ParticleFilter(T_Particle initialState,
                       int numParticles,
                       T_ID id,
                       std::pair<T_Particle, T_Particle> range,
                       boost::shared_ptr<ObservationModel> obsModel);

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
        static   ParticleFilter<T_Particle, T_ID, T_ParticleSet, T_Weight, T_WeightSet>
        make_particle_filter(T_Particle initialState, int numParticles, T_ID id,
                             std::pair<T_Particle, T_Particle> range,
                             boost::shared_ptr<ObservationModel> obsModel,
                             boost::shared_ptr<PredictionModel> predModel,
                             boost::shared_ptr<ResamplingModel> resModel);


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
        boost::shared_ptr<ObservationModel> _observationModel; /**< ptr to the Observation model **/
        boost::shared_ptr<PredictionModel> _predictionModel; /**< ptr to the prediction model **/
        boost::shared_ptr<ResamplingModel> _resamplingModel; /**< ptr to the resampling model **/
        // Default implementation
        boost::shared_ptr<DefaultPredictionModelImpl> _predictionModelImpl; /**< default predictiom model (used if non given) **/
        boost::shared_ptr<DefaultResamplingModelImpl> _resamplingModelImpl; /**< default resampling model (used if non given) **/

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


    }


#include <dspone/ParticleFilter.inl>

#endif // PARTICLEFILTER_H_
