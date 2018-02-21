/*
 * Copyright (c) 2018 Alkenso (Vashurkin Vladimir)
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

#include <Everlog/EventHandler.h>
#include <Everlog/IEvent.h>

namespace everlog
{
    template <typename ... Backends>
    class Everlog
    {
    public:
        using EventType = IEvent<Backends...>;
        using IEventHandlerType = IEventHandler<Backends...>;
        template <typename ExactBackend> using EventHandlerType = EventHandler<ExactBackend, Backends...>;
        
        explicit Everlog(const Severity severity = Severity::None);
        
        template <typename Handler, typename = typename std::enable_if<std::is_base_of<IEventHandlerType, Handler>::value>>
        void addHandler(std::unique_ptr<Handler> handler);
        
        template <typename ExactBackend>
        void addHandler(ExactBackend&& backend);
        
        
        void setSeverity(const Severity severity);
        void logEvent(const Severity severity, const EventType& event);
        
    private:
        std::vector<std::unique_ptr<IEventHandlerType>> m_handlers;
        std::atomic<Severity> m_severity;
        
        std::mutex m_handlersMutex;
    };
}

template <typename ... Backends>
everlog::Everlog<Backends...>::Everlog(const Severity severity /* = Severity::None */)
: m_severity(severity)
{}

template <typename ... Backends>
template <typename Handler, typename>
void everlog::Everlog<Backends...>::addHandler(std::unique_ptr<Handler> handler)
{
    std::lock_guard<std::mutex> lock(m_handlersMutex);
    m_handlers.emplace_back(std::move(handler));
}

template <typename ... Backends>
template <typename ExactBackend>
void everlog::Everlog<Backends...>::addHandler(ExactBackend&& backend)
{
    addHandler(std::unique_ptr<EventHandlerType<ExactBackend>>(new EventHandlerType<ExactBackend>(std::forward<ExactBackend>(backend))));
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
    
    for (std::unique_ptr<IEventHandlerType>& handler : m_handlers)
    {
        handler->handleEvent(m_severity, event);
    }
}
