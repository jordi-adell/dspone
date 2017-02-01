#include <dspone/dsploggerImpl.h>
#include <boost/filesystem.hpp>


namespace  dsp {

	Logger::Logger(LogLevel level, bool enable, std::ostream &os) : _level(level), _os(os), _enabled(enable)
	{
	  //	  \x1b[32m
	    _levelNames = {"FATAL", "ERROR", "WARN ", "INFO ", "DEBUG", "TRACE"};
	}

	std::ostream &Logger::log(LogLevel level, const char *file, int line)
	{
	    std::string fname = boost::filesystem::basename(file);
	    if (_enabled && level <= _level)
	    {
		_os << "[" << _levelNames.at(level) << "] " <<  "[" << fname << ":" << line <<  "] ";
		return _os;
	    }
	}

	bool Logger::isEnabled(LogLevel level) { return (_enabled && level <= _level); }
	void Logger::enable() { _enabled = true;}
	void Logger::disable() { _enabled = false; }

}

