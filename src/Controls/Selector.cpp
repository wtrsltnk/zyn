/*
  ZynAddSubFX - a software synthesizer

  Control.cpp - Control template
  Copyright (C) 2009-2009 Mark McCurry
  Author: Mark McCurry

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
#include "Selector.h"
#include <cassert>

using namespace std;

//type,location,mutex
Selector::Selector(Node *parent, std::string id, int defaultval)
    :Control<int>(parent, id, defaultval)
{}

Selector::~Selector()
{}

string Selector::getString() const
{
    return getOption(getValue());
}

string Selector::getOption(int idx) const
{
    return options[idx];
}

int Selector::numOptions() const
{
    return options.size();
}

void Selector::addOption(string option)
{
    options.push_back(option);
}

void Selector::clearOptions()
{
    options.clear();
}

void Selector::redirHelper(NodeUser *dest)
{
    assert(dest);
    puts("SELECTOR CONNECTED!");
    ConnEvent tmp(this, (*this)(), (void *)&options);
    dest->handleEvent(&tmp);
};

void Selector::addpar(XMLwrapper *xml, const std::string& name)
{
    xml->addpar(name, getValue());
}

void Selector::getpar(XMLwrapper *xml, const std::string& name)
{
    setValue(xml->getpar127(name, getValue()));
}
