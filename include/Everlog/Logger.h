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

#pragma once

#include <vector>
#include <memory>
#include <atomic>
#include <mutex>

#include <Everlog/IEvent.h>

namespace everlog
{
    template <typename ... Backends>
    class Everlog
    {
    public:
        using EventType = IEvent<Backends...>;
        
        explicit Everlog(const Severity severity);
        
        template <typename Backend>
        void addBackend(Backend&& backend);
        
        template <typename Backend>
        void addWrappedBackend(std::unique_ptr<Backend> wrappedBackend);
        template <typename Backend>
        void addWrappedBackend(std::shared_ptr<Backend> wrappedBackend);
        
        void setSeverity(const Severity severity);
        void logEvent(const Severity severity, const EventType& event);
        
    private:
        using Handler_ft = std::function<void(const Severity severity, const EventType& event)>;
        std::vector<Handler_ft> m_handlers;
        
        std::atomic<Severity> m_severity;
    };
}

template <typename ... Backends>
everlog::Everlog<Backends...>::Everlog(const Severity severity)
: m_severity(severity)
{}

template <typename ... Backends>
template <typename Backend>
void everlog::Everlog<Backends...>::addBackend(Backend&& backend)
{
    addWrappedBackend(std::make_shared<Backend>(std::forward<Backend>(backend)));
}

template <typename ... Backends>
template <typename Backend>
void everlog::Everlog<Backends...>::addWrappedBackend(std::shared_ptr<Backend> wrappedBackend)
{
    m_handlers.emplace_back([wrappedBackend](const Severity severity, const EventType& event)
                            {
                                event.writeWithBackend(severity, *wrappedBackend);
                            });
}

template <typename ... Backends>
template <typename Backend>
void everlog::Everlog<Backends...>::addWrappedBackend(std::unique_ptr<Backend> wrappedBackend)
{
    addWrappedBackend(std::make_shared<Backend>(wrappedBackend.release()));
}

template <typename ... Backends>
void everlog::Everlog<Backends...>::setSeverity(const Severity severity)
{
    m_severity = severity;
}

template <typename ... Backends>
void everlog::Everlog<Backends...>::logEvent(const Severity severity, const EventType& event)
{
    if (Severity::None == severity ||
        Severity::None == m_severity ||
        severity > m_severity)
    {
        return;
    }
    
    for (const Handler_ft& handlerFn : m_handlers)
    {
        handlerFn(severity, event);
    }
}
