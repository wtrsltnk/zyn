/*
  ZynAddSubFX - a software synthesizer

  ADnoteParameters.cpp - Parameters for ADnote (ADsynth)
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "ADnoteParameters.h"
#include "FilterParams.h"
#include "EnvelopeParams.h"
#include "LFOParams.h"
#include "../Misc/db2rapInjFunc.h"
#include "../Misc/XMLwrapper.h"
#include "../DSP/FFTwrapper.h"
#include "../Synth/OscilGen.h"
#include "../Synth/Resonance.h"
#include "FilterParams.h"

int ADnote_unison_sizes[] =
{1, 2, 3, 4, 5, 6, 8, 10, 12, 15, 20, 25, 30, 40, 50, 0};

class VolumeConv:public InjFunction<int, REALTYPE>
{
    public:
        inline REALTYPE operator()(const int &x) const
        {return 4.0 * pow(0.1, 3.0 * (1.0 - x / 96.0)); }

        inline int operator()(const REALTYPE &x) const
        { return round(96 * (1 - (1 / (3 * log(0.1))) * (log(x) - log(4)))); }
};

class VoiceVolumeConv:public InjFunction<int, REALTYPE>
{
    public:
        inline REALTYPE operator()(const int &x) const
        {return pow(0.1, 3.0 * (1.0 - x / 127.0)); }

        inline int operator()(const REALTYPE &x) const
        { return round(127 * (1 - (1 / (3 * log(0.1))) * (log(x)))); }
};

ADnoteVoiceParam::ADnoteVoiceParam(Node *parent, std::string id, ADnoteParameters *par)
    :Node(parent, id),
      Enabled              (this, "enabled", false),
      type                 (this, "type", 0),
      delay                (this, "delay",0),
      resonance            (this, "Resonance",1),
      extoscil             (this, "ext_oscil",-1),
      extFMoscil           (this, "ext_fm_oscil",-1),
      oscilphase           (this, "oscil_phase",64),
      FMoscilphase         (this, "FMOscilPhase",64),
      filterBypass         (this, "filter_bypass",0),
      fixedFreq            (this, "fixed_freq",0),
      fixedFreqET          (this, "fixed_freq_et", 0),
      detuneSet            (this, "", 0, &par->GlobalPar.detune),
      freqEnvelopeEnabled  (this, "freq_envelope_enabled", 0),
      freqLfoEnabled       (this, "freq_lfo_enabled", 0),
      panning              (this, "panning", 64),
      volume               (this, "volume", 0.23, new VoiceVolumeConv),
      volumeMinus          (this, "volume_minus", 0),
      FMDetuneSet          (this, "FM", 0, &par->GlobalPar.detune)
{}

ADnoteParameters::ADnoteParameters(Node *parent, FFTwrapper *fft_)
    :PresetsArray(parent, "ADnoteParameters"),
     GlobalPar(this),
     voices(this, "Voices")
{
    setpresettype("Padsyth");
    fft = fft_;
    voices.addType("Voice");

    //TODO make allocation here of Voices
    for(int nvoice = 0; nvoice < NUM_VOICES; nvoice++) {
        VoicePar[nvoice] = new ADnoteVoiceParam(&voices, "VoiceParam", this);
        EnableVoice(nvoice);
        voices << VoicePar[nvoice];
        voices.createChild("Voice");
    }

    defaults();
}

ADnoteGlobalParam::ADnoteGlobalParam(Node *par)
      :volume(par, "volume", 10, new VolumeConv),
      //note: the original conversion functon is found in ADnote.cpp:337
      detune (par, "", 1),
      panning(par, "panning", 64)
{
    FreqEnvelope = new EnvelopeParams(par, "FREQUENCY_ENVELOPE", 0, 0);
    FreqEnvelope->ASRinit(64, 50, 64, 60);
    FreqLfo      = new LFOParams(par, "FREQUENCY_LFO", 70, 0, 64, 0, 0, 0, 0, 0);

    AmpEnvelope  = new EnvelopeParams(par, "AMPLITUDE_ENVELOPE", 64, 1);
    AmpEnvelope->ADSRinit_dB(0, 40, 127, 25);
    AmpLfo = new LFOParams(par, "AMPLITUDE_LFO", 80, 0, 64, 0, 0, 0, 0, 1);

    GlobalFilter   = new FilterParams(par, 2, 94, 40);
    FilterEnvelope = new EnvelopeParams(par, "FILTER_ENVELOPE", 0, 1);
    FilterEnvelope->ADSRinit_filter(64, 40, 64, 70, 60, 64);
    FilterLfo      = new LFOParams(par, "FILTER_LFO", 80, 0, 64, 0, 0, 0, 0, 2);
    Reson = new Resonance();
}

void ADnoteParameters::defaults()
{
    //Default Parameters
    GlobalPar.defaults();

    for(int nvoice = 0; nvoice < NUM_VOICES; nvoice++)
        defaults(nvoice);

    VoicePar[0]->Enabled = 1;
}

void ADnoteGlobalParam::defaults()
{
    /* Frequency Global Parameters */
    PStereo = 1; //stereo
    detune.defaults();
    FreqEnvelope->defaults();
    FreqLfo->defaults();
    PBandwidth = 64;

    /* Amplitude Global Parameters */
    volume.defaults();

    panning.defaults(); //center
    PAmpVelocityScaleFunction = 64;
    AmpEnvelope->defaults();
    AmpLfo->defaults();
    PPunchStrength = 0;
    PPunchTime     = 60;
    PPunchStretch  = 64;
    PPunchVelocitySensing = 72;
    Hrandgrouping  = 0;

    /* Filter Global Parameters*/
    PFilterVelocityScale = 64;
    PFilterVelocityScaleFunction = 64;
    GlobalFilter->defaults();
    FilterEnvelope->defaults();
    FilterLfo->defaults();
    Reson->defaults();
}

/*
 * Defaults a voice
 */
void ADnoteParameters::defaults(int n)
{
    VoicePar[n]->defaults();
}

void ADnoteVoiceParam::defaults()
{
    Enabled     = 0;

    Unison_size = 1;
    Unison_frequency_spread = 60;
    Unison_stereo_spread    = 64;
    Unison_vibratto = 64;
    Unison_vibratto_speed   = 64;
    Unison_invert_phase     = 0;

    type.defaults();
    
    fixedFreq.defaults();
    fixedFreqET.defaults();
    resonance.defaults();
    filterBypass.defaults();
    extoscil.defaults();
    extFMoscil.defaults();
    oscilphase.defaults();
    FMoscilphase.defaults();
    delay.defaults();
    volume.defaults();
    volumeMinus.defaults();
    panning.defaults();
    detuneSet.defaults();
    freqLfoEnabled.defaults();
    freqEnvelopeEnabled.defaults();
    PAmpEnvelopeEnabled       = 0;
    PAmpLfoEnabled            = 0;
    PAmpVelocityScaleFunction = 127;
    PFilterEnabled            = 0;
    PFilterEnvelopeEnabled    = 0;
    PFilterLfoEnabled         = 0;
    PFMEnabled                = 0;

    //I use the internal oscillator (-1)
    PFMVoice  = -1;

    PFMVolume = 90;
    PFMVolumeDamp   = 64;
    FMDetuneSet.defaults();
    //PFMDetune       = 8192;
    //PFMCoarseDetune = 0;
    //PFMDetuneType   = 0;
    PFMFreqEnvelopeEnabled   = 0;
    PFMAmpEnvelopeEnabled    = 0;
    PFMVelocityScaleFunction = 64;

    OscilSmp->defaults();
    FMSmp->defaults();

    AmpEnvelope->defaults();
    AmpLfo->defaults();

    FreqEnvelope->defaults();
    FreqLfo->defaults();

    VoiceFilter->defaults();
    FilterEnvelope->defaults();
    FilterLfo->defaults();

    FMFreqEnvelope->defaults();
    FMAmpEnvelope->defaults();
}


/*
 * Init the voice parameters
 */
void ADnoteParameters::EnableVoice(int nvoice)
{
    VoicePar[nvoice]->enable(fft, GlobalPar.Reson);
}

void ADnoteVoiceParam::enable(FFTwrapper *fft, Resonance *Reson)
{
    OscilSmp = new OscilGen(fft, Reson, this, "OSCIL");
    FMSmp    = new OscilGen(fft, NULL, this, "FMSmp");

    AmpEnvelope    = new EnvelopeParams(this, "AMPLITUDE_ENVELOPE", 64, 1);
    AmpEnvelope->ADSRinit_dB(0, 100, 127, 100);
    AmpLfo         = new LFOParams(this, "AMPLITUDE_LFO", 90, 32, 64, 0, 0, 30, 0, 1);

    FreqEnvelope   = new EnvelopeParams(this, "FREQUENCY_ENVELOPE", 0, 0);
    FreqEnvelope->ASRinit(30, 40, 64, 60);
    FreqLfo        = new LFOParams(this, "FREQUENCY_LFO", 50, 40, 0, 0, 0, 0, 0, 0);

    VoiceFilter    = new FilterParams(this, 2, 50, 60);
    FilterEnvelope = new EnvelopeParams(this, "FILTER_ENVELOPE", 0, 0);
    FilterEnvelope->ADSRinit_filter(90, 70, 40, 70, 10, 40);
    FilterLfo      = new LFOParams(this, "FILTER_LFO", 50, 20, 64, 0, 0, 0, 0, 2);

    FMFreqEnvelope = new EnvelopeParams(this, "FMFreqEnvelope", 0, 0);
    FMFreqEnvelope->ASRinit(20, 90, 40, 80);
    FMAmpEnvelope  = new EnvelopeParams(this, "FMAmpEnvelope",64, 1);
    FMAmpEnvelope->ADSRinit(80, 90, 127, 100);
}

/*
 * Get the Multiplier of the fine detunes of the voices
 */
REALTYPE ADnoteParameters::getBandwidthDetuneMultiplier()
{
    REALTYPE bw = (GlobalPar.PBandwidth - 64.0) / 64.0;
    return pow(2.0, bw * pow(fabs(bw), 0.2) * 5.0);
}

/*
 * Get the unison spread in cents for a voice
 */

REALTYPE ADnoteParameters::getUnisonFrequencySpreadCents(int nvoice) {
    REALTYPE unison_spread = VoicePar[nvoice]->Unison_frequency_spread / 127.0;
    unison_spread = pow(unison_spread * 2.0, 2.0) * 50.0; //cents
    return unison_spread;
}

/*
 * Kill the voice
 */
void ADnoteParameters::KillVoice(int nvoice)
{
    VoicePar[nvoice]->kill();
}

void ADnoteVoiceParam::kill()
{
    delete OscilSmp;
    delete FMSmp;

    delete AmpEnvelope;
    delete AmpLfo;

    delete FreqEnvelope;
    delete FreqLfo;

    delete VoiceFilter;
    delete FilterEnvelope;
    delete FilterLfo;

    delete FMFreqEnvelope;
    delete FMAmpEnvelope;
}


ADnoteGlobalParam::~ADnoteGlobalParam()
{
    delete FreqEnvelope;
    delete FreqLfo;
    delete AmpEnvelope;
    delete AmpLfo;
    delete GlobalFilter;
    delete FilterEnvelope;
    delete FilterLfo;
    delete Reson;
}

ADnoteParameters::~ADnoteParameters()
{
    for(int nvoice = 0; nvoice < NUM_VOICES; nvoice++)
        KillVoice(nvoice);
    
    for(int nvoice = 0; nvoice < NUM_VOICES; nvoice++)
        delete VoicePar[nvoice];
}

int ADnoteParameters::get_unison_size_index(int nvoice) {
    int index  = 0;
    if(nvoice >= NUM_VOICES)
        return 0;
    int unison = VoicePar[nvoice]->Unison_size;

    while(1) {
        if(ADnote_unison_sizes[index] >= unison)
            return index;

        if(ADnote_unison_sizes[index] == 0)
            return index - 1;

        index++;
    }
    return 0;
}

void ADnoteParameters::set_unison_size_index(int nvoice, int index) {
    int unison = 1;
    for(int i = 0; i <= index; i++) {
        unison = ADnote_unison_sizes[i];
        if(unison == 0) {
            unison = ADnote_unison_sizes[i - 1];
            break;
        }
    }

    VoicePar[nvoice]->Unison_size = unison;
}



void ADnoteParameters::add2XMLsection(XMLwrapper *xml, int n)
{
    int nvoice = n;
    if(nvoice >= NUM_VOICES)
        return;

    int oscilused = 0, fmoscilused = 0; //if the oscil or fmoscil are used by another voice

    for(int i = 0; i < NUM_VOICES; i++) {
        if(VoicePar[i]->extoscil() == nvoice)
            oscilused = 1;
        if(VoicePar[i]->extFMoscil() == nvoice)
            fmoscilused = 1;
    }

    xml->addparbool("enabled", VoicePar[nvoice]->Enabled());
    if(((VoicePar[nvoice]->Enabled() == 0) && (oscilused == 0)
        && (fmoscilused == 0)) && (xml->minimal))
        return;

    VoicePar[nvoice]->add2XML(xml,fmoscilused);
}

void ADnoteVoiceParam::add2XML(XMLwrapper *xml, bool fmoscilused)
{
    xml->addpar("type", type());

    xml->addpar("unison_size", Unison_size);
    xml->addpar("unison_frequency_spread",
                Unison_frequency_spread);
    xml->addpar("unison_stereo_spread", Unison_stereo_spread);
    xml->addpar("unison_vibratto", Unison_vibratto);
    xml->addpar("unison_vibratto_speed", Unison_vibratto_speed);
    xml->addpar("unison_invert_phase", Unison_invert_phase);

    xml->addpar("delay", delay());
    xml->addparbool("resonance", resonance());

    xml->addpar("ext_oscil", extoscil());
    xml->addpar("ext_fm_oscil", extFMoscil());

    xml->addpar("oscil_phase", oscilphase());
    xml->addpar("oscil_fm_phase", FMoscilphase());

    xml->addparbool("filter_enabled", PFilterEnabled);
    xml->addparbool("filter_bypass", filterBypass());

    xml->addpar("fm_enabled", PFMEnabled);

    xml->beginbranch("OSCIL");
    OscilSmp->add2XML(xml);
    xml->endbranch();


    xml->beginbranch("AMPLITUDE_PARAMETERS");
    xml->addpar("panning", panning());
    xml->addpar("volume", volume());
    xml->addparbool("volume_minus", volumeMinus());
    xml->addpar("velocity_sensing", PAmpVelocityScaleFunction);

    xml->addparbool("amp_envelope_enabled",
                    PAmpEnvelopeEnabled);
    if((PAmpEnvelopeEnabled != 0) || (!xml->minimal)) {
        xml->beginbranch("AMPLITUDE_ENVELOPE");
        AmpEnvelope->add2XML(xml);
        xml->endbranch();
    }
    xml->addparbool("amp_lfo_enabled", PAmpLfoEnabled);
    if((PAmpLfoEnabled != 0) || (!xml->minimal)) {
        xml->beginbranch("AMPLITUDE_LFO");
        AmpLfo->add2XML(xml);
        xml->endbranch();
    }
    xml->endbranch();

    xml->beginbranch("FREQUENCY_PARAMETERS");
    xml->addparbool("fixed_freq", fixedFreq());
    xml->addpar("fixed_freq_et", fixedFreqET());
    detuneSet.add2XMLsection(xml);

    xml->addparbool("freq_envelope_enabled",
                    freqEnvelopeEnabled());
    if((freqEnvelopeEnabled() != 0) || (!xml->minimal)) {
        xml->beginbranch("FREQUENCY_ENVELOPE");
        FreqEnvelope->add2XML(xml);
        xml->endbranch();
    }
    xml->addparbool("freq_lfo_enabled", freqLfoEnabled());
    if((freqLfoEnabled() != 0) || (!xml->minimal)) {
        xml->beginbranch("FREQUENCY_LFO");
        FreqLfo->add2XML(xml);
        xml->endbranch();
    }
    xml->endbranch();


    if((PFilterEnabled != 0) || (!xml->minimal)) {
        xml->beginbranch("FILTER_PARAMETERS");
        xml->beginbranch("FILTER");
        VoiceFilter->add2XML(xml);
        xml->endbranch();

        xml->addparbool("filter_envelope_enabled",
                        PFilterEnvelopeEnabled);
        if((PFilterEnvelopeEnabled != 0) || (!xml->minimal)) {
            xml->beginbranch("FILTER_ENVELOPE");
            FilterEnvelope->add2XML(xml);
            xml->endbranch();
        }

        xml->addparbool("filter_lfo_enabled",
                        PFilterLfoEnabled);
        if((PFilterLfoEnabled != 0) || (!xml->minimal)) {
            xml->beginbranch("FILTER_LFO");
            FilterLfo->add2XML(xml);
            xml->endbranch();
        }
        xml->endbranch();
    }

    if((PFMEnabled != 0) || (fmoscilused != 0)
       || (!xml->minimal)) {
        xml->beginbranch("FM_PARAMETERS");
        xml->addpar("input_voice", PFMVoice);

        xml->addpar("volume", PFMVolume);
        xml->addpar("volume_damp", PFMVolumeDamp);
        xml->addpar("velocity_sensing",
                    PFMVelocityScaleFunction);

        xml->addparbool("amp_envelope_enabled",
                        PFMAmpEnvelopeEnabled);
        if((PFMAmpEnvelopeEnabled != 0) || (!xml->minimal)) {
            xml->beginbranch("AMPLITUDE_ENVELOPE");
            FMAmpEnvelope->add2XML(xml);
            xml->endbranch();
        }
        xml->beginbranch("MODULATOR");
        FMDetuneSet.add2XMLsection(xml);

        xml->addparbool("freq_envelope_enabled",
                        PFMFreqEnvelopeEnabled);
        if((PFMFreqEnvelopeEnabled != 0) || (!xml->minimal)) {
            xml->beginbranch("FREQUENCY_ENVELOPE");
            FMFreqEnvelope->add2XML(xml);
            xml->endbranch();
        }

        xml->beginbranch("OSCIL");
        FMSmp->add2XML(xml);
        xml->endbranch();

        xml->endbranch();
        xml->endbranch();
    }
}

void ADnoteGlobalParam::add2XML(XMLwrapper *xml)
{
    xml->addparbool("stereo", PStereo);

    xml->beginbranch("AMPLITUDE_PARAMETERS");
    xml->addpar("volume", volume());
    xml->addpar("panning", panning());
    xml->addpar("velocity_sensing", PAmpVelocityScaleFunction);
    xml->addpar("punch_strength", PPunchStrength);
    xml->addpar("punch_time", PPunchTime);
    xml->addpar("punch_stretch", PPunchStretch);
    xml->addpar("punch_velocity_sensing", PPunchVelocitySensing);
    xml->addpar("harmonic_randomness_grouping", Hrandgrouping);

    xml->beginbranch("AMPLITUDE_ENVELOPE");
    AmpEnvelope->add2XML(xml);
    xml->endbranch();

    xml->beginbranch("AMPLITUDE_LFO");
    AmpLfo->add2XML(xml);
    xml->endbranch();
    xml->endbranch();

    xml->beginbranch("FREQUENCY_PARAMETERS");
    detune.add2XMLsection(xml);

    xml->addpar("bandwidth", PBandwidth);

    xml->beginbranch("FREQUENCY_ENVELOPE");
    FreqEnvelope->add2XML(xml);
    xml->endbranch();

    xml->beginbranch("FREQUENCY_LFO");
    FreqLfo->add2XML(xml);
    xml->endbranch();
    xml->endbranch();


    xml->beginbranch("FILTER_PARAMETERS");
    xml->addpar("velocity_sensing_amplitude", PFilterVelocityScale);
    xml->addpar("velocity_sensing", PFilterVelocityScaleFunction);

    xml->beginbranch("FILTER");
    GlobalFilter->add2XML(xml);
    xml->endbranch();

    xml->beginbranch("FILTER_ENVELOPE");
    FilterEnvelope->add2XML(xml);
    xml->endbranch();

    xml->beginbranch("FILTER_LFO");
    FilterLfo->add2XML(xml);
    xml->endbranch();
    xml->endbranch();

    xml->beginbranch("RESONANCE");
    Reson->add2XML(xml);
    xml->endbranch();
}

void ADnoteParameters::add2XML(XMLwrapper *xml)
{
    GlobalPar.add2XML(xml);
    for(int nvoice = 0; nvoice < NUM_VOICES; nvoice++) {
        xml->beginbranch("VOICE", nvoice);
        add2XMLsection(xml, nvoice);
        xml->endbranch();
    }
}


void ADnoteGlobalParam::getfromXML(XMLwrapper *xml)
{
    PStereo = xml->getparbool("stereo", PStereo);

    if(xml->enterbranch("AMPLITUDE_PARAMETERS")) {
        GETPAR(volume,"volume");
        GETPAR(panning,"panning");
        PAmpVelocityScaleFunction = xml->getpar127("velocity_sensing",
            PAmpVelocityScaleFunction);

        PPunchStrength = xml->getpar127("punch_strength", PPunchStrength);
        PPunchTime     = xml->getpar127("punch_time", PPunchTime);
        PPunchStretch  = xml->getpar127("punch_stretch", PPunchStretch);
        PPunchVelocitySensing = xml->getpar127("punch_velocity_sensing",
            PPunchVelocitySensing);
        Hrandgrouping  = xml->getpar127("harmonic_randomness_grouping",
            Hrandgrouping);

        if(xml->enterbranch("AMPLITUDE_ENVELOPE")) {
            AmpEnvelope->getfromXML(xml);
            xml->exitbranch();
        }

        if(xml->enterbranch("AMPLITUDE_LFO")) {
            AmpLfo->getfromXML(xml);
            xml->exitbranch();
        }

        xml->exitbranch();
    }

    if(xml->enterbranch("FREQUENCY_PARAMETERS")) {
        detune.getFromXMLsection(xml);
        PBandwidth    = xml->getpar127("bandwidth", PBandwidth);

        xml->enterbranch("FREQUENCY_ENVELOPE");
        FreqEnvelope->getfromXML(xml);
        xml->exitbranch();

        xml->enterbranch("FREQUENCY_LFO");
        FreqLfo->getfromXML(xml);
        xml->exitbranch();

        xml->exitbranch();
    }


    if(xml->enterbranch("FILTER_PARAMETERS")) {
        PFilterVelocityScale = xml->getpar127("velocity_sensing_amplitude",
            PFilterVelocityScale);
        PFilterVelocityScaleFunction = xml->getpar127("velocity_sensing",
            PFilterVelocityScaleFunction);

        xml->enterbranch("FILTER");
        GlobalFilter->getfromXML(xml);
        xml->exitbranch();

        xml->enterbranch("FILTER_ENVELOPE");
        FilterEnvelope->getfromXML(xml);
        xml->exitbranch();

        xml->enterbranch("FILTER_LFO");
        FilterLfo->getfromXML(xml);
        xml->exitbranch();
        xml->exitbranch();
    }

    if(xml->enterbranch("RESONANCE")) {
        Reson->getfromXML(xml);
        xml->exitbranch();
    }
}

void ADnoteParameters::getfromXML(XMLwrapper *xml)
{
    GlobalPar.getfromXML(xml);

    for(int nvoice = 0; nvoice < NUM_VOICES; nvoice++) {
        VoicePar[nvoice]->Enabled = 0;
        if(xml->enterbranch("VOICE", nvoice) == 0)
            continue;
        getfromXMLsection(xml, nvoice);
        xml->exitbranch();
    }
}

void ADnoteParameters::getfromXMLsection(XMLwrapper *xml, int n)
{
    int nvoice = n;
    if(nvoice >= NUM_VOICES)
        return;

    VoicePar[nvoice]->getfromXML(xml, nvoice);
}


void ADnoteVoiceParam::getfromXML(XMLwrapper *xml, unsigned nvoice)
{
    Enabled     = xml->getparbool("enabled", 0);
    Unison_size = xml->getpar127("unison_size", Unison_size);
    Unison_frequency_spread = xml->getpar127( "unison_frequency_spread",
        Unison_frequency_spread);
    Unison_stereo_spread    = xml->getpar127( "unison_stereo_spread",
            Unison_stereo_spread);
    Unison_vibratto = xml->getpar127( "unison_vibratto", Unison_vibratto);
    Unison_vibratto_speed   = xml->getpar127( "unison_vibratto_speed", Unison_vibratto_speed);
    Unison_invert_phase     = xml->getpar127( "unison_invert_phase", Unison_invert_phase);

    GETPAR(type,"type");
    GETPAR(delay,"delay");
    GETPAR(resonance,"resonance");

    extoscil.setValue(xml->getpar("ext_oscil", -1, -1, nvoice - 1));
    extFMoscil.setValue(xml->getpar("ext_fm_oscil", -1, -1, nvoice - 1));

    GETPAR(oscilphase, "oscil_phase");
    GETPAR(FMoscilphase, "oscil_fm_phase");
    PFilterEnabled = xml->getparbool("filter_enabled", PFilterEnabled);
    GETPAR(filterBypass, "filter_bypass");
    PFMEnabled     = xml->getpar127("fm_enabled", PFMEnabled);

    if(xml->enterbranch("OSCIL")) {
        OscilSmp->getfromXML(xml);
        xml->exitbranch();
    }


    if(xml->enterbranch("AMPLITUDE_PARAMETERS")) {
        GETPAR(panning,"panning");
        GETPAR(volume,"volume");
        GETPAR(volumeMinus, "volume_minus");
        PAmpVelocityScaleFunction = xml->getpar127("velocity_sensing", PAmpVelocityScaleFunction);

        PAmpEnvelopeEnabled = xml->getparbool("amp_envelope_enabled", PAmpEnvelopeEnabled);
        if(xml->enterbranch("AMPLITUDE_ENVELOPE")) {
            AmpEnvelope->getfromXML(xml);
            xml->exitbranch();
        }

        PAmpLfoEnabled = xml->getparbool("amp_lfo_enabled", PAmpLfoEnabled);
        if(xml->enterbranch("AMPLITUDE_LFO")) {
            AmpLfo->getfromXML(xml);
            xml->exitbranch();
        }
        xml->exitbranch();
    }

    if(xml->enterbranch("FREQUENCY_PARAMETERS")) {
        GETPAR(fixedFreq, "fixed_freq");
        GETPAR(fixedFreqET, "fixed_freq_et");
        detuneSet.getFromXMLsection(xml);
        freqEnvelopeEnabled = 
            xml->getparbool("freq_envelope_enabled", freqEnvelopeEnabled());

        if(xml->enterbranch("FREQUENCY_ENVELOPE")) {
            FreqEnvelope->getfromXML(xml);
            xml->exitbranch();
        }

        freqLfoEnabled = xml->getparbool("freq_lfo_enabled", freqLfoEnabled());

        if(xml->enterbranch("FREQUENCY_LFO")) {
            FreqLfo->getfromXML(xml);
            xml->exitbranch();
        }
        xml->exitbranch();
    }

    if(xml->enterbranch("FILTER_PARAMETERS")) {
        if(xml->enterbranch("FILTER")) {
            VoiceFilter->getfromXML(xml);
            xml->exitbranch();
        }

        PFilterEnvelopeEnabled = xml->getparbool("filter_envelope_enabled", PFilterEnvelopeEnabled);
        if(xml->enterbranch("FILTER_ENVELOPE")) {
            FilterEnvelope->getfromXML(xml);
            xml->exitbranch();
        }

        PFilterLfoEnabled = xml->getparbool("filter_lfo_enabled", PFilterLfoEnabled);
        if(xml->enterbranch("FILTER_LFO")) {
            FilterLfo->getfromXML(xml);
            xml->exitbranch();
        }
        xml->exitbranch();
    }

    if(xml->enterbranch("FM_PARAMETERS")) {
        PFMVoice = xml->getpar("input_voice", PFMVoice, -1, nvoice - 1);
        PFMVolume     = xml->getpar127("volume", PFMVolume);
        PFMVolumeDamp = xml->getpar127("volume_damp", PFMVolumeDamp);
        PFMVelocityScaleFunction = xml->getpar127("velocity_sensing", PFMVelocityScaleFunction);

        PFMAmpEnvelopeEnabled    = xml->getparbool("amp_envelope_enabled", PFMAmpEnvelopeEnabled);
        if(xml->enterbranch("AMPLITUDE_ENVELOPE")) {
            FMAmpEnvelope->getfromXML(xml);
            xml->exitbranch();
        }

        if(xml->enterbranch("MODULATOR")) {
            FMDetuneSet.getFromXMLsection(xml);

            PFMFreqEnvelopeEnabled = xml->getparbool("freq_envelope_enabled",
                    PFMFreqEnvelopeEnabled);
            if(xml->enterbranch("FREQUENCY_ENVELOPE")) {
                FMFreqEnvelope->getfromXML(xml);
                xml->exitbranch();
            }

            if(xml->enterbranch("OSCIL")) {
                FMSmp->getfromXML(xml);
                xml->exitbranch();
            }

            xml->exitbranch();
        }
        xml->exitbranch();
    }
}

