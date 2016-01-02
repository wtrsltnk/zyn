#ifndef __CHORUS_FX_VST_H__
#define __CHORUS_FX_VST_H__

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "../Effects/Chorus.h"

#define kNumPrograms 0

namespace ChorusParams
{
enum
{
    Volume = 0,
    Panning,
    LFO_Frequency,
    LFO_Randomness,
    LFO_Type,
    LFO_stereo,
    Depth,
    Delay,
    Feedback,
    Flange_Mode,
    Subtractive,

    kNumParams
};
}


class ChorusFxVst : public AudioEffectX
{
public:
    ChorusFxVst (audioMasterCallback audioMaster);
    ~ChorusFxVst ();

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
    Chorus* _effect;

    long size;
    float fTempo;
    float* _buffer[2];
};

#endif  // __CHORUS_FX_VST_H__
