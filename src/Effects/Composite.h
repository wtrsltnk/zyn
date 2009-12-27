#ifndef EFFECTS_COMPOSITE_H
#define EFFECTS_COMPOSITE_H

#include "Effect.h"


class Composite : public Effect
{
    public:
        Composite(bool insertion_, REALTYPE *const efxoutl_,
               REALTYPE *const efxoutr_, FilterParams *filterpars_,
               const unsigned char &Ppreset_);

        virtual void setpreset(unsigned char npreset);
        virtual void changepar(const int &npar, const unsigned char &value);
        virtual unsigned char getpar(const int &npar) const;
        virtual void out(REALTYPE *const smpsl, REALTYPE *const smpsr);

    private:
        Effect *ef;

};

#endif // EFFECTS_COMPOSITE_H
