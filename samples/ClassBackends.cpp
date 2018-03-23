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


using Log = everlog::Everlog<true, SQLiteDb, IStringBackend, StderrBackend>;
EVERLOG_DECLARE_DEFAULT(Log);

class LogEvent : public Log::EventType
{
public:
    explicit LogEvent(const std::string& s) : m_message(s) {}
    
    virtual void writeWithBackend(SQLiteDb& h, const everlog::Severity severity) const override
    {
        std::stringstream query;
        query << "INSERT INTO " << std::to_string(static_cast<int64_t>(severity)) << " (message) VALUES (" << m_message << ")";
        h.execQuery(query.str());
    }
    
    virtual void writeWithBackend(IStringBackend& h, const everlog::Severity severity) const override
    {
        h.logMessage("[" + std::to_string(static_cast<int64_t>(severity)) + "]: " + m_message);
    }
    
    virtual void writeWithBackend(StderrBackend& h, const everlog::Severity severity) const override
    {
        h.logMessage("[" + std::to_string(static_cast<int64_t>(severity)) + "] - Error: " + m_message);
    }
    
private:
    const std::string m_message;
};

int main()
{
    Log& logger = everlog::DefaultInstance::init(everlog::Severity::Warning);
    
    /* Backends can be added by value */
    logger.addBackend(SQLiteDb());
    logger.addBackend(StdoutBackend());
    logger.addBackend(StderrBackend());
    
    /* Backends can be added wrapped in shared_ptr */
    logger.addBackend(std::make_shared<SQLiteDb>());
    logger.addBackend(std::make_shared<StdoutBackend>());
    logger.addBackend(std::make_shared<StderrBackend>());
    
    /* Backends can be added wrapped in unique_ptr */
    logger.addBackend(std::make_unique<SQLiteDb>());
    logger.addBackend(std::make_unique<StdoutBackend>());
    logger.addBackend(std::make_unique<StderrBackend>());
    
    LogEvent event("Something happened...");
    everlog::LogError(event);
    
    return 0;
}
