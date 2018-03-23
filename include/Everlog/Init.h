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
    static constexpr int kDefaultInstance = 0;
    
    template <int Instance>
    class GlobalInstance;
    
    using DefaultInstance = GlobalInstance<kDefaultInstance>;
    
    namespace utils
    {
        template <int Instance, typename InstanceType, typename... InitArgs>
        class InstanceHolder
        {
        public:
            static InstanceType& init(InitArgs... args)
            {
                static std::once_flag s_once;
                std::call_once(s_once, [&args...]() { m_instance.reset(new InstanceType(args...)); });
                return *m_instance;
            }
            
            static InstanceType* get()
            {
                return m_instance.get();
            }
            
            /* When need to perform force release of global instance in exact moment of shutting down */
            static void destroy()
            {
                m_instance.reset();
            }
            
        private:
            static std::unique_ptr<InstanceType> m_instance;
        };
        
        template<int Instance, typename InstanceType, typename... InitArgs>
        std::unique_ptr<InstanceType> InstanceHolder<Instance, InstanceType, InitArgs...>::m_instance;
        
        
        template <bool UseSeverity, int Instance, typename EverlogType>
        class EverlogInstanceHolder;
        
        template <int Instance, typename EverlogType>
        class EverlogInstanceHolder<false, Instance, EverlogType>: public InstanceHolder<Instance, EverlogType>
        {};
        
        template <int Instance, typename EverlogType>
        class EverlogInstanceHolder<true, Instance, EverlogType>: public InstanceHolder<Instance, EverlogType, const Severity>
        {};
    }
}

#define EVERLOG_DECLARE_INSTANCE_(everlogType, instanceIdx, useSeverity) \
    template <> class everlog::GlobalInstance<instanceIdx> : public utils::EverlogInstanceHolder<useSeverity, instanceIdx, everlogType> {}

#define EVERLOG_DECLARE_INSTANCE(everlogType, instanceIdx)  EVERLOG_DECLARE_INSTANCE_(everlogType, instanceIdx, everlogType::UseSeverity)
#define EVERLOG_DECLARE_DEFAULT(everlogType)                EVERLOG_DECLARE_INSTANCE(everlogType, everlog::kDefaultInstance)
