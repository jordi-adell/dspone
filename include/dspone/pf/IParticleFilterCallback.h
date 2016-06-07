/*
* IParticleFilterCallback.h
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
#ifndef __IPARTICLE_FILER_CALLBACK_H_
#define __IPARTICLE_FILER_CALLBACK_H_


namespace dsp {

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

}

#endif // __IPARTICLEFILTER_H_
