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

#include <Everlog/IEvent.h>

namespace everlog
{
    template <typename... Backends>
    class BackendHolder;
    
    template <typename Backend>
    class BackendHolder<Backend>
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
    class BackendHolder<Backend, Backends...>: public BackendHolder<Backend>, public BackendHolder<Backends...>
    {
    public:
        using BackendHolder<Backend>::addBackend;
        using BackendHolder<Backends...>::addBackend;
        
    protected:
        template <typename EventType, typename ... Args>
        void logEvent(const EventType& event, Args&&... args);
    };
    
    
    template <typename... Backends>
    class EverlogBase: public BackendHolder<Backends...>
    {
    public:
        template <typename T>
        void addBackend(std::shared_ptr<T> backend);
        
        template <typename T>
        void addBackend(std::unique_ptr<T> backend);
        
        template <typename T>
        void addBackend(T&& backend);
    };
}


template <typename Backend>
void everlog::BackendHolder<Backend>::addBackend(std::shared_ptr<Backend> backend)
{
    m_backends.push_back(backend);
}

template <typename Backend>
template <typename ... Args>
void everlog::BackendHolder<Backend>::logEvent(const IEventBase<Backend, Args...>& event, typename std::decay<Args>::type... args)
{
    for (auto& b : m_backends)
    {
        event.writeWithBackend(*b, std::forward<Args>(args)...);
    }
}

template <typename Backend, typename... Backends>
template <typename EventType, typename ... Args>
void everlog::BackendHolder<Backend, Backends...>::logEvent(const EventType& event, Args&&... args)
{
    BackendHolder<Backend>::logEvent(event, std::forward<Args>(args)...);
    BackendHolder<Backends...>::logEvent(event, std::forward<Args>(args)...);
}


template <typename... Backends>
template <typename T>
void everlog::EverlogBase<Backends...>::addBackend(std::shared_ptr<T> backend)
{
    BackendHolder<Backends...>::addBackend(backend);
}

template <typename... Backends>
template <typename T>
void everlog::EverlogBase<Backends...>::addBackend(std::unique_ptr<T> backend)
{
    std::shared_ptr<T> sPtr = std::move(backend);
    BackendHolder<Backends...>::addBackend(sPtr);
}

template <typename... Backends>
template <typename T>
void everlog::EverlogBase<Backends...>::addBackend(T&& backend)
{
    BackendHolder<Backends...>::addBackend(std::make_shared<T>(std::forward<T>(backend)));
}
