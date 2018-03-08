//
//  main.cpp
//  EventLog
//
//  Created by Vladimir Vashurkin on 2/7/18.
//  Copyright © 2018 Vladimir Vashurkin. All rights reserved.
//

#include <iostream>
#include <sstream>

#include <Everlog/Everlog.h>

class SQLiteDb
{
public:
    void execQuery(const std::string& query)
    {
        std::cout << "Execute query: " << query << std::endl;
    }
};

class IStringBackend
{
public:
    virtual void logMessage(const std::string& message) = 0;
};

class StdoutBackend : public IStringBackend
{
public:
    virtual void logMessage(const std::string& message)
    {
        std::cout << "Out: " << message << '\n';
    }
    
};

class StderrBackend : public IStringBackend
{
public:
    virtual void logMessage(const std::string& message)
    {
        std::cerr << "Err: " << message << '\n';
    }
};


using Log = everlog::Everlog<SQLiteDb, IStringBackend>;
EVERLOG_DECLARE_DEFAULT(Log)

class LogEvent : public Log::EventType
{
public:
    explicit LogEvent(const std::string& s) : m_message(s) {}
    
    virtual void writeWithBackend(const everlog::Severity severity, SQLiteDb& h) const override
    {
        std::stringstream query;
        query << "INSERT INTO " << std::to_string(static_cast<int64_t>(severity)) << " (message) VALUES (" << m_message << ")";
        h.execQuery(query.str());
    }
    
    virtual void writeWithBackend(const everlog::Severity severity, IStringBackend& h) const override
    {
        h.logMessage("[" + std::to_string(static_cast<int64_t>(severity)) + "]: " + m_message);
    }
    
private:
    const std::string m_message;
};

int main()
{
    Log& logger = everlog::DefaultInstance::init(everlog::Severity::Warning);
    
    logger.addBackend(StdoutBackend());
    logger.addBackend(StderrBackend());
    logger.addBackend(SQLiteDb());
    
    LOG_ERROR(LogEvent("Something happened..."));
    
    return 0;
}
