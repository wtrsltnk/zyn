/*
    AudioOut.cpp

    Copyright 2009, Alan Calvert

    This file is part of yoshimi, which is free software: you can
    redistribute it and/or modify it under the terms of the GNU General
    Public License as published by the Free Software Foundation, either
    version 3 of the License, or (at your option) any later version.

    yoshimi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with yoshimi.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <cstring>

using namespace std;

#include "../Misc/Master.h"
#include "AudioOut.h"

AudioOut::AudioOut(OutMgr *out)
    :samplerate(SAMPLE_RATE),bufferSize(SOUND_BUFFER_SIZE),
     usePartial(false),buffering(6),manager(out),enabled(false)
{
    pthread_mutex_init(&outBuf_mutex, NULL);
    pthread_cond_init (&outBuf_cv, NULL);
}

AudioOut::~AudioOut()
{
#warning TODO destroy other mutex
}

void AudioOut::out(Stereo<Sample> smps)
{
#warning TODO check for off by one errors
    pthread_mutex_lock(&outBuf_mutex);
    if(samplerate != SAMPLE_RATE) { //we need to resample
        smps.l().resample(SAMPLE_RATE,samplerate);
        smps.r().resample(SAMPLE_RATE,samplerate);
    }

    if(usePartial) { //we have a partial to use
        smps.l() = partialIn.l().append(smps.l());
        smps.r() = partialIn.r().append(smps.r());
    }

    if(smps.l().size() == bufferSize) { //sized just right
        outBuf.push(smps);
        usePartial = false;
        pthread_cond_signal(&outBuf_cv);
    }
    else if(smps.l().size() > bufferSize) { //store overflow
        outBuf.push(Stereo<Sample>(smps.l().subSample(0,bufferSize),
                                        smps.r().subSample(0,bufferSize)));
        partialIn = Stereo<Sample>(smps.l().subSample(bufferSize,smps.l().size()),
                                 smps.r().subSample(bufferSize,smps.r().size()));
        usePartial = true;
        pthread_cond_signal(&outBuf_cv);
    }
    else { //underflow
        partialIn = smps;
        usePartial = true;
    }
    pthread_mutex_unlock(&outBuf_mutex);
}

void AudioOut::setSamplerate(int _samplerate)
{
    pthread_mutex_lock(&outBuf_mutex);
    usePartial = false;
    samplerate = _samplerate;
    //hm, the queue does not have a clear
    while(!outBuf.empty())
        outBuf.pop();
    pthread_mutex_unlock(&outBuf_mutex);

}

void AudioOut::setBufferSize(int _bufferSize)
{
    pthread_mutex_lock(&outBuf_mutex);
    usePartial = false;
    bufferSize = _bufferSize;
    //hm, the queue does not have a clear
    while(!outBuf.empty())
        outBuf.pop();
    pthread_mutex_unlock(&outBuf_mutex);
};

void AudioOut::bufferingSize(int nBuffering)
{
   buffering = nBuffering;
}

int AudioOut::bufferingSize()
{
    return buffering;
}

const Stereo<Sample> AudioOut::getNext()
{
    const unsigned int BUFF_SIZE = buffering;
    Stereo<Sample> ans;
    pthread_mutex_lock(&outBuf_mutex);
    bool isEmpty = outBuf.empty();
    pthread_mutex_unlock(&outBuf_mutex);

    if(isEmpty)//fetch samples if possible
    {
        if((unsigned int)manager->getRunning() < BUFF_SIZE)
            manager->requestSamples(BUFF_SIZE-manager->getRunning());
        if(true)
            cout << "-----------------Starvation------------------"<< endl;
        return current;
    }
    else
    {
        pthread_mutex_lock(&outBuf_mutex);
        ans = outBuf.front();
        outBuf.pop();
        if(outBuf.size()+manager->getRunning()<BUFF_SIZE)
            manager->requestSamples(BUFF_SIZE - (outBuf.size()
                        + manager->getRunning()));
        if(false)
            cout << "AudioOut "<< outBuf.size()<< '+' << manager->getRunning() << endl;
        pthread_mutex_unlock(&outBuf_mutex);
    }
    current=ans;
    return ans;
}