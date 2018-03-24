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

#include <Everlog/Logger.h>
#include <Everlog/Init.h>

namespace everlog
{
    template <int InstanceIdx = kDefaultInstance, typename Event>
    void LogIt(const Severity severity, const Event& e);
    
    template <int InstanceIdx = kDefaultInstance, typename Event>
    void LogIt(const Event& e);
    
    
#define DECLARE_LOG_FN_I(_severity) \
    template <int instanceIdx, typename Event> \
    void Log##_severity(const Event& e) { LogIt<instanceIdx>(everlog::Severity::_severity, e); }
    
    DECLARE_LOG_FN_I(Fatal)
    DECLARE_LOG_FN_I(Error)
    DECLARE_LOG_FN_I(Warning)
    DECLARE_LOG_FN_I(Info)
    DECLARE_LOG_FN_I(Debug)
    DECLARE_LOG_FN_I(Verbose)
    
#define DECLARE_LOG_FN(_severity) \
    template <typename Event> \
    void Log##_severity(const Event& e) { LogIt<kDefaultInstance>(everlog::Severity::_severity, e); }
    
    DECLARE_LOG_FN(Fatal)
    DECLARE_LOG_FN(Error)
    DECLARE_LOG_FN(Warning)
    DECLARE_LOG_FN(Info)
    DECLARE_LOG_FN(Debug)
    DECLARE_LOG_FN(Verbose)
    
    namespace utils
    {
        template <int InstanceIdx, typename Event, typename... Args>
        void LogIt(const Event& e, Args&&... args);
    }
}


template <int InstanceIdx, typename Event>
void everlog::LogIt(const Severity severity, const Event& e)
{
    utils::LogIt<InstanceIdx>(e, severity);
}

template <int InstanceIdx, typename Event>
void everlog::LogIt(const Event& e)
{
    utils::LogIt<InstanceIdx>(e);
}

template <int InstanceIdx, typename Event, typename... Args>
void everlog::utils::LogIt(const Event& e, Args&&... args)
{
    auto* logger = everlog::GlobalInstance<InstanceIdx>::get();
    if (logger)
    {
        logger->logEvent(args..., e);
    }
}
