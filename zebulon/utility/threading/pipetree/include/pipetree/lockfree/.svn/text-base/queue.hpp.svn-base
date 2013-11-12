////////////////////////////////////////////////////////////////////////////////////
///
/// \file queue.hpp
///
/// Author(s): Michael Scherer<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Email: michael.scherer@ieee.org<br>
/// Web: http://zebulon.svn.sourceforge.net/viewvc/zebulon/v4/software/utility/threading/pipetree/<br>
/// All Rights Reserved <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
///  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
///  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
///  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
///  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
///  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
///  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
///  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
////////////////////////////////////////////////////////////////////////////////////
#ifndef __PIPETREE_LOCKFREE_QUEUE_
#define __PIPETREE_LOCKFREE_QUEUE_

namespace PipeTree
{
    namespace Lockfree
    {
        template <class T>
        class Queue
        {
        protected:
            //template <T>
            struct Node
            {
                Node()
                {
                    value=T();
                    next=NULL;
                }

                virtual ~Node() {}

                T value;
                Node* next;
            };

        public:
            Queue()
            {
                head = new Node();
                tail = &(head->next);
                mSize=0;
            }

            virtual ~Queue()
            {
                clear();
            }

            void clear()
            {
                T tmp;
                while (size() > 0)
                    pop_front(tmp);
            }

            void push_back(T object)
            {
                Node* node = new Node();
                node->value = object;
                node->next = NULL;


                while (true)
                {
                    //Node** prevTail = tail;
                    //Node* next = (*prevTail)->next;

                    if (__sync_bool_compare_and_swap(tail, NULL, node))
                    {
                        tail = &(node->next);

                        unsigned int _size=0;
                        do
                        {
                            _size = mSize;
                        } while(!__sync_bool_compare_and_swap(&mSize,_size,_size+1));

                        //__sync_bool_compare_and_swap(&tail, prevTail, &node->next);
                        return;
                    }
                }
            }

            bool pop_front(T& result)
            {
                Node* node = head;
                Node* next = node->next;
                if (next != NULL)
                {
                    T value = next->value;
                    if (__sync_bool_compare_and_swap(&head, node, next))
                    {
                        result = value;
                        delete node;

                        unsigned int _size;
                        do
                        {
                            _size = mSize;
                        } while(!__sync_bool_compare_and_swap(&mSize,_size,_size-1));

                        return true;
                    }
                }

                result = T();
                return false;
            }

            unsigned int size()
            {
                return mSize;
            }

        protected:
            Node* head;
            Node** tail;

            volatile unsigned int mSize;
        };
    }
}

#endif // __PIPETREE_LOCKFREE_QUEUE_
