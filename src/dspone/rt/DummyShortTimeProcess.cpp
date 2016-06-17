#include <dspone/rt/DummyShortTimeProcess.h>

namespace dsp
{

DummyShortTimeProcess::DummyShortTimeProcess(int windowSize, bool generateDataChannel) :
    ShortTimeProcess(windowSize),
    _generateDataChannel(generateDataChannel)
{

}


DummyShortTimeProcess::~DummyShortTimeProcess()
{

}

void DummyShortTimeProcess::frameAnalysis (BaseType *inFrame,  BaseType *analysis, int frameLength, int analysisLength, int)
{
    int length = (frameLength < analysisLength) ? frameLength : analysisLength;
    wipp::copyBuffer(inFrame, analysis, length);
}


void DummyShortTimeProcess::processParametrisation(std::vector<double *> &analysisFrames, int analysisLength,
						   std::vector<double *> &dataChannels, int dataLength)
{
    if (_generateDataChannel)
    {
	for (size_t c = 0; c < dataChannels.size(); ++c)
	{
	    wipp::set(_constant, dataChannels[c], dataLength);
	}
    }
}

void DummyShortTimeProcess::frameSynthesis(BaseType *outFrame, BaseType *analysis, int frameLength, int analysisLength, int )
{
    int length = (frameLength < analysisLength) ? frameLength : analysisLength;
    wipp::copyBuffer(analysis, outFrame, length);
}



}

