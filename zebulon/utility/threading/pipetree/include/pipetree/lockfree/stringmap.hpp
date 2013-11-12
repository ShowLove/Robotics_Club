////////////////////////////////////////////////////////////////////////////////////
///
/// \file stringmap.hpp
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
#ifndef __PIPETREE_LOCKFREE_STRINGMAP_
#define __PIPETREE_LOCKFREE_STRINGMAP_

#include <typeinfo>
#include <pthread.h>
#include <string>

#include <boost/thread.hpp>

namespace PipeTree
{
    namespace Lockfree
    {
        ///
        /// \brief A templated lock-free map with a string to be used as a key
        ///         classes of type T must have a default constructor and have
        ///         overloaded the '=' operator
    template <class T>
        class StringMap
        {
        protected:
            struct Node
            {
                Node()
                {
                    key="";
                    free=true;
                }

                Node& operator= (const Node& param)
                {
                    this->key = param.key;
                    this->value = param.value;
                    this->free = param.free;
                    return *this;
                }

                std::string key;
                T value;
                bool free;
            };

        public:
            StringMap(uint32_t _capacity=13)
            {
                mCapacity = _capacity;
                mStorage = (Node**)calloc(mCapacity,sizeof(Node*));
                for (int i=0; i<mCapacity; i++)
                {
                    mStorage[i] = new Node();
                }
            }

            virtual ~StringMap()
            {
                free(mStorage);
            }

            bool put(const std::string& key, T value)
            {
                if (key == "")
                    return false;

                uint32_t h = Hash(key);

                int probe=0;
                int probefactor=0;

                Node* occupied=NULL;

                Node* n = new Node();
                n->key = key;
                n->value = value;
                n->free = false;

                int last=0;
                do
                {
                    do
                    {
                        occupied = mStorage[last = (h+probe) % mCapacity];
                        probe = pow(2, probefactor++);
                    } while (occupied->free != true && occupied->key != key && occupied->key != "");
                } while (!__sync_bool_compare_and_swap(&mStorage[last], occupied, n));
                delete occupied;

                return true;
            }

            bool get(const std::string& key, T& value) const
            {
                Node* occupied;
                uint32_t h = Hash(key);
                int probe = 0;
                int probefactor = 0;

                int capacitycheck=0;
                int last;
                do
                {
                    occupied = mStorage[last = (h+probe) % mCapacity]; // copy of the thing
                    probe = pow(2, probefactor++);

                    if (++capacitycheck > 2*mCapacity)
                    {
                        return false;
                    }
                } while (occupied->key != key);

                value= occupied->value;
                return true;
            }

            bool remove(const std::string& key)
            {
                Node* occupied=NULL;
                uint32_t h = Hash(key);
                int probe = 0;
                int probefactor = 0;

                int capacitycheck=0;
                do
                {
                    occupied = mStorage[(h+probe) % mCapacity];
                    probe = pow(2, ++probefactor);

                    if (++capacitycheck > 2*mCapacity)
                    {
                        return false;
                    }
                } while (occupied->key != key);

                Node* n = new Node();
                bool ret = __sync_bool_compare_and_swap(&occupied, occupied, n);
                if (ret)
                    delete occupied;
                else
                    delete n;

                return ret;
            }

            void reset()
            {
                for (int i=0; i<mCapacity; i++)
                {
                    mStorage[i]->value=T();
                    mStorage[i]->free=true;
                }
            }

            void clear()
            {
                for (int i=0; i<mCapacity; i++)
                {
                    Node* n = new Node();
                    Node* occupied = mStorage[i];
                    while (!__sync_bool_compare_and_swap(&occupied, occupied, n));
                    delete occupied;

                    /*mStorage[i]->value=T();
                    mStorage[i]->key="";
                    mStorage[i]->free=true;*/
                }
            }

            void resize(int _capacity)
            {
                /*Node** _storage = (Node**)calloc(_capacity,sizeof(Node*));
                for (int i=0; i<_capacity; i++)
                {
                    _storage[i] = new Node();
                }

                for (int i=0; i<mCapacity; i++)
                {

                }//*/
            }

        protected:
            Node** mStorage;
            int mCapacity;

            ///
            /// \brief Hash function based off of FNV-Hash
            ///         http://isthe.com/chongo/tech/comp/fnv/
            /// \param str The string to be hashed
            /// \return a value representing the hash code
            ///
            uint32_t Hash(const std::string& str) const
            {
                unsigned int hash = 2166136261; // 32-bit offset basis
                for (unsigned int i=0; i<str.size(); i++)
                {
                    hash = hash ^ str[i];
                    hash = hash * 16777619; // 32-bit prime number
                }
                return hash;
            }
        };
    }
}

#endif // __PIPETREE_LOCKFREE_STRINGMAP_
