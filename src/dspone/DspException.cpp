#include <dspone/DspException.h>

namespace dsp
{

DspException::DspException(const std::string &msg) :
std::runtime_error(msg)
{

}


}
