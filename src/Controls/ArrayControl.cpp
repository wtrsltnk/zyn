#include "ArrayControl.h"
#include "EventClasses.h"
#include "../Misc/Util.h"

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
