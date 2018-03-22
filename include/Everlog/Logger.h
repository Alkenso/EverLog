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

#include <Everlog/IEvent.h>

namespace everlog
{
    template <typename... Backends>
    class EverlogBase;
    
    template <typename Backend>
    class EverlogBase<Backend>
    {
    public:
        void addBackend(std::shared_ptr<Backend> backend);
        
    protected:
        template <typename ... Args>
        void logEvent(const IEventBase<Backend, Args...>& event, typename std::decay<Args>::type... args);
        
    private:
        std::vector<std::shared_ptr<Backend>> m_backends;
    };
    
    template <typename Backend, typename... Backends>
    class EverlogBase<Backend, Backends...>: public EverlogBase<Backend>, public EverlogBase<Backends...>
    {
    public:
        using EverlogBase<Backend>::addBackend;
        using EverlogBase<Backends...>::addBackend;
        
    protected:
        template <typename EventType, typename ... Args>
        void logEvent(const EventType& event, Args&&... args);
    };
    
    template <typename... Backends>
    class EverlogBase1: public EverlogBase<Backends...>
    {
    public:
        using EverlogBase<Backends...>::addBackend;
        
        template <typename T>
        void addBackend(std::unique_ptr<T> wrappedBackend);
        
        template <typename T>
        void addBackend(T&& wrappedBackend);
    };
    
    template <bool UseSeverity, typename ... Backends>
    class Everlog;
    
    template <typename ... Backends>
    class Everlog<false, Backends...>: public EverlogBase1<Backends...>
    {
    public:
        using EventType = IEvent<false, Backends...>;
        
        void logEvent(const EventType& event);
    };
    
    template <typename ... Backends>
    class Everlog<true, Backends...>: public EverlogBase1<Backends...>
    {
    public:
        using EventType = IEvent<true, Backends...>;
        
        explicit Everlog(const Severity severity);
        void setSeverity(const Severity severity);
        void logEvent(const Severity severity, const EventType& event);
        
    private:
        std::atomic<Severity> m_severity;
    };
}

template <typename Backend>
void everlog::EverlogBase<Backend>::addBackend(std::shared_ptr<Backend> backend)
{
    m_backends.push_back(backend);
}

template <typename Backend>
template <typename ... Args>
void everlog::EverlogBase<Backend>::logEvent(const IEventBase<Backend, Args...>& event, typename std::decay<Args>::type... args)
{
    for (auto& b : m_backends)
    {
        event.writeWithBackend(*b, std::forward<Args>(args)...);
    }
}

template <typename Backend, typename... Backends>
template <typename EventType, typename ... Args>
void everlog::EverlogBase<Backend, Backends...>::logEvent(const EventType& event, Args&&... args)
{
    EverlogBase<Backend>::logEvent(event, std::forward<Args>(args)...);
    EverlogBase<Backends...>::logEvent(event, std::forward<Args>(args)...);
}

template <typename... Backends>
template <typename T>
void everlog::EverlogBase1<Backends...>::addBackend(std::unique_ptr<T> wrappedBackend)
{
    EverlogBase<Backends...>::addBackend(std::move(wrappedBackend));
}

template <typename... Backends>
template <typename T>
void everlog::EverlogBase1<Backends...>::addBackend(T&& wrappedBackend)
{
    EverlogBase<Backends...>::addBackend(std::make_shared<T>(std::forward<T>(wrappedBackend)));
}

template <typename ... Backends>
void everlog::Everlog<false, Backends...>::logEvent(const EventType& event)
{
    EverlogBase<Backends...>::logEvent(event);
}

template <typename ... Backends>
everlog::Everlog<true, Backends...>::Everlog(const Severity severity)
: m_severity(severity)
{}

template <typename ... Backends>
void everlog::Everlog<true, Backends...>::setSeverity(const Severity severity)
{
    m_severity = severity;
}

template <typename ... Backends>
void everlog::Everlog<true, Backends...>::logEvent(const Severity severity, const EventType& event)
{
    if (Severity::None == severity ||
        Severity::None == m_severity ||
        severity > m_severity)
    {
        return;
    }
    
    EverlogBase<Backends...>::logEvent(event, severity);
}
