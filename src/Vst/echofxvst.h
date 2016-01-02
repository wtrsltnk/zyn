#ifndef __ECHO_FX_VST_H__
#define __ECHO_FX_VST_H__

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "../Effects/Echo.h"

#define kNumPrograms 0

namespace EchoParams
{
enum
{
    Volume = 0,
    Panning,
    Delay,
    L_R_Delay,
    L_R_Crossover,
    Feedback,
    Dampening,

    kNumParams
};
}


class EchoFxVst : public AudioEffectX
{
public:
    EchoFxVst (audioMasterCallback audioMaster);
    ~EchoFxVst ();

    virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterLabel (VstInt32 index, char* label);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

    virtual void resume ();

    virtual bool getEffectName (char* name);
    virtual bool getVendorString (char* text);
    virtual bool getProductString (char* text);
    virtual VstInt32 getVendorVersion () { return 1000; }

    virtual VstPlugCategory getPlugCategory () { return kPlugCategEffect; }

protected:
    Echo* _effect;

    long size;
    float fTempo;
    float* _buffer[2];
};

#endif  // __ECHO_FX_VST_H__
