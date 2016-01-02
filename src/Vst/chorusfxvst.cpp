#include "chorusfxvst.h"

#include <stdio.h>
#include <string.h>

SYNTH_T  *synth;


AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new ChorusFxVst (audioMaster);
}


ChorusFxVst::ChorusFxVst (audioMasterCallback audioMaster)
    : AudioEffectX (audioMaster, NUM_CHORUS_PRESETS, ChorusParams::kNumParams)
{
    VstTimeInfo* vstTimeInfo = getTimeInfo(kVstTempoValid + kVstTransportPlaying);

    // init
    size = vstTimeInfo->sampleRate;
    fTempo = vstTimeInfo->tempo;

    this->_buffer[0] = new float[size];
    this->_buffer[1] = new float[size];
    this->_effect = new Chorus(false, this->_buffer[0], this->_buffer[1], size, size);

    setNumInputs (2);
    setNumOutputs (2);

    setUniqueID ('5884');

    resume ();
}


ChorusFxVst::~ChorusFxVst ()
{
    delete this->_effect;
    if (this->_buffer[0])
        delete[] this->_buffer[0];
    if (this->_buffer[1])
        delete[] this->_buffer[1];
}


void ChorusFxVst::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
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


void ChorusFxVst::setProgram (VstInt32 program)
{
    this->_effect->setpreset(program);
}


void ChorusFxVst::setProgramName (char *name)
{ }


void ChorusFxVst::getProgramName (char *name)
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

bool ChorusFxVst::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
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


void ChorusFxVst::resume ()
{
    AudioEffectX::resume ();
}


void ChorusFxVst::setParameter (VstInt32 index, float value)
{
    switch (index)
    {
    case ChorusParams::Subtractive: { this->_effect->changepar(index, unsigned char(value * 127)); break; }
    default: { this->_effect->changepar(index, unsigned char(value * 127)); break; }
    }
}


float ChorusFxVst::getParameter (VstInt32 index)
{
    switch (index)
    {
    case ChorusParams::Subtractive: { return this->_effect->getpar(index) == 0 ? 0.0f : 1.0f; }
    default: { return this->_effect->getpar(index) / 127.0f; }
    }
}


void ChorusFxVst::getParameterName (VstInt32 index, char *label)
{
    switch (index)
    {
    case ChorusParams::Volume: { strcpy(label, "Volume"); break; }
    case ChorusParams::Panning: { strcpy(label, "Panning"); break; }
    case ChorusParams::LFO_Frequency: { strcpy(label, "LFO Frequency"); break; }
    case ChorusParams::LFO_Randomness: { strcpy(label, "LFO Randomness"); break; }
    case ChorusParams::LFO_Type: { strcpy(label, "LFO Type"); break; }
    case ChorusParams::LFO_stereo: { strcpy(label, "LFO stereo"); break; }
    case ChorusParams::Depth: { strcpy(label, "Depth"); break; }
    case ChorusParams::Delay: { strcpy(label, "Delay"); break; }
    case ChorusParams::Feedback: { strcpy(label, "Feedback"); break; }
    case ChorusParams::Flange_Mode: { strcpy(label, "Flange Mode"); break; }
    case ChorusParams::Subtractive: { strcpy(label, "Subtractive"); break; }
    }
}


void ChorusFxVst::getParameterDisplay (VstInt32 index, char *text)
{
    switch (index)
    {
    case ChorusParams::Delay: { break; }
    case ChorusParams::Depth: { break; }
    case ChorusParams::Feedback: { break; }
    case ChorusParams::Flange_Mode: { break; }
    case ChorusParams::LFO_Frequency: { break; }
    case ChorusParams::LFO_Randomness: { break; }
    case ChorusParams::LFO_stereo: { break; }
    case ChorusParams::LFO_Type: { strcpy(text, this->_effect->getpar(index) == 0 ? "SINE" : "TRI"); break; }
    case ChorusParams::Panning: { strcpy(text, "db"); break; }
    case ChorusParams::Subtractive: { strcpy(text, this->_effect->getpar(index) == 0 ? "ON" : "OFF"); break; }
    case ChorusParams::Volume: { int2string(this->_effect->getpar(index), text, kVstMaxParamStrLen); break; }
    default: { strcpy(text, ""); break; }
    }
}


void ChorusFxVst::getParameterLabel (VstInt32 index, char *label)
{
    switch (index)
    {
    case ChorusParams::Delay: { break; }
    case ChorusParams::Depth: { break; }
    case ChorusParams::Feedback: { break; }
    case ChorusParams::Flange_Mode: { break; }
    case ChorusParams::LFO_Frequency: { break; }
    case ChorusParams::LFO_Randomness: { break; }
    case ChorusParams::LFO_stereo: { break; }
//    case ChorusParams::LFO_Type: { break; }
    case ChorusParams::Panning: { strcpy(label, "db"); break; }
//    case ChorusParams::Subtractive: { break; }
    case ChorusParams::Volume: { strcpy(label, "db"); break; }
    default: { strcpy(label, ""); break; }
    }
}


bool ChorusFxVst::getEffectName (char* name)
{
    strcpy (name, "Chorus");
    return true;
}


bool ChorusFxVst::getProductString (char* text)
{
    strcpy (text, "ZynAddSubFx Chorus");
    return true;
}


bool ChorusFxVst::getVendorString (char* text)
{
    strcpy (text, "ZynAddSubFx");
    return true;
}
