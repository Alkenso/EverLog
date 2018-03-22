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

//namespace everlog
//{
//    static constexpr int kDefaultInstance = 0;
//    
//    template <int Instance>
//    class GlobalInstance {};
//    
//    using DefaultInstance = GlobalInstance<kDefaultInstance>;
//    
//    namespace utils
//    {
//        template <class T, template <class...> class Template>
//        struct is_specialization : std::false_type {};
//        
//        template <template <typename...> class Template, typename... Args>
//        struct is_specialization<Template<Args...>, Template> : std::true_type {};
//        
//        template <int Instance, typename EverlogType>
//        class InstanceHolder
//        {
//            static_assert(is_specialization<EverlogType, Everlog>::value, "Pass only Everlog class specialization as EverlogType");
//            
//        public:
//            static EverlogType& init(const everlog::Severity severity)
//            {
//                static std::once_flag s_once;
//                std::call_once(s_once, [severity]() { m_instance.reset(new EverlogType(severity)); });
//                return *m_instance;
//            }
//            
//            static EverlogType* get()
//            {
//                return m_instance.get();
//            }
//            
//        private:
//            static std::unique_ptr<EverlogType> m_instance;
//        };
//        
//        template<int Instance, typename EverlogType>
//        std::unique_ptr<EverlogType> InstanceHolder<Instance, EverlogType>::m_instance;
//    }
//}
//
//#define EVERLOG_DECLARE_INSTANCE_(EverlogType, instanceIdx) \
//namespace everlog \
//{ \
//    template <> class GlobalInstance<instanceIdx> : public utils::InstanceHolder<instanceIdx, EverlogType> {}; \
//}
//
//#define EVERLOG_DECLARE_DEFAULT(everlogType)                EVERLOG_DECLARE_INSTANCE_(everlogType, everlog::kDefaultInstance)
//#define EVERLOG_DECLARE_INSTANCE(everlogType, instanceIdx)  EVERLOG_DECLARE_INSTANCE_(everlogType, instanceIdx)
