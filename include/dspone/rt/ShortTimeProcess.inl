#include <dspone/rt/ShortTimeProcess.h>

#include <sstream>

namespace dsp {


template <typename SampleType>
int ShortTimeProcess::process(const std::vector<SampleType *> &signal,
			      unsigned int inbuffersize,
			      const std::vector<SampleType *> &output,
			      unsigned int outbuffersize)
{
    int datachannels = std::max(signal.size(), output.size()) - _nchannels;
    if (datachannels > _nDataChannels)
    {
	std::ostringstream oss;
	oss << "You passed me a number of data channels larger than possible!";
	oss << "I can only process " << _nchannels << " signal channels";
	oss << " and a maximum of " << _nDataChannels << " data channels";
	throw(DspException(oss.str()));
    }
    return overlapAndAdd(signal, inbuffersize, output, outbuffersize);
}


template <typename SampleType>
int ShortTimeProcess::process(const std::vector<boost::shared_array<SampleType> > &signal, unsigned int inbuffersize,
			      const std::vector<boost::shared_array<SampleType> > &output, unsigned int outbuffersize)
{
    unsigned int signalSize = signal.size();
    unsigned int outputSize = output.size();
    std::vector<SampleType*> vsignal;
    std::vector<SampleType*> voutput;

    if (outputSize < _nchannels || signalSize < _nchannels)
    {
	throw(DspException("Wrong i/o number of channels"));
    }

    for (unsigned int i = 0; i <signalSize; ++i)
    {
	vsignal.push_back(signal[i].get());
    }
    for (unsigned int i = 0; i <outputSize; ++i)
    {
	voutput.push_back(output[i].get());
    }
    return process(vsignal,inbuffersize, voutput, outbuffersize);
}

template<typename SampleType>
int ShortTimeAnalysis::process(const std::vector<boost::shared_array<SampleType> > &signal,
			       unsigned int buffersize)
{
    unsigned int signalSize = signal.size();
    std::vector<SampleType*> auxSignal;

    if (signalSize < _nchannels)
    {
	throw(DspException("Wrong input number of channels"));
    }
    for (unsigned int i = 0; i <signalSize; ++i)
	auxSignal.push_back(signal[i].get());
    return process(auxSignal, buffersize);
}

template<typename SampleType>
int ShortTimeAnalysis::process(const std::vector<SampleType *> &signal, unsigned int buffersize)
{
    std::vector<SampleType*> output;
    return ShortTimeProcess::process(signal, buffersize, output, 0);
}

/**
       * @brief The OverlapAndAdd function
       * This function is an implementation of the Overlap and Add
       * algorithm for the short time processing of audio application.
       * Basically windowing frames and then regenerates the signal
       * by adding in overlap consecutive frames.
       */
template<typename sampleType>
int ShortTimeProcess::overlapAndAdd(const std::vector<sampleType *> &signal, unsigned int inbuffersize,
				    const std::vector<sampleType *> &output, unsigned int outbuffersize)
{
    int latencycount;
    size_t occupancy;
    //    = _latencyBufferSignal[0].getCountUsed();
    wipp::cf_occupancy(_latencyBufferSignal[0], &occupancy);
    latencycount = occupancy;
    int pendingSamples   = inbuffersize + latencycount;
    unsigned int samplesLeftOver  = pendingSamples % _windowSize;
    int samplesToProcess = pendingSamples - samplesLeftOver;
    int fullNChannels = std::max(signal.size(), output.size());

    if (samplesLeftOver >= _windowSize)
    {
	samplesLeftOver = _windowSize;
	ERROR_STREAM("I will be losing some smaples, because the latency buffer is filled up");
    }

    if (samplesToProcess <= 0)
    {
	for (size_t i=0; i<signal.size(); ++i)
	{
	    size_t stored;
	    BaseType aux[inbuffersize];
	    wipp::copyBuffer(signal.at(i), aux, inbuffersize);
	    wipp::cf_write(_latencyBufferSignal[i], aux, inbuffersize, &stored);
	    //    _latencyBufferSignal.at(i).write(aux, inbuffersize);
	}
	return 0;
    }

    SignalVector basetSignal;
    // HR stands for high resolution: (float, 64 bits) / sample
    // However, oputput is: (signed integer, 16bits) / sample
    SignalVector outHR;
    size_t stored;

    for (size_t i=0;
	 i < fullNChannels;
	 ++i)
    {
	// Allocate memory for 64bits signals.
	basetSignal.push_back(SignalPtr(new BaseType[samplesToProcess]));
	wipp::setZeros(basetSignal.back().get(), samplesToProcess);
	outHR.push_back(SignalPtr(new BaseType[samplesToProcess]));
	wipp::setZeros(outHR.back().get(), samplesToProcess);

	// Put stored signal from previous iterations (latency)
	// to 64bits vectors.
	//	_latencyBufferSignal[i].read(basetSignal[i].get(),std::min(latencycount, samplesToProcess));
	wipp::cf_read(_latencyBufferSignal[i], basetSignal[i].get(),std::min(latencycount, samplesToProcess), &stored);

	// Put the current signal to process to the 64bits vector.

	if (i < signal.size())
	    wipp::copyBuffer(signal[i],&(basetSignal[i][latencycount]), inbuffersize - samplesLeftOver);

	// Copy the las window Shift to be processed to the latency buffer, so that
	// it can be used in next iteration.
	//	_latencyBufferSignal[i].write(&(basetSignal[i][samplesToProcess-_windowShift]), _windowShift);
	wipp::cf_write(_latencyBufferSignal[i], &(basetSignal[i][samplesToProcess-_windowShift]), _windowShift, &stored);

	// Copy the samples that can not be processed now, so that they are processed
	// in the next iteration.
	int samplesLeftOverFromInputBuffer = std::min(inbuffersize, samplesLeftOver);
	BaseType aux[samplesLeftOverFromInputBuffer];
	if (i < signal.size())
	    wipp::copyBuffer(&(signal[i][inbuffersize - samplesLeftOverFromInputBuffer]),
		    aux, samplesLeftOverFromInputBuffer);
//	_latencyBufferSignal[i].write(aux, samplesLeftOverFromInputBuffer);
	wipp::cf_write(_latencyBufferSignal[i], aux, samplesLeftOverFromInputBuffer, &stored);

    }

    ShortTimeProcessing(basetSignal, outHR, samplesToProcess);

    if (!_doSynthesis)
	return 0;

    int outLength = samplesToProcess - _windowShift;
    for (size_t i=0; i < output.size(); ++i)
    {
	BaseType *currentLBP = _latencyBufferProcessed[i].get();
	BaseType *currentOutHR = outHR[i].get();
	wipp::add(currentLBP, currentOutHR, _windowShift);

	// Stores half of the last window shift for the next call
	wipp::copyBuffer(&currentOutHR[samplesToProcess-_windowShift],  currentLBP, _windowShift);

	if (_firstCall)
	{
	    if (outLength >= static_cast<int>(_windowShift))
		wipp::mult(_iwindow.get(), currentOutHR, _windowShift);
	}

	wipp::copyBuffer(currentOutHR, output[i], outLength);
    }

    _firstCall = false;
    if (outLength > static_cast<int>(outbuffersize))
    {
	ERROR_STREAM("The supplied buffersize "<< outbuffersize << " is not large enough: " << outLength <<". Some samples will be lost");
	outLength = outbuffersize;
    }
    return outLength;
}


template <typename sampleType>
int ShortTimeProcess::getRemainingSpeech(const std::vector<sampleType *> &buffer, unsigned int buffersize)
{

    if (buffersize >= _windowShift)
	buffersize = _windowShift-1;
    for (size_t i=0; i<buffer.size(); ++i)
    {
	wipp::mult(&_iwindow[_windowShift], _latencyBufferProcessed[i].get(),_windowShift-1);
	wipp::mult(&_iwindow[_windowShift], _latencyBufferProcessed[i].get(),_windowShift-1);
	wipp::copyBuffer(_latencyBufferProcessed[i].get(), buffer[i], buffersize);
    }
    _firstCall = true;
    return buffersize;
}


template<typename SampleType>
int ShortTimeProcess::getRemainingSpeech(const std::vector<boost::shared_array<SampleType> > &buffer, unsigned int buffersize)
{
    std::vector<SampleType*> vsignal;
    if (buffer.size() < _nchannels)
	throw(DspException("Wrong i/o number of channels"));

    for (unsigned int i = 0; i < buffer.size() && i < vsignal.size(); ++i)
	vsignal.push_back(buffer[i].get());
    return getRemainingSpeech(vsignal, buffersize);
}

}

