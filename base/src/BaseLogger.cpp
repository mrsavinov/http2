
#include "BaseLogger.hpp"
#include "ConsoleColors.hpp"
#include <cstring>
#include <cstdarg>
#include <errno.h>
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <algorithm>

namespace etu{
namespace base{
typedef std::unique_lock<std::mutex> ScopedLock;
static const char* _logLevelToTag(LOG_LEVEL level);

void addCurrTime(const std::string& input, std::string& output)
{
	time_t currTime = time(0);
	struct tm* timeinfo;
	timeinfo = localtime(&currTime);
	char buffer[50];
	strftime(buffer, 50, "%c", timeinfo);

	output = std::string(buffer) + " " + input;
}


LOG_LEVEL transform_log_level( int syslog_priority ) throw()
{
#if 0
#ifndef WINDOWS_SYSTEM
	switch( syslog_priority )
	{
		case LOG_EMERG:
			return FATAL_LEVEL;
		case LOG_ALERT:
			return ERROR_LEVEL;
		case LOG_CRIT:
			return ERROR_LEVEL;
		case LOG_ERR:
			return ERROR_LEVEL;
		case LOG_WARNING:
			return WARNING_LEVEL;
		case LOG_NOTICE:
			return NOTICE_LEVEL;
		case LOG_INFO:
			return INFO_LEVEL;
		case LOG_DEBUG:
			return DEBUG_LEVEL;
	}
#endif
#endif
	return DEBUG_LEVEL;
}

class ConsoleLog : public BaseLog
{
public:
    ConsoleLog(const char* libFilter, int logFilter)
    	: BaseLog(libFilter, logFilter, CONSOLE_LOG) {}

    virtual void post(LOG_LEVEL logLevel, const std::string& text)
    {
		std::string output;
		addCurrTime(text, output);

#ifdef _WIN32
    	printf("%s %s", logLevelToColorConsole( logLevel ), output.c_str());
#else
		if(isatty(STDOUT_FILENO)){
			std::cout << logLevelToColorConsole( logLevel ) << " " << output;
		} else {
			printf("%s %s",  _logLevelToTag( logLevel ), output.c_str());
		}
#endif
		fflush(stdout);
    }

	static const char* logLevelToColorConsole( LOG_LEVEL level )
	{
		switch ( level )
		{
			case TRACE_LEVEL:
				return  CONSOLE_COLOR_GREY "TRACE " CONSOLE_COLOR_OFF;
			case DEBUG_LEVEL:
				return  CONSOLE_COLOR_GREY "DEBUG " CONSOLE_COLOR_OFF;
			case INFO_LEVEL:
				return  CONSOLE_COLOR_GREEN "INFO  " CONSOLE_COLOR_OFF;
			case NOTICE_LEVEL:
				return  CONSOLE_COLOR_GREEN "NOTICE" CONSOLE_COLOR_OFF;
			case WARNING_LEVEL:
				return CONSOLE_COLOR_YELLOW "WARN  " CONSOLE_COLOR_OFF;
			case ERROR_LEVEL:
				return    CONSOLE_COLOR_RED "ERROR " CONSOLE_COLOR_OFF;
			case FATAL_LEVEL:
				return    CONSOLE_COLOR_RED "FATAL " CONSOLE_COLOR_OFF;
			default:
				return CONSOLE_COLOR_MAGENTA "??????" CONSOLE_COLOR_OFF;
		}
	}

	bool valid() { return true; } // always valid
};

class SysLog : public BaseLog
{
public:
    SysLog(const char* daemonName, int facility, const char* libFilter, int logFilter)
    	: BaseLog(libFilter, logFilter, SYS_LOG)
    {
		#ifndef _WIN32
			openlog(daemonName, LOG_PID, facility);
		#endif
    }

    static int getFacilityByName(const char* facility);
    virtual void post(LOG_LEVEL logLevel, const std::string& text);
	bool valid() { return true; } // i dunno howto validate syslog TBD
};

class FileLog : public BaseLog
{
public:
    FileLog(const char* libFilter, int logFilter, const std::string logName);
    ~FileLog();

    virtual void post(LOG_LEVEL logLevel, const std::string& text);
	bool valid() { return f_ != NULL; }
	void SIGHUP_event();

private:
    std::string logName_;
    FILE* f_;
    std::mutex fileMutex_;
};

uid_type createConsoleLog(const char* libFilter, int priority) {
	return Logger::instance().addConsoleLog(libFilter, priority);
}

uid_type createFileLog(const char* libFilter, int priority, const char* fileName) {
	return Logger::instance().addFileLog(libFilter, priority, fileName);
}

uid_type createSysLog(const char* daemonName, int facility, const char* libFilter, int priority) {
	return Logger::instance().addSysLog(daemonName, facility, libFilter, priority);
}

uid_type registerExternalLog(BaseLog * logger) {
	return Logger::instance().registerLogger(logger);
}

bool deleteLogger( uid_type loggerId ) { return Logger::instance().deleteLog( loggerId ); }
void deleteAllLogsByType(int type) { Logger::instance().deleteAllLogsByType(type); }

Logger* Logger::instance_ = NULL;

FileLog::FileLog(const char* libFilter, int logFilter, const std::string logName)
	: BaseLog(libFilter, logFilter, FILE_LOG)
	, logName_(logName)
	, f_(NULL)
{
	f_ = fopen(logName_.c_str(), "a");
	if (!f_)
	{
		fprintf(stderr, "Can't open file %s.\n", logName_.c_str());
	}
}

FileLog::~FileLog()
{
    if (f_)
    {
    	fclose(f_);
    }
}


void FileLog::SIGHUP_event()
{
	base::ScopedLock locker(fileMutex_);
   	fclose(f_);
   	f_ = fopen(logName_.c_str(), "a");
}


void FileLog::post(LOG_LEVEL logLevel, const std::string& text)
{
    if (!f_)
    {
    	return;
    }

	std::string output;
	addCurrTime(text, output);
	base::ScopedLock locker(fileMutex_);
    fprintf(f_, "%s%s", _logLevelToTag( logLevel ), output.c_str());
    fflush (f_);
}

void SysLog::post(LOG_LEVEL logLevel, const std::string& text)
{
#ifndef _WIN32
    int syslogLevel = logLevel;

    switch (logLevel)
    {
    case TRACE_LEVEL:
        syslogLevel = LOG_DEBUG;
        break;
    case DEBUG_LEVEL:
        syslogLevel = LOG_DEBUG;
        break;
    case INFO_LEVEL:
        syslogLevel = LOG_INFO;
        break;
    case NOTICE_LEVEL:
        syslogLevel = LOG_NOTICE;
        break;
    case WARNING_LEVEL:
        syslogLevel = LOG_WARNING;
        break;
    case ERROR_LEVEL:
        syslogLevel = LOG_ERR;
        break;
    case FATAL_LEVEL:
        syslogLevel = LOG_EMERG;
        break;
    }

	syslog(syslogLevel, "%s", text.c_str());
#endif // WIN32	
}

int SysLog::getFacilityByName(const char* facility)
{
#ifdef _WIN32
	return 0;
#else
    std::string _facility(facility);
    if(_facility == "LOG_DAEMON")         return LOG_DAEMON;
    else if (_facility == "LOG_AUTHPRIV") return LOG_AUTHPRIV;
    else if (_facility == "LOG_CRON")     return LOG_CRON;
    else if (_facility == "LOG_UUCP")     return LOG_UUCP;
    else if (_facility == "LOG_KERN")     return LOG_KERN;
    else if (_facility == "LOG_LOCAL0")   return LOG_LOCAL0;
    else if (_facility == "LOG_LOCAL1")   return LOG_LOCAL1;
    else if (_facility == "LOG_LOCAL2")   return LOG_LOCAL2;
    else if (_facility == "LOG_LOCAL3")   return LOG_LOCAL3;
    else if (_facility == "LOG_LOCAL4")   return LOG_LOCAL4;
    else if (_facility == "LOG_LOCAL5")   return LOG_LOCAL5;
    else if (_facility == "LOG_LOCAL6")   return LOG_LOCAL6;
    else if (_facility == "LOG_LOCAL7")   return LOG_LOCAL7;
    else if (_facility == "LOG_LPR")      return LOG_LPR;
    else if (_facility == "LOG_MAIL")     return LOG_MAIL;
    else if (_facility == "LOG_NEWS")     return LOG_NEWS;
    else if (_facility == "LOG_SYSLOG")   return LOG_SYSLOG;
    else if (_facility == "LOG_USER")     return LOG_USER;
    else                                  return LOG_AUTH;
#endif
}

struct LoggerTypeComparator
{
    const int type_;

    LoggerTypeComparator(const int& type)
      : type_(type)
    { }

    bool operator()(const std::pair<uid_type, BaseLog*>& data) const
    {
        return data.second->type_ == type_;
    }
};

Logger* Logger::ptr(){
	if (!Logger::instance_)
	{
		Logger::instance_ = new Logger();
	}
	return Logger::instance_;
}

Logger& Logger::instance()
{
	return *Logger::ptr();
}

void Logger::setInstance(Logger* l)
{
	if (&(*Logger::instance_) != &(*l)) {
            delete Logger::instance_;
            Logger::instance_ = l;
	}
}


Logger::Logger()
    : currUID_(0)
{
    defaultLog_ = new ConsoleLog(BASE_LOG_LIB_FILTER_ALL, BASE_LOG_FILTER_INFO); // info for admins
}

Logger::~Logger()
{
    userLogs_.clear();
    delete defaultLog_;
}

unsigned Logger::registerLogger(BaseLog * logger)
{
	if ( !logger->valid() ) {
		delete logger;
		return 0;
	}
	base::ScopedLock locker(mutex_);
	userLogs_.push_back(std::make_pair(++currUID_, logger));
	return currUID_;
}

unsigned Logger::addFileLog(const char* libFilter, int logFilter, const char* logName)
{
    BaseLog* log = new FileLog(libFilter, logFilter, logName);
    return registerLogger(log);
}

unsigned Logger::addConsoleLog(const char* libFilter, int logFilter)
{
    BaseLog* log = new ConsoleLog(libFilter, logFilter);
	return registerLogger(log);
}

unsigned Logger::addSysLog(const char* daemonName, int facility, const char* libFilter, int logFilter)
{
    BaseLog* log = new SysLog(daemonName, facility, libFilter, logFilter);
	return registerLogger(log);
}


bool Logger::deleteLog(uid_type uid)
{
	base::ScopedLock locker(mutex_);
    for(BaseLogArr::iterator it = userLogs_.begin(); it != userLogs_.end(); ++it)
    {
        BaseLog* log = it->second;
        if (it->first == uid)
        {
            userLogs_.erase(it);
            delete log;
            return true;
        }
    }

    return false;
}


void Logger::deleteAllLogsByType(int type)
{
	base::ScopedLock locker(mutex_);
    
    userLogs_.erase(std::remove_if(userLogs_.begin(), userLogs_.end(), LoggerTypeComparator(type)), userLogs_.end());
}


void Logger::send(const char* module, LOG_LEVEL logLevel, const std::string& text)
{
	base::ScopedLock locker(mutex_);
    if ( userLogs_.empty() && (logLevel & defaultLog_->logFilter_)) {
        defaultLog_->post(logLevel, text);
    } else {
        for (size_t i = 0; i < userLogs_.size(); ++i) {
            BaseLog* log = userLogs_[i].second;

            if ((log->logFilter_ & logLevel) &&
                strstr(module, log->libFilter_))//(!strcmp(log->libFilter_, BASE_LOG_LIB_FILTER_ALL) ||
            {
                log->post(logLevel, text);
            }
        }
    }
}

void Logger::event(int eventId)
{
	switch(eventId)
	{
	case 0:
		{
			base::ScopedLock locker(mutex_);
			for (BaseLogArr::iterator it = userLogs_.begin(); it != userLogs_.end(); ++it)
			{
				it->second->SIGHUP_event();
			}
		}
		break;
	default:
		break;
	}
}

LogEntry::LogEntry(const char* file, int line, const char* func, LOG_LEVEL logLevel)
    : module_(file)
    , file_(file)
    , line_(line)
    , func_(func)
    , logLevel_(logLevel)
{}

LogEntry::LogEntry(const char* module, const char* file, int line, const char* func, LOG_LEVEL logLevel)
    : module_(module)
    , file_(file)
    , line_(line)
    , func_(func)
    , logLevel_(logLevel)
{}

LogEntry::LogEntry(const char* file, int line, const char* func, LOG_LEVEL logLevel, const char *message, ...)
    : module_(file)
    , file_(file)
    , line_(line)
    , func_(func)
    , logLevel_(logLevel)
{
	const int len = 1000;
    char buffer[len];
    va_list args;
    va_start(args, message);
    vsnprintf(buffer, len, message, args);
    va_end(args);

    temp_ << buffer;
}

LogEntry::LogEntry(const char* module, const char* file, int line, const char* func, LOG_LEVEL logLevel, const char *message, ...)
    : module_(module)
    , file_(file)
    , line_(line)
    , func_(func)
    , logLevel_(logLevel)
{
	const int len = 1024;
    char buffer[len];
    va_list args;
    va_start(args, message);
    vsnprintf(buffer, len, message, args);
    va_end(args);

    temp_ << buffer;
}

LogEntry::~LogEntry() {
	flush();
    Logger::instance().send(module_, logLevel_, log_.str());
}

const char* _logLevelToTag(LOG_LEVEL level) {
    switch (level)
    {
    case TRACE_LEVEL:
        return "[TRACE] ";
    case DEBUG_LEVEL:
        return "[DEBUG] ";
    case INFO_LEVEL:
        return "[INFO]  ";
    case NOTICE_LEVEL:
        return "[NOTICE]";
    case WARNING_LEVEL:
        return "[WARN]  ";
    case ERROR_LEVEL:
        return "[ERROR] ";
    case FATAL_LEVEL:
        return "[FATAL] ";
    default:
        return "[??????]";
    }
}

// TBD isatty() member of LogEntry to be catched here too
void LogEntry::flush() {
    auto tid = 0;
//    time_t currTime = time(0);
//    struct tm* timeinfo;
//    timeinfo = localtime(&currTime);
//    char buffer[50];
//    strftime(buffer, 50, "%c", timeinfo);
	std::string f(file_);
	std::string::size_type pos = f.find_last_of("/\\");
	if (pos != std::string::npos){
		f = f.substr(pos + 1);
	}
#ifdef _WIN32
    static size_t prefixLen = 0;
    //if (!prefixLen)
		//prefixLen = strlen(__FILE__) - strlen("logger.cpp");
#else
	static __thread size_t prefixLen = 0;// strlen(__FILE__) - strlen("logger.cpp");
#endif    

	//log_ << std::hex << tid() << std::dec;
	log_ << tid;
    if (temp_.str().find('\n') == std::string::npos) {
    	log_ << " " << temp_.str().c_str() << " [" << func_ << " " << f << " +" << line_ << "]" << std::endl;;
   
    } else {
    	log_ << " [" << func_ << " " << f << " +" << line_ << "]" << std::endl <<
			temp_.str().c_str() << std::endl;
    }
}
}
}
