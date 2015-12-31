#include "zynchorusfxvst.h"

#include <stdio.h>
#include <string.h>

SYNTH_T  *synth;

ZynChorusFxVst::ZynChorusFxVst (audioMasterCallback audioMaster)
    : AudioEffectX (audioMaster, NUM_CHORUS_PRESETS, kNumParams)
{
    VstTimeInfo* vstTimeInfo = getTimeInfo(kVstTempoValid + kVstTransportPlaying);

    // init
    size = vstTimeInfo->sampleRate;
    fTempo = vstTimeInfo->tempo;

    this->_buffer[0] = new float[size];
    this->_buffer[1] = new float[size];
    this->_chorus = new Chorus(false, 0, 0, size, size);

    setNumInputs (2);	// mono input
    setNumOutputs (2);	// stereo output

    setUniqueID ('5864');

    resume ();
}


ZynChorusFxVst::~ZynChorusFxVst ()
{
    if (this->_buffer[0])
        delete[] this->_buffer[0];
    if (this->_buffer[1])
        delete[] this->_buffer[1];
}

void ZynChorusFxVst::setProgram (VstInt32 program)
{
    this->_chorus->setpreset(program);
}


void ZynChorusFxVst::setProgramName (char *name)
{ }


void ZynChorusFxVst::getProgramName (char *name)
{
    switch (this->curProgram)
    {
    case 0: { strcpy(name, "Chorus1"); break; }
    case 1: { strcpy(name, "Chorus2"); break; }
    case 2: { strcpy(name, "Chorus3"); break; }
    case 3: { strcpy(name, "Celeste1"); break; }
    case 4: { strcpy(name, "Celeste2"); break; }
    case 5: { strcpy(name, "Flange1"); break; }
    case 6: { strcpy(name, "Flange2"); break; }
    case 7: { strcpy(name, "Flange3"); break; }
    case 8: { strcpy(name, "Flange4"); break; }
    case 9: { strcpy(name, "Flange5"); break; }
    }
}

bool ZynChorusFxVst::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    switch (index)
    {
    case 0: { strcpy(text, "Chorus1"); return true; }
    case 1: { strcpy(text, "Chorus2"); return true; }
    case 2: { strcpy(text, "Chorus3"); return true; }
    case 3: { strcpy(text, "Celeste1"); return true; }
    case 4: { strcpy(text, "Celeste2"); return true; }
    case 5: { strcpy(text, "Flange1"); return true; }
    case 6: { strcpy(text, "Flange2"); return true; }
    case 7: { strcpy(text, "Flange3"); return true; }
    case 8: { strcpy(text, "Flange4"); return true; }
    case 9: { strcpy(text, "Flange5"); return true; }
    }

    return false;
}


void ZynChorusFxVst::resume ()
{
    AudioEffectX::resume ();
}


void ZynChorusFxVst::setParameter (VstInt32 index, float value)
{
    switch (index)
    {
    case Volume: { this->_chorus->changepar(index, unsigned char(value * 127)); break; }
    case Panning: { this->_chorus->changepar(index, unsigned char(value * 127)); break; }
    default: { this->_chorus->changepar(index, unsigned char(value * 255)); break; }
    }
}


float ZynChorusFxVst::getParameter (VstInt32 index)
{
    switch (index)
    {
    case Volume: { return this->_chorus->getpar(index) / 127.0f; }
    case Panning: { return this->_chorus->getpar(index) / 127.0f; }
    }
    return this->_chorus->getpar(index) / 255.0f;
}


void ZynChorusFxVst::getParameterName (VstInt32 index, char *label)
{
    switch (index)
    {
    case Volume: { strcpy_s(label, 7, "Volume"); break; }
    case Panning: { strcpy_s(label, 8, "Panning"); break; }
    case LFO_Frequency: { strcpy_s(label, 14, "LFO Frequency"); break; }
    case LFO_Randomness: { strcpy_s(label, 15, "LFO Randomness"); break; }
    case LFO_Type: { strcpy_s(label, 9, "LFO Type"); break; }
    case LFO_stereo: { strcpy_s(label, 11, "LFO stereo"); break; }
    case Depth: { strcpy_s(label, 6, "Depth"); break; }
    case Delay: { strcpy_s(label, 6, "Delay"); break; }
    case Feedback: { strcpy_s(label, 9, "Feedback"); break; }
    case Flange_Mode: { strcpy_s(label, 12, "Flange Mode"); break; }
    case Subtractive: { strcpy_s(label, 12, "Subtractive"); break; }
    }
}


void ZynChorusFxVst::getParameterDisplay (VstInt32 index, char *text)
{
    switch (index)
    {
    case Volume: { int2string(this->_chorus->getpar(index), text, kVstMaxParamStrLen); break; }
    case LFO_Type: { strcpy(text, this->_chorus->getpar(index) == 0 ? "SINE" : "TRI"); break; }
    default: { strcpy(text, "text"); break; }
    }
}


void ZynChorusFxVst::getParameterLabel (VstInt32 index, char *label)
{
    switch (index)
    {
    case Volume: { strcpy(label, "db"); break; }
    default: { strcpy(label, "label"); break; }
    }
}


bool ZynChorusFxVst::getEffectName (char* name)
{
    strcpy (name, "Chorus");
    return true;
}


bool ZynChorusFxVst::getProductString (char* text)
{
    strcpy (text, "ZynAddSubFx Chorus");
    return true;
}


bool ZynChorusFxVst::getVendorString (char* text)
{
    strcpy (text, "ZynAddSubFx");
    return true;
}


void ZynChorusFxVst::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float *in1 = inputs[0];
    float *in2 = inputs[1];
    float *out1 = outputs[0];
    float *out2 = outputs[1];

    VstInt32 tmpSampleFrame = sampleFrames;
    while (--tmpSampleFrame >= 0)
    {
        (*out1++) = (*in1++);
        (*out2++) = (*in2++);
    }
}

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new ZynChorusFxVst (audioMaster);
}

