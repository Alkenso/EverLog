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

/* Backend function */
void PrintConsole(const std::string& message)
{
    std::cout << message << '\n';
}

using Function = std::function<void(const std::string&)>;
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
    
    /* Passing non std::function object explicitly specify template argument*/
    logger.addBackend<Function>(&PrintConsole);
    
    const std::string externalCtx = "ext";
    logger.addBackend<Function>([externalCtx](const std::string& message){
        std::cout << externalCtx << ": " << message << '\n';
    });
    
    LOG_ERROR(LogEvent("Error here!"));
    
    return 0;
}
