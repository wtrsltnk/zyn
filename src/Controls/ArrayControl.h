/*
  ZynAddSubFX - a software synthesizer

  Ranger.h - Control base class
  Copyright (C) 2009-2009 Mark McCurry
  Author: Harald Hvaal

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

#ifndef ARRAYCONTROL_H
#define ARRAYCONTROL_H

#include "GenControl.h"
#include "../globals.h"
#include <boost/function.hpp>

class ArrayControl : public GenControl
{
    public:
        ArrayControl(Node *parent, std::string id);
        ArrayControl(Node *parent, std::string id, boost::function1<size_t, REALTYPE *> func);
        virtual ~ArrayControl();

        virtual int getInt() const {return 0;}
        virtual void setInt(int) {}
        virtual std::string getString() const;
        virtual void defaults();
        void queueGetInt(){}
        void queueSetInt(int){}

        void damage();
    private:
        //It behaves like it has an array, but does not need one
        boost::function1<size_t, REALTYPE *> callback;
};

#endif // ARRAYCONTROL_H
