////////////////////////////////////////////////////////////////////////////////////
///
///  \file circulararray.h
///  \brief This file contains a template for creating a fixed size Circular
///         Array.  This template is useful when speed counts and you only want
///         to store a maximum number of elements.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 20 March 2009
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
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
#ifndef __CXUTILS_CIRCULAR_ARRAY_H
#define __CXUTILS_CIRCULAR_ARRAY_H

#include <stdlib.h>
#include <assert.h>

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class CircularArray
    ///   \brief Template for creating circular arrays.  
    ///
    ///   These arrays are of a limited size and do not automatically re-size.  
    ///   When the end of the array is reached than the oldest data is replaced 
    ///   with the latest.
    ///
    ///   Depending on what direction is set, when indexing either the most
    ///   recent data (FILO) is at index 0, or the oldest is at index 0 (FIFO).  
    ///   By default data is read out FIFO.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    template <class T>
    class CircularArray
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class iterator
        ///   \brief Class for iterating through the circular array in a manner that is
        ///          familiar to those who use the STL.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class iterator
        {
            friend class CircularArray;
        public:
            iterator() : mpArray(0), mIndex(0) {}
            iterator(const iterator& itr) : mpArray(itr.mpArray), mIndex(itr.mIndex) {}
            ~iterator() {}
            T& operator->()
            {
                return *(*this);
            }
            T& operator*()
            {       
                int pos = 0;
                pos = ((int)(mpArray->mPosition)) - ((int)(mpArray->mElements)) + mIndex;
                if(pos < 0)
                {
                    pos += (int)mpArray->mReserved;
                }
                return mpArray->mpArray[pos];
            }
            iterator& operator=(const iterator& itr)
            {
                mpArray = itr.mpArray;
                mIndex = itr.mIndex;
                return *this;
            }
            iterator& operator++(int)
            {
                ++mIndex;
                return *this;
            }
            iterator& operator++()
            {
                mIndex++;
                return *this;
            }
            bool operator!=(const iterator& itr) const
            {
                if(mpArray == itr.mpArray && mIndex < itr.mIndex)
                {
                    return true;
                }
                return false;
            }
        private:
            CircularArray<T>* mpArray;
            unsigned int mIndex;
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class reverse_iterator
        ///   \brief Class for iterating through the circular array in a manner that is
        ///          familiar to those who use the STL.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class reverse_iterator
        {
            friend class CircularArray;
        public:
            reverse_iterator() : mpArray(0), mIndex(0) {}
            reverse_iterator(const reverse_iterator& itr) : mpArray(itr.mpArray), mIndex(itr.mIndex) {}
            ~reverse_iterator() {}
            T& operator->()
            {
                return *(*this);
            }
            T& operator*()
            {       
                int pos = 0;
                pos = ((int)(mpArray->mPosition))  - ((int)(mIndex)) - 1;
                if(pos < 0)
                {
                    pos += (int)mpArray->mReserved;
                }                    
                return mpArray->mpArray[pos];
            }
            reverse_iterator& operator=(const iterator& itr)
            {
                mpArray = itr.mpArray;
                mIndex = itr.mIndex;
                return *this;
            }
            reverse_iterator& operator++(int)
            {
                ++mIndex;
                return *this;
            }
            reverse_iterator& operator++()
            {
                mIndex++;
                return *this;
            }
            bool operator!=(const iterator& itr)
            {
                if(mpArray == itr.mpArray && mIndex < itr.mIndex)
                {
                    return true;
                }
                return false;
            }
        private:
            CircularArray<T>* mpArray;
            unsigned int mIndex;
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class const_iterator
        ///   \brief Class for iterating through the circular array in a manner that is
        ///          familiar to those who use the STL.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class const_iterator
        {
            friend class CircularArray;
        public:
            const_iterator() : mpArray(0), mIndex(0) {}
            const_iterator(const const_iterator& itr) : mpArray(itr.mpArray), mIndex(itr.mIndex) {}
            ~const_iterator() {}
            const T& operator->() const
            {
                return *(*this);
            }
            const T& operator*() const
            {       
                int pos = 0;
                pos = ((int)(mpArray->mPosition)) - ((int)(mpArray->mElements)) + mIndex;
                if(pos < 0)
                {
                    pos += (int)mpArray->mReserved;
                }
                return mpArray->mpArray[pos];
            }
            const_iterator& operator=(const iterator& itr)
            {
                mpArray = itr.mpArray;
                mIndex = itr.mIndex;
                return *this;
            }
            const_iterator& operator=(const const_iterator& itr)
            {
                mpArray = itr.mpArray;
                mIndex = itr.mIndex;
                return *this;
            }
            const_iterator& operator++(int)
            {
                ++mIndex;
                return *this;
            }
            const_iterator& operator++()
            {
                mIndex++;
                return *this;
            }
            bool operator!=(const iterator& itr)
            {
                if(mpArray == itr.mpArray && mIndex < itr.mIndex)
                {
                    return true;
                }
                return false;
            }
            bool operator!=(const const_iterator& itr)
            {
                if(mpArray == itr.mpArray && mIndex < itr.mIndex)
                {
                    return true;
                }
                return false;
            }
        private:
            CircularArray<T>* mpArray;
            unsigned int mIndex;
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class const_reverse_iterator
        ///   \brief Class for iterating through the circular array in a manner that is
        ///          familiar to those who use the STL.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class const_reverse_iterator
        {
            friend class CircularArray;
        public:
            const_reverse_iterator() : mpArray(0), mIndex(0) {}
            const_reverse_iterator(const const_reverse_iterator& itr) : mpArray(itr.mpArray), mIndex(itr.mIndex) {}
            ~const_reverse_iterator() {}
            const T& operator->() const
            {
                return *(*this);
            }
            const T& operator*() const
            {       
                int pos = 0;
                pos = ((int)(mpArray->mPosition))  - ((int)(mIndex)) - 1;
                if(pos < 0)
                {
                    pos += (int)mpArray->mReserved;
                }                    
                return mpArray->mpArray[pos];
            }
            const_reverse_iterator& operator=(const const_iterator& itr)
            {
                mpArray = itr.mpArray;
                mIndex = itr.mIndex;
                return *this;
            }
            const_reverse_iterator& operator++(int)
            {
                ++mIndex;
                return *this;
            }
            const_reverse_iterator& operator++()
            {
                mIndex++;
                return *this;
            }
            bool operator!=(const const_iterator& itr)
            {
                if(mpArray == itr.mpArray && mIndex < itr.mIndex)
                {
                    return true;
                }
                return false;
            }
        private:
            CircularArray<T>* mpArray;
            unsigned int mIndex;
        };
        CircularArray(const unsigned int size = 5);
        CircularArray(const CircularArray<T> &another);
        ~CircularArray();
        iterator begin()
        {
            iterator itr;
            itr.mpArray = (CircularArray<T> *)this;
            itr.mIndex = 0;
            return itr;
        }
        const_iterator begin() const
        {
            const_iterator itr;
            itr.mpArray = (CircularArray<T> *)this;
            itr.mIndex = 0;
            return itr;
        }
        iterator rbegin()
        {
            return begin();
        }
        const_iterator rbegin() const
        {
            return begin();
        }
        iterator end()
        {
            iterator itr;
            itr.mpArray = (CircularArray<T> *)this;
            itr.mIndex = mElements;
            return itr;
        }
        const_iterator end() const
        {
            const_iterator itr;
            itr.mpArray = (CircularArray<T> *)this;
            itr.mIndex = mElements;
            return itr;
        }
        iterator rend()
        {
            return end();
        }
        const_iterator rend() const
        {
            return end();
        }
        unsigned int GetFrontIndex() const { return mPosition; }
        void Clear();
        void Destroy();
        unsigned int Reserve(const unsigned int size);
        unsigned int Resize(const unsigned int size);
        unsigned int PushBack(const T &data);
        bool PopFront(T* data);
        bool PopBack(T* data);
        unsigned int Reserved() const;
        unsigned int Size() const;
        CircularArray<T> &operator=(const CircularArray<T> &another);
    protected:
        unsigned int mElements; ///<  Number of elements in the array.
        unsigned int mReserved; ///<  Amount of memory reserved.
        unsigned int mPosition; ///<  Position in the array.
        T *mpArray;             ///<  The circular array.
    };

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Constructor.
    ///
    ///   \param size Default size of array.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    template <class T>
    CircularArray<T>::CircularArray(const unsigned int size) : mElements(0),
                                                               mReserved(0),
                                                               mPosition(0),
                                                               mpArray(0)
    {
        Reserve(size);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Copy constructor.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    template <class T>
    CircularArray<T>::CircularArray(const CircularArray<T> &another) : mElements(0),
                                                                       mReserved(0),
                                                                       mPosition(0),
                                                                       mpArray(0)
    {
        *this = another;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Destructor.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    CircularArray<T>::~CircularArray()
    {
        Destroy();
    }


    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Clears contents, but does not delete memory.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    void CircularArray<T>::Clear()
    {
        mPosition = mElements = 0;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Deletes all allocated memory.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    void CircularArray<T>::Destroy()
    {
        if(mpArray)
            delete[] mpArray;
            
        mpArray = NULL;
        mElements = mReserved = mPosition = 0;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Reserve how large the array is.
    ///
    ///   \param size How large the array is.
    ///
    ///   \return 1 on ok, 0 on fail.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    unsigned int CircularArray<T>::Reserve(const unsigned int size)
    {
        T *ptr = NULL;
        //  Delete old data
        Destroy();

        ptr = new T[size];
        assert(ptr);

        mpArray = ptr;
        mReserved = size;
        mPosition = mElements = 0;

        return 1;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Resizes the array and preserves any existing data when possible.
    ///
    ///   Data is only preserved if you are increasing the size of the array.
    ///
    ///   \param size The new size of the array.
    ///
    ///   \return 1 on ok, 0 on fail.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    unsigned int CircularArray<T>::Resize(const unsigned int size)
    {
        if(size == mReserved)
            return 1;
        
        T *ptr = new T[size];
        assert(ptr);
        
        if(mElements < size)
        {
            //CircularArray<T> myT;
            //myT.Resize(1);
            
            typename CircularArray<T>::iterator itr;
            unsigned int i = 0;
            for(itr = this->begin(); itr != this->end(); itr++)
            {
                ptr[i++] = *itr;
            }
            mPosition = mElements;                        
        }
        else
        {
            mElements = mPosition = 0;
        }
        
        if(mpArray)
            delete[] mpArray;

        mpArray = ptr;
        mReserved = size;
        
        return 1;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Add data to the array.  
    ///
    ///   \param data Data to be added.
    ///
    ///   \return Number of elements in array.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    unsigned int CircularArray<T>::PushBack(const T &data)
    {
        if(!mpArray)
            return 0;
        // Add data to current position.
        mpArray[mPosition] = data;
        // Update write position in circular array.
        mPosition++;
        // Check for wrap around.
        if(mPosition >= mReserved)
            mPosition = 0;
        // Increase number of elements up to max.
        if(mElements < mReserved)
            mElements++;

        return mElements;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Remove data from the front of the array.
    ///
    ///   \param[out] data Copy of the data popped.  Set to NULL if you do not wish to
    ///                    get a copy.
    ///
    ///   \return Number of elements in array.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    bool CircularArray<T>::PopFront(T* data = NULL)
    {
        if(!mpArray)
            return false;

        if(mElements > 0)
        {
            if(data)
            {
                int pos = ((int)(mPosition)) - ((int)(mElements));
                if(pos < 0)
                {
                    pos += (int)mReserved;
                }           
                *data = mpArray[pos];
            }
            
            mElements--;
                
            return true;
        }
        return false;
    }
    
    
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Remove data from the back of the array.
    ///
    ///   \param[out] data Copy of the data popped.  Set to NULL if you do not wish to
    ///                    get a copy.
    ///
    ///   \return Number of elements in array.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    bool CircularArray<T>::PopBack(T* data = NULL)
    {
        if(!mpArray)
            return false;

        if(mElements > 0)
        {
            if(data)
            {
                int pos = ((int)(mPosition)) - 1;
                if(pos < 0)
                {
                    pos += (int)mReserved;
                }           
                *data = mpArray[pos];
            }
            
            mElements--;
            mPosition--;
            if(mPosition < 0)
            {
                mPosition += mReserved;
            }
            return true;
        }
        return false;
    }


    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \return The maximum size of the array.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    unsigned int CircularArray<T>::Reserved() const { return mReserved; }

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \return The number of elements in the array.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    unsigned int CircularArray<T>::Size() const { return mElements; }

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Copies data.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    CircularArray<T> &CircularArray<T>::operator=(const CircularArray<T> &another)
    {
        if(this != &another)
        {
            if(mReserved != another.mReserved)
            {
                Reserve(another.mReserved);
            }
            Clear();
            typename CircularArray<T>::const_iterator itr;
            for(itr = another.begin(); itr != another.end(); itr++)
            {
                PushBack(*itr);
            }
        }

        return *this;
    }
}

#endif

/* End of File */
