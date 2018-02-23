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

#include <utility>

#include <Everlog/IEventHandler.h>

namespace everlog
{
    template <typename ExactBackend, typename ... Backends>
    class EventHandler : public IEventHandler<Backends...>
    {
    public:
        EventHandler(ExactBackend&& backend);
        
        virtual void handleEvent(const Severity severity, const IEvent<Backends...>& event) override;
        
    protected:
        ExactBackend& backend();
        
    private:
        ExactBackend m_backend;
    };
}

template <typename ExactBackend, typename ... Backends>
everlog::EventHandler<ExactBackend, Backends...>::EventHandler(ExactBackend&& backend)
: m_backend(std::forward<ExactBackend>(backend))
{}

template <typename ExactBackend, typename ... Backends>
void everlog::EventHandler<ExactBackend, Backends...>::handleEvent(const Severity severity, const IEvent<Backends...>& event)
{
    event.writeWithBackend(severity, m_backend);
}

template <typename ExactBackend, typename ... Backends>
ExactBackend& everlog::EventHandler<ExactBackend, Backends...>::backend()
{
    return m_backend;
}
