// --------------------------------------------------------------------------
// DISPLACE: DYNAMIC INDIVIDUAL VESSEL-BASED SPATIAL PLANNING
// AND EFFORT DISPLACEMENT
// Copyright (c) 2012, 2013, 2014, 2015, 2016 Francois Bastardie <fba@aqua.dtu.dk>

//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
// --------------------------------------------------------------------------

#ifndef OUTPUQUEUEMANAGER_H
#define OUTPUQUEUEMANAGER_H

#include <ipcqueue.h>

#include <pthread.h>
#include <semaphore.h>

#include <queue>
#include <list>
#include <memory>
#include <ostream>

#include <boost/shared_ptr.hpp>

class OutputMessage;

/** @brief A manager for the outputs from all the other simulator threads. */
class OutputQueueManager
{
public:
    explicit OutputQueueManager();

    void disableIpcQueue();
    void start();
    void finish();

    void lock() {
        pthread_mutex_lock(&mMutex);
    }

    void unlock() {
        pthread_mutex_unlock(&mMutex);
    }

    void enqueue(boost::shared_ptr<OutputMessage> msg);

private:
    /** @brief The pthread-style entry point to the thread. */
    static void *thread_trampoline(void *);

    struct ThreadArgs {
        OutputQueueManager *obj;
    };

    void *thread(ThreadArgs *args);


    /* == members == */

    int mThreadId;
    pthread_t mThread;
    pthread_mutex_t mMutex;
    sem_t mSemaphore;

    std::queue<boost::shared_ptr<OutputMessage> > mQueue;

    IpcQueue ipcQueue;
    enum ProtocolType { Binary, None } mType;
};

#endif // OUTPUQUEUEMANAGER_H
