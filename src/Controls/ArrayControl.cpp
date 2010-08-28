#include "ArrayControl.h"
#include "EventClasses.h"
#include "../Misc/Util.h"
#include <cassert>
#include <stdio.h>

using std::string;


ArrayControl::ArrayControl(Node *parent, string id)
    :GenControl(parent, id)
{
}

ArrayControl::ArrayControl(Node *parent, string id, boost::function1<size_t, REALTYPE *> func)
    :GenControl(parent, id), callback(func)
{
}

ArrayControl::~ArrayControl()
{
}

string ArrayControl::getString() const
{
    return "dummy";
}

void ArrayControl::defaults()
{
}

void ArrayControl::redirHelper(NodeUser *dest)
{
    assert(dest);
    puts("ARRAYCONTROL CONNECTED!");
    REALTYPE *buf = getUiBuf();
    size_t s = callback(buf);
    ConnEvent tmp = ConnEvent(this,buf);
    dest->handleEvent(&tmp);
};
void ArrayControl::damage()
{
    puts("ARRAYCONTROL DAMAGED!");
    REALTYPE *buf = getUiBuf();
    size_t s = callback(buf);
    //returnUiBuf(buf);
    NewValueEvent tmp = NewValueEvent(this,buf);
    tmp.own();
    forward(&tmp);
}
// vim: sw=4 sts=4 et tw=100
