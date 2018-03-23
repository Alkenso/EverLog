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

void PrintConsole(const std::string& message)
{
    std::cout << message << '\n';
}

using Function = decltype(&PrintConsole);
using Log = everlog::Everlog<false, Function, SQLiteDb>;
EVERLOG_DECLARE_DEFAULT(Log);

class LogEvent : public Log::EventType
{
public:
    explicit LogEvent(const std::string& s) : m_message(s) {}
    
    virtual void writeWithBackend(SQLiteDb& h) const override
    {
        std::stringstream query;
        query << "INSERT INTO Events (message) VALUES (" << m_message << ")";
        h.execQuery(query.str());
    }
    
    virtual void writeWithBackend(Function& h) const override
    {
        h(m_message);
    }
    
private:
    const std::string m_message;
};

int main()
{
    Log& logger = everlog::DefaultInstance::init();
    
    logger.addBackend(SQLiteDb());
    logger.addBackend(&PrintConsole);
    
    LogEvent event("Something happened...");
    everlog::LogIt(event);
    
    return 0;
}
