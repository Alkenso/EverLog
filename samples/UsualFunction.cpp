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

/* Backend function */
void PrintConsole(const std::string& message)
{
    std::cout << message << '\n';
}

using Function = decltype(&PrintConsole);
using Log = everlog::Everlog<Function>;

EVERLOG_DECLARE_DEFAULT(Log)

class LogEvent : public Log::EventType
{
public:
    explicit LogEvent(const std::string& s) : m_message(s) {}
    
    virtual void writeWithBackend(const everlog::Severity severity, Function& h) const override
    {
        std::stringstream ss;
        ss << "Level " << static_cast<int64_t>(severity) << ": " << m_message;
        h(ss.str());
    }
    
private:
    const std::string m_message;
};

int main()
{
    Log& logger = everlog::DefaultInstance::init(everlog::Severity::Warning);
    
    logger.addBackend(&PrintConsole);
    
    LOG_ERROR(LogEvent("Error here!"));
    
    return 0;
}
