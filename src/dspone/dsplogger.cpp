#include <dspone/dsploggerImpl.h>
#include <boost/filesystem.hpp>


namespace  dsp {

std::unique_ptr<Logger> Logger::_logger;

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


void Logger::setLevel(LogLevel level)
{
  _level = level;
}

void Logger::setLevel(std::string level)
{
  auto it = std::find(_levelNames.begin(), _levelNames.end(), level);
  if (it != _levelNames.end())
  {
    setLevel(static_cast<LogLevel>(it - _levelNames.begin()));
  }
}

bool Logger::isEnabled(LogLevel level) { return (_enabled && level <= _level); }
void Logger::enable() { _enabled = true;}
void Logger::disable() { _enabled = false; }

Logger &Logger::logger(LogLevel level, bool enable, std::ostream &os)
{
  if (!_logger)
  {
    _logger.reset(new Logger(level, enable, os));
  }
  return *(_logger.get());
}

}

