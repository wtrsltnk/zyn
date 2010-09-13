/*
  ZynAddSubFX - a software synthesizer

  ArrayControlHelper.cpp - A ControlHelper variant for arrays

  Copyright (C) 2010 Harald Hvaal

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#include "ArrayControlHelper.h"
#include <algorithm>
#include <cstring>
using namespace std;

ArrayControlHelper::ArrayControlHelper(QObject *parent)
    : ControlHelper(parent)
{

}

void ArrayControlHelper::connectedEvent(ConnEvent *ev)
{
    ControlHelper::connectedEvent(ev);
    if(ev->buf)
        emit arrayUpdated(ev->dat.f);
}

void ArrayControlHelper::disconnectedEvent()
{
}

void ArrayControlHelper::newValueEvent(NewValueEvent *event)
{
    //Could add a refence counting system instead of all of this silly allocation
    if(event->buf) {
        puts("ARRAYCONTROLHELPER GOT NEW BUFFER");
        size_t size = max(2048,max(SOUND_BUFFER_SIZE,OSCIL_SIZE));
        REALTYPE *tmp = new REALTYPE[size];
        memcpy(tmp,event->buf,size*sizeof(float));
        emit arrayUpdated(tmp);
        //gets deallocated on recieving end
    }
}

#include "ArrayControlHelper.moc"
// vim: sw=4 sts=4 et tw=100
