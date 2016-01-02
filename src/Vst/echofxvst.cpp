#include "echofxvst.h"

#include <stdio.h>
#include <string.h>

SYNTH_T  *synth;


AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new EchoFxVst (audioMaster);
}


EchoFxVst::EchoFxVst (audioMasterCallback audioMaster)
    : AudioEffectX (audioMaster, NUM_ECHO_PRESETS, EchoParams::kNumParams)
{
    VstTimeInfo* vstTimeInfo = getTimeInfo(kVstTempoValid + kVstTransportPlaying);

    // init
    size = vstTimeInfo->sampleRate;
    fTempo = vstTimeInfo->tempo;

    this->_buffer[0] = new float[size];
    this->_buffer[1] = new float[size];
    this->_effect = new Echo(false, this->_buffer[0], this->_buffer[1], size, size);

    setNumInputs (2);
    setNumOutputs (2);

    setUniqueID ('5883');

    resume ();
}


EchoFxVst::~EchoFxVst ()
{
    delete this->_effect;
    if (this->_buffer[0])
        delete[] this->_buffer[0];
    if (this->_buffer[1])
        delete[] this->_buffer[1];
}


void EchoFxVst::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    this->_effect->out(Stereo<float*>(inputs[0], inputs[1]), sampleFrames);

    float *in1 = this->_buffer[0];
    float *in2 = this->_buffer[1];
    float *out1 = outputs[0];
    float *out2 = outputs[1];

    VstInt32 tmp = sampleFrames;
    while (--tmp >= 0)
    {
        (*out1++) = (*in1++);
        (*out2++) = (*in2++);
    }
}

void EchoFxVst::setProgram (VstInt32 program)
{
    this->_effect->setpreset(program);
}


void EchoFxVst::setProgramName (char *name)
{ }


void EchoFxVst::getProgramName (char *name)
{
    switch (this->curProgram)
    {
    case 0: { strcpy(name, "Echo 1"); break; }
    case 1: { strcpy(name, "Echo 2"); break; }
    case 2: { strcpy(name, "Echo 3"); break; }
    case 3: { strcpy(name, "Simple Echo"); break; }
    case 4: { strcpy(name, "Canyon"); break; }
    case 5: { strcpy(name, "Panning Echo 1"); break; }
    case 6: { strcpy(name, "Panning Echo 2"); break; }
    case 7: { strcpy(name, "Panning Echo 3"); break; }
    case 8: { strcpy(name, "Feedback Echo"); break; }
    }
}

bool EchoFxVst::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    switch (index)
    {
    case 0: { strcpy(text, "Echo 1"); return true; }
    case 1: { strcpy(text, "Echo 2"); return true; }
    case 2: { strcpy(text, "Echo 3"); return true; }
    case 3: { strcpy(text, "Simple Echo"); return true; }
    case 4: { strcpy(text, "Canyon"); return true; }
    case 5: { strcpy(text, "Panning Echo 1"); return true; }
    case 6: { strcpy(text, "Panning Echo 2"); return true; }
    case 7: { strcpy(text, "Panning Echo 3"); return true; }
    case 8: { strcpy(text, "Feedback Echo"); return true; }
    }

    return false;
}


void EchoFxVst::resume ()
{
    AudioEffectX::resume ();
}


void EchoFxVst::setParameter (VstInt32 index, float value)
{
    switch (index)
    {
    case EchoParams::Volume: { this->_effect->changepar(index, unsigned char(value * 127)); break; }
    case EchoParams::Panning: { this->_effect->changepar(index, unsigned char(value * 127)); break; }
    default: { this->_effect->changepar(index, unsigned char(value * 255)); break; }
    }
}


float EchoFxVst::getParameter (VstInt32 index)
{
    switch (index)
    {
    case EchoParams::Volume: { return this->_effect->getpar(index) / 127.0f; }
    case EchoParams::Panning: { return this->_effect->getpar(index) / 127.0f; }
    }
    return this->_effect->getpar(index) / 255.0f;
}


void EchoFxVst::getParameterName (VstInt32 index, char *label)
{
    switch (index)
    {
    case EchoParams::Dampening: { strcpy(label, "Dampening"); break; }
    case EchoParams::Delay: { strcpy(label, "Delay"); break; }
    case EchoParams::Feedback: { strcpy(label, "Feedback"); break; }
    case EchoParams::L_R_Crossover: { strcpy(label, "L/R Crossover"); break; }
    case EchoParams::L_R_Delay: { strcpy(label, "L/R Delay"); break; }
    case EchoParams::Panning: { strcpy(label, "Panning"); break; }
    case EchoParams::Volume: { strcpy(label, "Volume"); break; }
    }
}


void EchoFxVst::getParameterDisplay (VstInt32 index, char *text)
{
    switch (index)
    {
    case EchoParams::Volume: { int2string(this->_effect->getpar(index), text, kVstMaxParamStrLen); break; }
    default: { strcpy(text, "text"); break; }
    }
}


void EchoFxVst::getParameterLabel (VstInt32 index, char *label)
{
    switch (index)
    {
    case EchoParams::Volume: { strcpy(label, "db"); break; }
    default: { strcpy(label, "label"); break; }
    }
}


bool EchoFxVst::getEffectName (char* name)
{
    strcpy (name, "Echo");
    return true;
}


bool EchoFxVst::getProductString (char* text)
{
    strcpy (text, "ZynAddSubFx Echo");
    return true;
}


bool EchoFxVst::getVendorString (char* text)
{
    strcpy (text, "ZynAddSubFx");
    return true;
}

