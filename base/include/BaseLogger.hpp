#pragma once
#include <mutex>

#include <sstream>
#include <vector>
#include <signal.h>

// Template for you subproject logger
//#define SUB_PROJ_NAME_LOGF_TRACE(MESSAGE, ...)BASE_LOGF_TRACE(MESSAGE, ##__VA_ARGS__)
//#define SUB_PROJ_NAME_LOGF_DEBUG(MESSAGE, ...)BASE_LOGF_DEBUG(MESSAGE, ##__VA_ARGS__)
//#define SUB_PROJ_NAME_LOGF_INFO(MESSAGE, ...)BASE_LOGF_INFO(MESSAGE, ##__VA_ARGS__)
//#define SUB_PROJ_NAME_LOGF_NOTICE(MESSAGE, ...)BASE_LOGF_NOTICE(MESSAGE, ##__VA_ARGS__)
//#define SUB_PROJ_NAME_LOGF_WARNING(MESSAGE, ...)BASE_LOGF_WARNING(MESSAGE, ##__VA_ARGS__)
//#define SUB_PROJ_NAME_LOGF_ERROR(MESSAGE, ...)BASE_LOGF_ERROR(MESSAGE, ##__VA_ARGS__)
//#define SUB_PROJ_NAME_LOGF_FATAL(MESSAGE, ...)BASE_LOGF_FATAL(MESSAGE, ##__VA_ARGS__)
//
//#define SUB_PROJ_NAME_LOG_TRACE(CHAIN)BASE_LOG_TRACE(CHAIN)
//#define SUB_PROJ_NAME_LOG_DEBUG(CHAIN)BASE_LOG_DEBUG(CHAIN)
//#define SUB_PROJ_NAME_LOG_INFO(CHAIN)BASE_LOG_INFO(CHAIN)   
//#define SUB_PROJ_NAME_LOG_NOTICE(CHAIN)BASE_LOG_NOTICE(CHAIN) 
//#define SUB_PROJ_NAME_LOG_WARNING(CHAIN)BASE_LOG_WARNING(CHAIN)
//#define SUB_PROJ_NAME_LOG_ERROR(CHAIN)BASE_LOG_ERROR(CHAIN)  
//#define SUB_PROJ_NAME_LOG_FATAL(CHAIN)BASE_LOG_FATAL(CHAIN)  
//
//#define SUB_PROJ_NAME_LOG_CUSTOM_TRACE(MODULE, CHAIN)BASE_LOG_CUSTOM_TRACE(MODULE, CHAIN)
//#define SUB_PROJ_NAME_LOG_CUSTOM_DEBUG(MODULE, CHAIN)BASE_LOG_CUSTOM_DEBUG(MODULE, CHAIN)
//#define SUB_PROJ_NAME_LOG_CUSTOM_INFO(MODULE, CHAIN)BASE_LOG_CUSTOM_INFO(MODULE, CHAIN)    
//#define SUB_PROJ_NAME_LOG_CUSTOM_NOTICE(MODULE, CHAIN)BASE_LOG_CUSTOM_NOTICE(MODULE, CHAIN)  
//#define SUB_PROJ_NAME_LOG_CUSTOM_WARNING(MODULE, CHAIN)BASE_LOG_CUSTOM_WARNING(MODULE, CHAIN) 
//#define SUB_PROJ_NAME_LOG_CUSTOM_ERROR(MODULE, CHAIN)BASE_LOG_CUSTOM_ERROR(MODULE, CHAIN)
//#define SUB_PROJ_NAME_LOG_CUSTOM_FATAL(MODULE, CHAIN)BASE_LOG_CUSTOM_FATAL(MODULE, CHAIN)
//#define SUB_PROJ_NAME_LOGF_CUSTOM_TRACE(MODULE, MESSAGE, ...)BASE_LOGF_CUSTOM_TRACE(MODULE, MESSAGE, ##__VA_ARGS__)
//#define SUB_PROJ_NAME_LOGF_CUSTOM_DEBUG(MODULE, MESSAGE, ...)BASE_LOGF_CUSTOM_DEBUG(MODULE, MESSAGE, ##__VA_ARGS__)
//#define SUB_PROJ_NAME_LOGF_CUSTOM_INFO(MODULE, MESSAGE, ...)BASE_LOGF_CUSTOM_INFO(MODULE, MESSAGE, ##__VA_ARGS__)
//#define SUB_PROJ_NAME_LOGF_CUSTOM_NOTICE(MODULE, MESSAGE, ...)BASE_LOGF_CUSTOM_NOTICE(MODULE, MESSAGE, ##__VA_ARGS__)
//#define SUB_PROJ_NAME_LOGF_CUSTOM_WARNING(MODULE, MESSAGE, ...)BASE_LOGF_CUSTOM_WARNING(MODULE, MESSAGE, ##__VA_ARGS__)
//#define SUB_PROJ_NAME_LOGF_CUSTOM_ERROR(MODULE, MESSAGE, ...)BASE_LOGF_CUSTOM_ERROR(MODULE, MESSAGE, ##__VA_ARGS__)
//#define SUB_PROJ_NAME_LOGF_CUSTOM_FATAL(MODULE, MESSAGE, ...)BASE_LOGF_CUSTOM_FATAL(MODULE, MESSAGE, ##__VA_ARGS__)
namespace etu {
namespace base {
	
typedef unsigned int				uid_type; // as type

// SIGHUP event
#define BASE_LOG_SIGHUP_EVENT base::Logger::instance().event(SIGHUP);
#define BASE_LOG_SET_LOGGER(LOGGER) base::Logger::setInstance(LOGGER)
#define BASE_LOG_GET_LOGGER() base::Logger::ptr()

// printf style
#define BASE_LOGF(PRIORITY, MESSAGE, ...) { base::LogEntry(__FILE__, __LINE__, __FUNCTION__, PRIORITY, MESSAGE,##__VA_ARGS__); }
#ifdef _DEBUG
    #define BASE_LOGF_TRACE(MESSAGE, ...) BASE_LOGF(base::TRACE_LEVEL, MESSAGE, ##__VA_ARGS__)
	#define BASE_LOGF_DEBUG(MESSAGE, ...) BASE_LOGF(base::DEBUG_LEVEL, MESSAGE, ##__VA_ARGS__)
#else
    #define BASE_LOGF_TRACE(MESSAGE, ...)
	#define BASE_LOGF_DEBUG(MESSAGE, ...)
#endif
#define BASE_LOGF_INFO(MESSAGE, ...) BASE_LOGF(base::INFO_LEVEL, MESSAGE, ##__VA_ARGS__)
#define BASE_LOGF_NOTICE(MESSAGE, ...) BASE_LOGF(base::NOTICE_LEVEL, MESSAGE, ##__VA_ARGS__)
#define BASE_LOGF_WARNING(MESSAGE, ...) BASE_LOGF(base::WARNING_LEVEL, MESSAGE, ##__VA_ARGS__)
#define BASE_LOGF_ERROR(MESSAGE, ...) BASE_LOGF(base::ERROR_LEVEL, MESSAGE, ##__VA_ARGS__)
#define BASE_LOGF_FATAL(MESSAGE, ...) BASE_LOGF(base::FATAL_LEVEL, MESSAGE, ##__VA_ARGS__)

// printf style custom
#define BASE_LOGF_CUSTOM(MODULE, PRIORITY, MESSAGE, ...) { base::LogEntry(MODULE, __FILE__, __LINE__, __FUNCTION__, PRIORITY, MESSAGE, ##__VA_ARGS__); }
#ifdef _DEBUG
	#define BASE_LOGF_CUSTOM_TRACE(MODULE, MESSAGE, ...) BASE_LOGF_CUSTOM(MODULE, base::TRACE_LEVEL, MESSAGE, ##__VA_ARGS__)
    #define BASE_LOGF_CUSTOM_DEBUG(MODULE, MESSAGE, ...) BASE_LOGF_CUSTOM(MODULE, base::DEBUG_LEVEL, MESSAGE, ##__VA_ARGS__)
#else
	#define BASE_LOGF_CUSTOM_TRACE(MODULE, MESSAGE, ...)
    #define BASE_LOGF_CUSTOM_DEBUG(MODULE, MESSAGE, ...)
#endif
#define BASE_LOGF_CUSTOM_INFO(MODULE, MESSAGE, ...) BASE_LOGF_CUSTOM(MODULE, base::INFO_LEVEL, MESSAGE, ##__VA_ARGS__)
#define BASE_LOGF_CUSTOM_NOTICE(MODULE, MESSAGE, ...) BASE_LOGF_CUSTOM(MODULE, base::NOTICE_LEVEL, MESSAGE, ##__VA_ARGS__)
#define BASE_LOGF_CUSTOM_WARNING(MODULE, MESSAGE, ...) BASE_LOGF_CUSTOM(MODULE, base::WARNING_LEVEL, MESSAGE, ##__VA_ARGS__)
#define BASE_LOGF_CUSTOM_ERROR(MODULE, MESSAGE, ...) BASE_LOGF_CUSTOM(MODULE, base::ERROR_LEVEL, MESSAGE, ##__VA_ARGS__)
#define BASE_LOGF_CUSTOM_FATAL(MODULE, MESSAGE, ...) BASE_LOGF_CUSTOM(MODULE, base::FATAL_LEVEL, MESSAGE, ##__VA_ARGS__)

// chain style
#define BASE_LOG(PRIORITY, CHAIN)   { base::LogEntry(__FILE__, __LINE__, __FUNCTION__, PRIORITY) << CHAIN; }
#ifdef _DEBUG
	#define BASE_LOG_TRACE(CHAIN) BASE_LOG(base::TRACE_LEVEL, CHAIN)
    #define BASE_LOG_DEBUG(CHAIN) BASE_LOG(base::DEBUG_LEVEL, CHAIN)
#else
	#define BASE_LOG_TRACE(CHAIN)
    #define BASE_LOG_DEBUG(CHAIN)
#endif
#define BASE_LOG_INFO(CHAIN)    BASE_LOG(base::INFO_LEVEL, CHAIN)
#define BASE_LOG_NOTICE(CHAIN)  BASE_LOG(base::NOTICE_LEVEL, CHAIN)
#define BASE_LOG_WARNING(CHAIN) BASE_LOG(base::WARNING_LEVEL, CHAIN)
#define BASE_LOG_ERROR(CHAIN)   BASE_LOG(base::ERROR_LEVEL, CHAIN)
#define BASE_LOG_FATAL(CHAIN)   BASE_LOG(base::FATAL_LEVEL, CHAIN)

// chain style custom
#define BASE_LOG_CUSTOM(PRIORITY, MODULE, CHAIN)   { base::LogEntry(MODULE, __FILE__, __LINE__, __FUNCTION__, PRIORITY) << CHAIN; }
#ifdef _DEBUG
	#define BASE_LOG_CUSTOM_TRACE(MODULE, CHAIN)   BASE_LOG_CUSTOM(base::TRACE_LEVEL, MODULE, CHAIN)
    #define BASE_LOG_CUSTOM_DEBUG(MODULE, CHAIN)   BASE_LOG_CUSTOM(base::DEBUG_LEVEL, MODULE, CHAIN)
#else
	#define BASE_LOG_CUSTOM_TRACE(MODULE, CHAIN)
    #define BASE_LOG_CUSTOM_DEBUG(MODULE, CHAIN)
#endif
#define BASE_LOG_CUSTOM_INFO(MODULE, CHAIN)    BASE_LOG_CUSTOM(base::INFO_LEVEL, MODULE, CHAIN)
#define BASE_LOG_CUSTOM_NOTICE(MODULE, CHAIN)  BASE_LOG_CUSTOM(base::NOTICE_LEVEL, MODULE, CHAIN)
#define BASE_LOG_CUSTOM_WARNING(MODULE, CHAIN) BASE_LOG_CUSTOM(base::WARNING_LEVEL, MODULE, CHAIN)
#define BASE_LOG_CUSTOM_ERROR(MODULE, CHAIN)   BASE_LOG_CUSTOM(base::ERROR_LEVEL, MODULE, CHAIN)
#define BASE_LOG_CUSTOM_FATAL(MODULE, CHAIN)   BASE_LOG_CUSTOM(base::FATAL_LEVEL, MODULE, CHAIN)

#define BASE_LOG_LIB_FILTER_ALL "/"

#define BASE_LOG_FILTER_ALL		(BASE_LOG_FILTER_TRACE)
#define BASE_LOG_FILTER_TRACE   (base::TRACE_LEVEL | BASE_LOG_FILTER_DEBUG)
#define BASE_LOG_FILTER_DEBUG   (base::DEBUG_LEVEL | BASE_LOG_FILTER_INFO)
#define BASE_LOG_FILTER_INFO    (base::INFO_LEVEL | BASE_LOG_FILTER_NOTICE)
#define BASE_LOG_FILTER_NOTICE  (base::NOTICE_LEVEL | BASE_LOG_FILTER_WARNING)
#define BASE_LOG_FILTER_WARNING (base::WARNING_LEVEL | BASE_LOG_FILTER_ERROR)
#define BASE_LOG_FILTER_ERROR   (base::ERROR_LEVEL | BASE_LOG_FILTER_FATAL)
#define BASE_LOG_FILTER_FATAL   (base::FATAL_LEVEL)

enum LOG_LEVEL
{
	TRACE_LEVEL   = 0x40,
    DEBUG_LEVEL   = 0x20,
    INFO_LEVEL    = 0x10,
    NOTICE_LEVEL  = 0x08,
    WARNING_LEVEL = 0x04,
    ERROR_LEVEL   = 0x02,
    FATAL_LEVEL   = 0x01
};


/**
 * Get LOG_LEVEL value coresponding to sys log constants
 */
LOG_LEVEL transform_log_level( int syslog_priority ) throw();

class LogEntry {
public:
    LogEntry(const char* file, int line, const char* func, LOG_LEVEL logLevel);
    LogEntry(const char* module, const char* file, int line, const char* func, LOG_LEVEL logLevel);
    LogEntry(const char* file, int line, const char* func, LOG_LEVEL logLevel, const char *message, ...);
    LogEntry(const char* module, const char* file, int line, const char* func, LOG_LEVEL logLevel, const char *message, ...);
    ~LogEntry();

    template <typename T>
    LogEntry& operator << (const T& t) {
        temp_ << t;
        return *this;
    }

private:
    void flush();
    const char* module_;
    const char* file_;
    const int line_;
    const char* func_;
    const LOG_LEVEL logLevel_;
    std::ostringstream log_;
    std::ostringstream temp_;
};

#if CXX11
namespace __private
{
	inline void base_log_write( LogEntry& logger ) { }

	template <class T, class ...Args> inline void base_log_write( LogEntry& logger, const T& arg, const Args& ...args )
	{
		logger << arg;
		base_log_write( logger, args... );
	}
	
	template <class ...Args> inline void base_log_write( const char* file, const int line, const char* func, LOG_LEVEL prio, const Args& ...args )
	{
		LogEntry logger( file, line, func, prio );
		__private::base_log_write( logger, args... );
	}
}

# define PRIVATE_BASE_LOGT(PRIORITY, ...)	{ base::__private::base_log_write(__FILE__, __LINE__, __FUNCTION__, PRIORITY, ##__VA_ARGS__ ); }
# define BASE_LOGT(...)						PRIVATE_BASE_LOGT(base::INFO_LEVEL, ##__VA_ARGS__)
# define BASE_LOGT_ERROR(...)					PRIVATE_BASE_LOGT(base::ERROR_LEVEL, ##__VA_ARGS__)
# define BASE_LOGT_WARNING(...)				PRIVATE_BASE_LOGT(base::WARNING_LEVEL, ##__VA_ARGS__)
# define BASE_LOGT_INFO(...)					BASE_LOGT(__VA_ARGS__)
# define BASE_LOGT_NOTICE(...)					PRIVATE_BASE_LOGT(base::NOTICE_LEVEL, ##__VA_ARGS__)
# ifdef DEBUG
#  define BASE_LOGT_TRACE(...)				PRIVATE_BASE_LOGT(base::TRACE_LEVEL, ##__VA_ARGS__)
#  define BASE_LOGT_DEBUG(...)				PRIVATE_BASE_LOGT(base::DEBUG_LEVEL, ##__VA_ARGS__)
# else
#  define BASE_LOGT_TRACE(...)
#  define BASE_LOGT_DEBUG(...)
# endif

#else

# define PRIVATE_BASE_LOGT(PRIORITY, ...)
# define BASE_LOGT(...)
# define BASE_LOGT_ERROR(...)
# define BASE_LOGT_WARNING(...)
# define BASE_LOGT_INFO(...)
# define BASE_LOGT_NOTICE(...)
# define BASE_LOGT_DEBUG(...)
# define BASE_LOGT_TRACE(...)

#endif

enum LoggerType
{
    CONSOLE_LOG = 0,
    SYS_LOG     = 1,
    FILE_LOG    = 2,
    ZEND_LOG    = 3,
    BUFFER_LOG  = 4,
    HHVM_LOG    = 5
};

/**
 * @name BaseLogger
 */
class BaseLog {
public:
    BaseLog(const char* libFilter, int logFilter, int type)
    	: libFilter_(libFilter)
    	, logFilter_(logFilter)
        , type_(type)
    {}
        
    virtual ~BaseLog() {}

    virtual void post(LOG_LEVEL logLevel, const std::string& text) = 0;
    virtual bool valid() = 0; // by exceptions? no! we have default logger to fallback
    virtual void SIGHUP_event() {}

    const char*     libFilter_;
    const int       logFilter_;
    const int       type_;
};

typedef std::vector< std::pair<uid_type, BaseLog*> > BaseLogArr; // as class

/**
 * Logs manager class
 * @name LogManager
 */
class Logger {
public:
	static Logger& instance();
	static Logger* ptr();
	static void setInstance(Logger*);
    unsigned addFileLog(const char* libFilter, int priorityFilter, const char* logName);
    unsigned addConsoleLog(const char* libFilter, int priorityFilter);
    unsigned addSysLog(const char* daemonName, int facility, const char* libFilter, int priorityFilter);
    unsigned registerLogger(BaseLog * logger);
    bool deleteLog(uid_type uid);
    void deleteAllLogsByType(int type);
    void send(const char* file, LOG_LEVEL logLevel, const std::string& text);
    void event(int eventId);

private:
    static Logger* instance_;

    Logger();
    ~Logger();

    BaseLogArr userLogs_;
	std::mutex mutex_;

    BaseLog* defaultLog_;
    uid_type currUID_;
};


uid_type createConsoleLog(const char* libFilter, int priority);
uid_type createFileLog(const char* libFilter, int priority, const char* fileName);
uid_type createSysLog(const char* daemonName, int facility, const char* libFilter, int priority);
uid_type registerExternalLog(BaseLog * logger);
bool deleteLogger( uid_type LoggerId );
void deleteAllConsoleLogs();
void deleteAllLogsByType(int type);


}
}
