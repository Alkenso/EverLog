# EverLog

Everlog is lightweight header-only library that allows perform logging of any kind of the log object into different destinations (backends)

Everlog is useful when the goal is to log some non-string information that have to be written into different destinations in variadic formats.

For example, you need to log some event into console output and into database.
Console requires human-readable message and the database requires SQL query string.
It could be easily performed using Everlog.

Out of the box Everlog provides:
- severity;
- global logger instances with convenient macroses;
- support of any kind of backends: functions, classes, interfaces.

Everlog is template-based solution and requires C++11.

**Example**

You have some complex information that have to be logged.
Let's all this log information could be described as class fields (or typical structure):

{code}class EventInfo
{
private:
    std::string m_message;
    std::string m_userId;
    time_t m_timestamp;
};{code}

This information must be written to sqlite database and into syslog.
Note that database have few tables: Errors, Warnings, etc.

{code}void PrintToSyslog(const int logLevel, const char* message)
{
    syslog(logLevel, message);
}

class SQLiteDb
{
public:
    ...

    void execQuery(const std::string& query)
    {
        std::cout << "Execute query: " << query << '\n';
    }

private:
    // sql implementation
};{code}


Declare Logger type
{code}using Log = everlog::Everlog<SQLiteDb, decltype(&PrintToSyslog)>;{code}

Declate logger global instance. (Note that there could be more then on global logger instance
{code}EVERLOG_DECLARE_DEFAULT(Log){code}

Extend EventInfo class to allow logging itself into different backends
Everlog<...>::EventType is used to force derived class to override necessary log methods

{code}class EventInfo : public Log::EventType
{
public:
    EventInfo(const std::string& userId, const std::string& message) : m_userId(userId), m_message(message), m_timestamp(time(0)) {}
    
    virtual void writeWithBackend(const everlog::Severity severity, SQLiteDb& h) const override
    {
        std::stringstream query;
        query << "INSERT INTO " << TableNameFromSeverity(severity) 
              << " (message, user_id, time) "
              << "VALUES (" << m_message << ", "
              << m_userId << ", "
              << m_timestamp << ")";
        h.execQuery(query.str());
    }
    
    virtual void writeWithBackend(const everlog::Severity severity, decltype(&PrintToSyslog)& h) const override
    {
        h(SyslogLevelFromSeverity(severity), std::to_string(m_timestamp) + "[" + m_userId + "]: " + m_message);
    }
    
private:
    std::string m_userId;
    std::string m_message;
    time_t m_timestamp;
};{code}

The last step is to initialize the logger

{code}int main(void)
{
    Log& logger = everlog::DefaultInstance::init(everlog::Severity::Info);
    
    logger.addBackend(&PrintToSyslog);
    logger.addBackend(SQLiteDb("/path/to/database.db"));
    
    ...
}{code}

Now we can use Everlog log system

{code}void Foo()
{
    LOG_INFO(EventInfo("John Doe", "Performing Foo..."));
    ...

    LOG_ERROR(EventInfo("John Doe", "Something went wrong"));
}

... and now we have messages logged into syslog in readable form and saved in the database
