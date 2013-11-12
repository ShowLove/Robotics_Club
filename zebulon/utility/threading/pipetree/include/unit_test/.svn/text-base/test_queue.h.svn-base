////////////////////////////////////////////////////////////////////////////////////
///
/// \file test_queue.h
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
#include <pipetree/lockfree/queue.hpp>

#include <pthread.h>
#include <iostream>

namespace Test
{
    namespace Queue
    {
        volatile bool quit;
        pthread_mutex_t mutex;

        void wait(int millis)
        {
            pthread_mutex_t fakeMutex = PTHREAD_MUTEX_INITIALIZER;
            pthread_cond_t fakeCond = PTHREAD_COND_INITIALIZER;

            struct timespec timeToWait;
            struct timeval now;
            int rt;

            gettimeofday(&now,NULL);

            int secs = (now.tv_usec/1000 + millis)/1000;
            timeToWait.tv_sec = now.tv_sec + secs;
            timeToWait.tv_nsec = now.tv_usec*1000 + ((now.tv_usec/1000 + millis) % 1000) *1000;

            pthread_mutex_lock(&fakeMutex);
            rt = pthread_cond_timedwait(&fakeCond, &fakeMutex, &timeToWait);
            pthread_mutex_unlock(&fakeMutex);
        }

        void* RunThread(void* args)
        {
            PipeTree::Lockfree::Queue<int>* q = static_cast<PipeTree::Lockfree::Queue<int>*>(args);

            while (!quit)
            {
                int i = rand();
                pthread_mutex_lock(&mutex);
                std::cout << 'e' << i << std::endl;
                pthread_mutex_unlock(&mutex);

                q->push_back(i);

                wait(250 + rand()%250);

                int j=0;
                if (q->pop_front(j))
                {
                    pthread_mutex_lock(&mutex);
                    std::cout << 'd' << j << std::endl;
                    pthread_mutex_unlock(&mutex);
                }
            }
        }

        void Run(int numthreads)
        {
            PipeTree::Lockfree::Queue<int> queue;
            quit = false;

            /*int *k;

            queue.push_back(new int(1));
            queue.push_back(new int(2));
            queue.push_back(new int(3));
            queue.push_back(new int(4));

            while (true)
            {
                k=queue.pop_front();
                if (k)
                {
                    std::cout << *k << std::endl;
                    delete k;
                }
            }//*/

            /*long long int i=0;
            while (true)
            {
                std::cout << "Test " << i++ << std::endl;
                wait(500);
            }//*/

            pthread_t threads[numthreads];

            for (int i=0; i<numthreads; i++)
            {
                pthread_create(&threads[i], NULL, RunThread, &queue);
            }

            wait(2000);
            quit = true;//*/

            int j=0;
            while (queue.size() > 0)
            {
                if (queue.pop_front(j))
                {
                    pthread_mutex_lock(&mutex);
                    std::cout << 'd' << j << std::endl;
                    pthread_mutex_unlock(&mutex);
                }
            }
        }
    }
}

/* End of file */
