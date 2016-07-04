#include <dspone/rt/ShortTimeAnalysis.h>
#include <dspone/rt/ShortTimeAnalysisImpl.hpp>


// -------------------------------------------------------------------------------
// -- ShortTimeAnalysis ----------------------------------------------------------
namespace dsp
{

ShortTimeAnalysis::ShortTimeAnalysis(int windowSize, int analysisLength, int nchannels) :
  ShortTimeProcess(windowSize, analysisLength, nchannels, ANALYSIS)
{
  _impl.reset(new ShortTimeAnalysisImpl(this, windowSize, analysisLength, nchannels));
}



ShortTimeAnalysis::~ShortTimeAnalysis()
{
  _impl.release();
}


// Is empty beacuse resynthesis is not performed in this class.
void ShortTimeAnalysis::frameSynthesis(double *, double *, int, int, int)
{

}

int ShortTimeAnalysis::process(const std::vector<double *> &signal, unsigned int buffersize)
{
  _impl->process(signal, buffersize);
}


int ShortTimeAnalysis::process(const std::vector<float *> &signal, unsigned int buffersize)
{
  _impl->process(signal, buffersize);
}


int ShortTimeAnalysis::process(const std::vector<int16_t *> &signal, unsigned int buffersize)
{
  _impl->process(signal, buffersize);
}


int ShortTimeAnalysis::process(const std::vector<uint16_t *> &signal, unsigned int buffersize)
{
  _impl->process(signal, buffersize);
}

int ShortTimeAnalysis::process(const std::vector<std::shared_ptr<double> > &signal, unsigned int buffersize)
{
  _impl->process(signal, buffersize);
}



int ShortTimeAnalysis::process(const std::vector<std::shared_ptr<float> > &signal, unsigned int buffersize)
{
  _impl->process(signal, buffersize);
}



int ShortTimeAnalysis::process(const std::vector<std::shared_ptr<int16_t> > &signal, unsigned int buffersize)
{
  _impl->process(signal, buffersize);
}

int ShortTimeAnalysis::process(const std::vector<std::shared_ptr<uint16_t> > &signal, unsigned int buffersize)
{
  _impl->process(signal, buffersize);
}


int ShortTimeAnalysis::getLatency() const
{
  _impl->getLatency();
}

int ShortTimeAnalysis::getMaxLatency() const
{
  _impl->getMaxLatency();
}

int ShortTimeAnalysis::getBufferSize() const
{
  _impl->getFrameSize();
}

int ShortTimeAnalysis::getNumberOfChannels() const
{
  _impl->getNumberOfChannels();
}


}
