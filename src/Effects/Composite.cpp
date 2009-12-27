#include "Composite.h"
#include "Reverb.h"

Composite::Composite(bool insertion_, REALTYPE *const efxoutl_,
        REALTYPE *const efxoutr_, FilterParams *filterpars_,
        const unsigned char &Ppreset_) :
    Effect(insertion_, efxoutl_, efxoutr_, filterpars_, Ppreset_)
{
    ef = new Reverb(insertion_, efxoutl_, efxoutr_);
    ef->outvolume = outvolume = volume = ef->volume = 10.0;
    ef->setpreset(1);
}

void Composite::setpreset(unsigned char npreset)
{

}

void Composite::changepar(const int &npar, const unsigned char &value)
{

}

unsigned char Composite::getpar(const int &npar) const
{

}

void Composite::out(REALTYPE *const smpsl, REALTYPE *const smpsr)
{
    ef->out(smpsl, smpsr);
}


// vim: sw=4 sts=4 et tw=100
