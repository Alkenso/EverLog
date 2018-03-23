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

#include <Everlog/Severity.h>

namespace everlog
{
    /* Base universal class for all everlog events */
    template <typename Backend, typename... Args>
    class IEventBase
    {
    public:
        virtual ~IEventBase() {}
        
        virtual void writeWithBackend(Backend&, Args...) const = 0;
    };
    
    /* General definition to apply recursive variadic args
     * The class itself forces events to be able to log with given list of backends */
    template <bool UseSeverity, typename... Backends>
    class IEvent;
    
    /* Classes for single backend */
    template <typename Backend>
    class IEvent<false, Backend> : public IEventBase<Backend>
    {
    public:
        virtual ~IEvent() {}
        
        virtual void writeWithBackend(Backend&) const override = 0;
    };
    
    template <typename Backend>
    class IEvent<true, Backend> : public IEventBase<Backend, const Severity>
    {
    public:
        virtual ~IEvent() {}
        
        virtual void writeWithBackend(Backend&, const Severity) const override = 0;
    };
    
    /* Class for multiple backends */
    template <bool UseSeverity, typename... Backends>
    class IEvent: public IEvent<UseSeverity, Backends>...
    {};
}
