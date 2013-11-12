////////////////////////////////////////////////////////////////////////////////////
///
/// \file hashtable.hpp
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

#include <typeinfo>
#include <pthread.h>

namespace PipeTree
{
    namespace Lockfree
    {
        //template <class Class>
        class HashTable
        {
        public:
            HashTable(uint32_t capacity=13)
            {
                storage = (void**)calloc(capacity, sizeof(void*));
            }

            ~HashTable()
            {
                free(storage);
            }

            void put(void* item)
            {
                if (item == NULL)
                    return;

                uint32_t h = hash(typeid(item).name());

                int probe=0;
                int probefactor=0;

                void* occupied;

                do
                {
                    do
                    {
                        occupied = storage[(h+probe) % capacity];
                        probe = pow(2, ++probefactor);
                    } while (typeid(occupied) != typeid(item));

                } while (!__sync_bool_compare_and_swap(&occupied, occupied, item));
            }

            template <class C>
            C* get()
            {
                C* empty=NULL;
                uint32_t h = hash(typeid(empty).name());

                do
                {
                    occupied = storage[(h+probe) % capacity];
                    probe = pow(2, ++probefactor);
                } while (typeid(occupied) != typeid(item));

                return occupied;
            }

            void clear()
            {
                for (int i=0; i<capacity; i++)
                    storage[i] = NULL;
            }

        protected:
            void** storage;
            int capacity;

            uint32_t hash(const char* str)
            {
                unsigned int hash = offset_basis;
                for (int i=0; str[i] != '\0'; i++)
                {
                    hash = hash ^ str[i];
                    hash = hash * 16777619;
                }
                return hash;
            }
        };
    }
}
