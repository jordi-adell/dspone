/*
* ShortTimeProcess.h
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
#ifndef __SHORTTIMEPROCESS_H_
#define __SHORTTIMEPROCESS_H_


#include <dspone/rt/SignalProcessor.h>

#include <boost/shared_array.hpp>

#include <memory>
#include <vector>

namespace dsp {

class ShortTimeProcessImpl;
class DspPlot;

class ShortTimeProcess : public SignalProcessor
{

    public:
	/**
	   * @brief Returns the latency of the algorithm. This the number of samples that are sotored
	   * for further processing the sample that will be supplied in the next call.
	   * @return
	   */
	int getLatency() const;
	int getMinLatency() const;
	int getMaxLatency() const;
	int getBufferSize() const;

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

	/**
	 * @brief getAnalysisLength
	 * Returns the length of the analysis vector. In some cases can be quite different from the
	 * frame length.
	 * @return
	 */
	int getAnalysisLength() const;
	void setAnalysisLength();

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


	/** @brief this type is used to set whether the short time process
	   * has to perform analysis only or need to reconstruct the signal too
	   * (analysis and synthesis)
	   **/
	typedef enum {ANALYSIS, ANALYSIS_SYNTHESIS} Mode;

	ShortTimeProcess(int windowSize,  int analisysLength = 0, int nchannels = 1, Mode mode = ANALYSIS_SYNTHESIS);
	virtual ~ShortTimeProcess();
	//        ShortTimeProcess(int windowSize,  int analisysLength, int nchannels, int ndatachannels, Mode mode = ANALYSIS_SYNTHESIS);

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

	void unwindowFrame(double *frame, size_t length) const;
	void unwindowFrame(double *frame, double *unwindowed, size_t length) const;

	/** Process a full buffer, keeps the last _windowShift samples from the buffer provided in the first call
	    to be used in the following computation. If is the last call, and no more samples have to be processed. Then
	    you need to call the getRamainingSpeech function
	    @param cleanSignal  buffer containing the already cleanned signal
	    @param noisySignal  buffer containing the signal plus the noise (singal before cleanning it)
	    @param output  buffer where the processed buffer will be stored.
	    @param buffer  length of the buffer
	    @return returns the number of samples writen to the output buffer
	    */
	int process(const std::vector<double *> &signal,
		    unsigned int inbuffersize,
		    const std::vector<double *> &output,
		    unsigned int outbuffersize);
	int process(const std::vector<boost::shared_array<double> > &signal,
		    unsigned int inbuffersize,
		    const std::vector<boost::shared_array<double> > &output,
		    unsigned int outbuffersize);

	int process(const std::vector<float *> &signal,
		    unsigned int inbuffersize,
		    const std::vector<float *> &output,
		    unsigned int outbuffersize);
	int process(const std::vector<boost::shared_array<float> > &signal,
		    unsigned int inbuffersize,
		    const std::vector<boost::shared_array<float> > &output,
		    unsigned int outbuffersize);

	int process(const std::vector<int16_t *> &signal,
		    unsigned int inbuffersize,
		    const std::vector<int16_t *> &output,
		    unsigned int outbuffersize);
	int process(const std::vector<boost::shared_array<int16_t> > &signal,
		    unsigned int inbuffersize,
		    const std::vector<boost::shared_array<int16_t> > &output,
		    unsigned int outbuffersize);

	int process(const std::vector<uint16_t *> &signal,
		    unsigned int inbuffersize,
		    const std::vector<uint16_t *> &output,
		    unsigned int outbuffersize);
	int process(const std::vector<boost::shared_array<uint16_t> > &signal,
		    unsigned int inbuffersize,
		    const std::vector<boost::shared_array<uint16_t> > &output,
		    unsigned int outbuffersize);


    private:
	friend class ShortTimeProcessImpl;
	friend class DspPlot;
	std::unique_ptr<ShortTimeProcessImpl> _impl;

	/** Processes on signal frame. Calculates |FFT|^2 of both signals. And generates a binary mask,
	    following the criteria:
	    0 if |FFT(cleanFrame)|^2 < |FFT(noiseFrame)|^2, 1 otherwise.
	    Then the power spectra of cleanFrame is multiplied by the mask, its invers FFT is obtained and
	    stored on cleanFrame
	    @param cleanFrame  signal to apply the binary mask to.
	    @param noiseFrame  signal contained singla + noise.
	   **/
	virtual void frameAnalysis (double *inFrame,  double *analysis, int frameLength, int analysisLength, int channel) = 0;
	virtual void processParametrisation(std::vector<double*> &analysisFrames, int analysisLength,
					    std::vector<double*> &dataChannels, int dataLength) = 0;

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
	virtual void frameSynthesis(double *outFrame, double *analysis, int frameLength, int analysisLength, int channel) = 0;


};



}



#endif // __SHORTTIMEProcess_H_
