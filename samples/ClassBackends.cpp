/*
 * Copyright (c) 2018 Alkenso (Vashurkin Vladimir), Alexander Smoktal
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 * THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <iostream>
#include <sstream>

#include <syslog.h>

#include <Everlog/Everlog.h>

class SQLiteDb
{
public:
    void execQuery(const std::string& query)
    {
        std::cout << "Execute query: " << query << '\n';
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
    
    syslog(<#int#>, <#const char *, ...#>)
    
    return 0;
}
