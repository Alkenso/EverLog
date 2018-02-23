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

namespace everlog
{
    namespace utils
    {
        template <int Instance>
        struct EverlogInstance {} ;
    }
    
    template <class T, template <class...> class Template>
    struct is_specialization : std::false_type {};
    
    template <template <typename...> class Template, typename... Args>
    struct is_specialization<Template<Args...>, Template> : std::true_type {};
}

#define EVERLOG_DECLARE_INSTANCE_(everlogType, instanceIdx) \
namespace everlog \
{ \
    namespace utils \
    { \
        template <> \
        struct EverlogInstance<instanceIdx> \
        { \
            static_assert(is_specialization<everlogType, Everlog>::value, "Pass only Everlog class specialization as everlogType"); \
            static everlogType instance; \
        }; \
        everlogType EverlogInstance<instanceIdx>::instance; \
    } \
}


#define EVERLOG_DEFAULT_INSTANCE 0

#define EVERLOG_DECLARE_DEFAULT(everlogType)                EVERLOG_DECLARE_INSTANCE_(everlogType, EVERLOG_DEFAULT_INSTANCE)
#define EVERLOG_DECLARE_INSTANCE(everlogType, instanceIdx)  EVERLOG_DECLARE_INSTANCE_(everlogType, instanceIdx)
