/*
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
#ifndef __DUMMY_STFT_H_
#define __DUMMY_STFT_H_

#include <dspone/rt/ShortTimeFourierTransform.h>
#include <dspone/rt/ShortTimeFourierAnalysis.h>

namespace dsp {


/**
	 * @brief The DummySTFT class
	 * A SFTF algorithm that does nothing, simply does STFT and its invers,
	 * the resulting signal has to be equal to the input one.
	 */
class DummySTFT : public STFT
{
    public:
	DummySTFT();
	~DummySTFT();
    private:
	void processParametrisation(std::vector<double *> &analysisFrames, int analysisLength,
				    std::vector<double *> &dataChannels, int dataLength);
};


class DummySTFTAnalysis : public STFTAnalysis
{
    public:
	DummySTFTAnalysis();
	~DummySTFTAnalysis();
    private:
	void frameAnalysis();
	void processParametrisation(std::vector<double *> &analysisFrames, int analysisLength,
				    std::vector<double *> &dataChannels, int dataLength);
};


}


#endif // __STFT_H_
