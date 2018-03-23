# Everlog

Everlog is lightweight header-only library that allows perform logging of any kind of the log object into different destinations (backends)

Everlog is useful when the goal is to log some non-string information that have to be written into different destinations in variadic formats.

Out of the box Everlog provides:
- support of any kind of backends: functions, classes, interfaces;
- support of multiple backends behind single logger instance;
- global logger instances with convenient macroses;
- log severity;
- minimal usage of macroses

Everlog is template-based solution and requires ***C++11***.

**Typical situation when Everlog can be used**

For example, you need to log some event into console output and into database.
Console requires human-readable message and the database requires SQL query string.
It could be easily performed using Everlog.

# Example

Lets assume you have some complex information that have to be logged.
Let's all this log information could be described as class (or structure):

    class EventInfo
    {
    private:
        std::string m_message;
        std::string m_userId;
        time_t m_timestamp;
    };

This information must be written to sqlite database and into syslog.
Assume that database have few tables: Errors, Warnings, etc.

    void PrintToSyslog(const int logLevel, const char* message)
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
    };


Declare Logger type

    using Log = everlog::Everlog<true, SQLiteDb, decltype(&PrintToSyslog)>;

Declate logger global instance. (Note that there could be more then on global logger instance

    EVERLOG_DECLARE_DEFAULT(Log);

Extend EventInfo class to allow logging itself into different backends
Everlog<...>::EventType is used to force derived class to override necessary log methods

    class EventInfo : public Log::EventType
    {
    public:
        EventInfo(const std::string& userId, const std::string& message) : m_userId(userId), m_message(message), m_timestamp(time(0)) {}

        virtual void writeWithBackend(SQLiteDb& db, const everlog::Severity severity) const override
        {
            std::stringstream query;
            query << "INSERT INTO " << TableNameFromSeverity(severity) 
                  << " (message, user_id, time) "
                  << "VALUES (" << m_message << ", "
                  << m_userId << ", "
                  << m_timestamp << ")";
            db.execQuery(query.str());
        }

        virtual void writeWithBackend(decltype(&PrintToSyslog)& logFn, const everlog::Severity severity) const override
        {
            logFn(SyslogLevelFromSeverity(severity), std::to_string(m_timestamp) + "[" + m_userId + "]: " + m_message);
        }

    private:
        std::string m_userId;
        std::string m_message;
        time_t m_timestamp;
    };

The last step is to initialize the logger

    int main(void)
    {
        Log& logger = everlog::DefaultInstance::init(everlog::Severity::Info);

        logger.addBackend(&PrintToSyslog);
        logger.addBackend(SQLiteDb("/path/to/database.db"));

        ...
    }

Now we can use Everlog log system

    void Foo()
    {
        everlog::LogInfo(EventInfo("John Doe", "Performing Foo..."));
        ...

        everlog::LogError(EventInfo("John Doe", "Oops...! Something went wrong"));
    }

And that's it! Now we have messages logged into syslog in readable form and saved in the database
