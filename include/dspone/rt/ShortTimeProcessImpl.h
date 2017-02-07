/*
* ShortTimeProcessImpl.h
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
#ifndef __SHORTTIMEPROCESSIMPL_H_
#define __SHORTTIMEPROCESSIMPL_H_


#include <dspone/rt/ShortTimeProcess.h>
#include <dspone/DspException.h>
#include <dspone/dspdefs.h>
#include <dspone/dsplogger.h>

#include <wipp/wipputils.h>
#include <wipp/wippdefs.h>
#include <wipp/wippsignal.h>
#include <wipp/wippfilter.h>

#include <vector>
#include <fstream>
#include <sstream>


namespace dsp {

class ShortTimeProcessImpl
{

    protected:

	typedef ShortTimeProcess::Mode Mode;

	//@TODO set it at run time
	static const int _maximumLatencyBufferSize = 3*96000; // 3 seconds at 96kHz

	// Default definitions
	const unsigned int _windowSize;
	const unsigned int _windowShift;
	const unsigned int _halfWindowSize;
	const unsigned int _nchannels;
	const unsigned int _nDataChannels;
	const int _analysisLength;
	// window
	const SignalPtr _window;
	const SignalPtr _iwindow;
	SignalVector _analysisFrames;
	std::vector<BaseType*> _analysisFramesPtr; /**< raw pointer to be passed to parocessParameterisation */
	SignalVector _dataFrames;
	std::vector<BaseType*> _dataFramesPtr;

    private:

	const Mode _mode; /**< This variable indicated where we are in analysis only or analysis and synthesis mode */
	const bool _doSynthesis; /**< When in analysis only mode this variable is set to false in construction */

	// Buffers for frame calculations (ShortTimeProcess)
	//        SignalPtr _signal;
	//        SignalPtr _CCS;

	// Buffers for framing
	SignalPtr _frame;

	// Buffers for continuity between processing steps
	//          SignalVector _latencyBufferSignal;
	//          unsigned int _latencyBufferOcupancy;

	//	std::vector<container::CircularBuffer<BaseType, _maximumLatencyBufferSize>  >_latencyBufferSignal;
	std::vector<wipp::wipp_circular_buffer_t *> _latencyBufferSignal;
	SignalVector _latencyBufferProcessed;

	bool _firstCall;

	/** This function process speech frame by frame. Each frame is extracted every _widnowShift and
	    is of _windowSize length. Each frame is windowed with a Hanning window, process by the
	    processFrame function and overlaped and added again to reconstruct de processed signal
	    @param signal  Signal to apply the binary mask
	    @param noise   Noise already extracted from the signal.
	  **/
	void ShortTimeProcessing(const SignalVector &signal, const SignalVector &output, int tlength);

	/**
	   * @brief The OverlapAndAdd function
	   * This function is an implementation of the Overlap and Add
	   * algorithm for the short time processing of audio application.
	   * Basically windowsing frmaes and then regenerates the signal
	   * by adding in overlap consecutive frames.
	     */
	template<typename sampleType>
	int overlapAndAdd(const std::vector<sampleType *> &signal, unsigned int inbuffersize, const std::vector<sampleType *> &output, unsigned int outbuffersize);

	/** Processes on signal frame. Calculates |FFT|^2 of both signals. And generates a binary mask,
	    following the criteria:
	    0 if |FFT(cleanFrame)|^2 < |FFT(noiseFrame)|^2, 1 otherwise.
	    Then the power spectra of cleanFrame is multiplied by the mask, its invers FFT is obtained and
	    stored on cleanFrame
	    @param cleanFrame  signal to apply the binary mask to.
	    @param noiseFrame  signal contained singla + noise.
	   **/
	void frameAnalysis (BaseType *inFrame,  BaseType *analysis, int frameLength, int analysisLength, int channel);
	void processParametrisation();

	/**
	   * @brief In this function you need to implement the code necessary to reconstruct the signal
	   * from the information in the analysis buffers.
	   * @param outFrame  output frame that will be used to reconstruct the signal. Windowing is considered already
	   * in this class you do not need to care about it.
	   * @param analysis  array where all the analysis parameters are stored. You might use them
	   * to reconstruct the signal
	   * @param frameLength  length of the signal that has to be reconstructed
	   * @param analysisLength  length of the analyasis
	   */
	void frameSynthesis(BaseType *outFrame, BaseType *analysis, int frameLength, int analysisLength, int channel);


	/** Sets all buffers to an initial value of 0.0 **/
	void initVariableMembers();
	void initBuffers();
	void initWindowBuffers();
	void allocateNDataChannels(int nDataChannels);


	/**
	   * @brief Static function to calculate the necessary order for
	   * the supplied samples Rate.
	   * Sometimes the FFT order depened on the frameRate as well as the original
	   * sample rate. This function is called in the constructor so the order of
	   * the FFT can be obtained before calling the constructor
	   * @param samplerate  sample rate
	   * @param frameRate  time between to frames (i.e., frame shift in seconds)
	   * @return returns the order of the FFT to accomplish as close as possible
	   * the given requirements.
	   */
	static int calculateOrderFromSampleRate(int samplerate, double frameRate);
	static int calculateWindowSizeFromSampleRate(int samplerate, double frameRate)
	{
	    return (1 << calculateOrderFromSampleRate(samplerate, frameRate));
	}


    public:
	ShortTimeProcessImpl(ShortTimeProcess *frameProcessor,
		int windowSize,
		int analisysLength,
		int nchannels = 1,
		Mode mode = ShortTimeProcess::ANALYSIS_SYNTHESIS);
	virtual ~ShortTimeProcessImpl();


	ShortTimeProcess *_frameProcessor;

	/** Process a full buffer, keeps the last _windowShift samples from the buffer provided in the first call
	    to be used in the following computation. If is the last call, and no more samples have to be processed. Then
	    you need to call the getRamainingSpeech function
	    @param cleanSignal  buffer containing the already cleanned signal
	    @param noisySignal  buffer containing the signal plus the noise (singal before cleanning it)
	    @param output  buffer where the processed buffer will be stored.
	    @param buffer  length of the buffer
	    @return returns the number of samples writen to the output buffer
	    */
	template <typename SampleType>
	int process(const std::vector<SampleType *> &signal,
		    unsigned int inbuffersize,
		    const std::vector<SampleType *> &output,
		    unsigned int outbuffersize);
	template <typename SampleType>
	int process(const std::vector<boost::shared_array<SampleType> > &signal,
		    unsigned int inbuffersize,
		    const std::vector<boost::shared_array<SampleType> > &output,
		    unsigned int outbuffersize);


	void clear();

	/**
	   * @brief Returns the latency of the algorithm. This the number of samples that are sotored
	   * for further processing the sample that will be supplied in the next call.
	   * @return
	   */
	int getLatency() const;
	int getMinLatency() const;
	int getMaxLatency() const;

	/**
	 * @brief getWindowSize
	 *  Returns the window size in samples
	 * @return
	 */
	int getWindowSize() const;

	/**
	 * @brief getFrameSize
	 *  Returns the frame size, usually window shift, in samples
	 * @return
	 */
	int getFrameSize() const;

	/**
	 * @brief getFrameRate
	 * Frame rate in samples
	 * @return   Frame rate period in samples.
	 */
	int getFrameRate() const;
	int getAnalysisLength() const;

	/**
	   * @brief The buffer supplied to the process function must have a length
	   * multiple of the internal frame size.
	   * @return the internal frame size.
	   */
	//        inline int getBufferSize() const {return _windowSize;}

	/**
	   * @brief getRate   Returns the frame period for the speech analysis.
	   * This is eventually the rate at which the audio is processed.
	   * @return  the time between analysis frames in seconds
	   */
	double getRate() const;

	/**
	 * @brief getNumberOfChannels
	 * @return  the number of channels able to process.
	 */
	virtual int getNumberOfChannels() const;

	/**
	 * @brief getNumberOfDataChannels
	 * @return the number of channels that do not contain signal.
	 * They are no processed, it maybe used to lables signals and pass
	 * this information from one module to the next one.
	 * User should define de meaning of the information stored in the
	 * data channels.
	 */
	virtual int getNumberOfDataChannels() const;

	/** Returns the number of samples that are kept internally and can be obtained by calling the getRemainingSpeech function **/
	int getAmountOfRemainingSamples();

	/** Returns the speech that has been kept internally for processing speech in further calls. If this functino is called,
	    next call to process will be considered a first call, thus some samples will be retain.
	    Template only accepts int16_t and floats.
	    @param buffer  The buffer to where signal has to be stored
	    @param buffersize  length of th ebuffer provided
	    **/
	template <typename sampleType>
	int getRemainingSpeech(const std::vector<sampleType *> &buffer, unsigned int buffersize);
	template <typename sampleType>
	int getRemainingSpeech(const std::vector<boost::shared_array<sampleType> > &buffer, unsigned int buffersize);

	void unwindowFrame(double *frame, size_t length) const;
	void unwindowFrame(double *frame, double *unwindow, size_t length) const;


};


}




#include "ShortTimeProcessImpl.inl"

#endif // __SHORTTIMEProcess_H_
