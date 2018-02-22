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

#include <Everlog/Logger.h>
#include <Everlog/Init.h>

#define EVERLOG_GET_LOGGER_I(instanceIdx)   (everlog::utils::EverlogInstance<instanceIdx>::instance)
#define EVERLOG_GET_LOGGER()                EVERLOG_GET_LOGGER_I(EVERLOG_DEFAULT_INSTANCE)


#define LOG_I(instanceIdx, severity, event) EVERLOG_GET_LOGGER_I(instanceIdx).logEvent(severity, event)

#define LOG_FATAL_I(instance, event)        LOG_I(instance, everlog::Severity::Fatal, event)
#define LOG_ERROR_I(instance, event)        LOG_I(instance, everlog::Severity::Error, event)
#define LOG_WARNING_I(instance, event)      LOG_I(instance, everlog::Severity::Warning, event)
#define LOG_INFO_I(instance, event)         LOG_I(instance, everlog::Severity::Info, event)
#define LOG_DEBUG_I(instance, event)        LOG_I(instance, everlog::Severity::Debug, event)
#define LOG_VERBOSE_I(instance, event)      LOG_I(instance, everlog::Severity::Verbose, event)


#define LOG(severity, event)                LOG_I(EVERLOG_DEFAULT_INSTANCE, severity, event)

#define LOG_FATAL(event)                    LOG(everlog::Severity::Fatal, event)
#define LOG_ERROR(event)                    LOG(everlog::Severity::Error, event)
#define LOG_WARNING(event)                  LOG(everlog::Severity::Warning, event)
#define LOG_INFO(event)                     LOG(everlog::Severity::Info, event)
#define LOG_DEBUG(event)                    LOG(everlog::Severity::Debug, event)
#define LOG_VERBOSE(event)                  LOG(everlog::Severity::Verbose, event)
