//MIT License
//
//Copyright (c) 2017 Mindaugas Vinkelis
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.


#ifndef BITSERY_EXT_STD_QUEUE_H
#define BITSERY_EXT_STD_QUEUE_H

#include <type_traits>
#include <queue>
//include type traits for deque and vector, because they are defaults for queue and priority_queue
#include "../traits/deque.h"
#include "../traits/vector.h"

namespace bitsery {
    namespace ext {

        class StdQueue {
        private:
            //inherit from queue so we could take underlying container
            template<typename T, typename C>
            struct QueueCnt : public std::queue<T, C> {
                static const C &getContainer(const std::queue<T, C> &s) {
                    //get address of underlying container
                    return s.*(&QueueCnt::c);
                }

                static C &getContainer(std::queue<T, C> &s) {
                    //get address of underlying container
                    return s.*(&QueueCnt::c);
                }
            };

            //inherit from queue so we could take underlying container
            template<typename T, typename C>
            struct PriorityQueueCnt : public std::priority_queue<T, C> {
                static const C &getContainer(const std::priority_queue<T, C> &s) {
                    //get address of underlying container
                    return s.*(&PriorityQueueCnt::c);
                }

                static C &getContainer(std::priority_queue<T, C> &s) {
                    //get address of underlying container
                    return s.*(&PriorityQueueCnt::c);
                }
            };

            size_t _maxSize;
        public:
            explicit StdQueue(size_t maxSize) : _maxSize{maxSize} {
            };

            //for queue
            template<typename Ser, typename Writer, typename T, typename C, typename Fnc>
            void serialize(Ser &ser, Writer &, const std::queue<T, C> &obj, Fnc &&fnc) const {
                ser.container(QueueCnt<T, C>::getContainer(obj), _maxSize, std::forward<Fnc>(fnc));
            }

            template<typename Des, typename Reader, typename T, typename C, typename Fnc>
            void deserialize(Des &des, Reader &, std::queue<T, C> &obj, Fnc &&fnc) const {
                des.container(QueueCnt<T, C>::getContainer(obj), _maxSize, std::forward<Fnc>(fnc));
            }

            //for priority_queue
            template<typename Ser, typename Writer, typename T, typename C, typename Fnc>
            void serialize(Ser &ser, Writer &, const std::priority_queue<T, C> &obj, Fnc &&fnc) const {
                ser.container(PriorityQueueCnt<T, C>::getContainer(obj), _maxSize, std::forward<Fnc>(fnc));
            }

            template<typename Des, typename Reader, typename T, typename C, typename Fnc>
            void deserialize(Des &des, Reader &, std::priority_queue<T, C> &obj, Fnc &&fnc) const {
                des.container(PriorityQueueCnt<T, C>::getContainer(obj), _maxSize, std::forward<Fnc>(fnc));
            }

        };
    }

    namespace traits {
        template<typename T>
        struct ExtensionTraits<ext::StdQueue, T> {
            using TValue = typename T::value_type;
            static constexpr bool SupportValueOverload = true;
            static constexpr bool SupportObjectOverload = true;
            static constexpr bool SupportLambdaOverload = true;
        };
    }

}


#endif //BITSERY_EXT_STD_QUEUE_H
