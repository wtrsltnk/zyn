/*
  ZynAddSubFX - a software synthesizer

  PADnoteParameters.h - Parameters for PADnote (PADsynth)
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

#ifndef PAD_NOTE_PARAMETERS_H
#define PAD_NOTE_PARAMETERS_H

#include "../globals.h"

#include "Presets.h"
#include <string>
#include <functional>

/**
 * Parameters for PAD synthesis
 *
 * Note - unlike most other parameter objects significant portions of this
 * object are `owned' by the non-realtime context. The realtime context only
 * needs the samples generated by the PADsynth algorithm and modulators (ie
 * envelopes/filters/LFOs) for amplitude, frequency, and filters.
 * The ownership will be unclear for the time being, but it should be made more
 * explicit with time.
 */
class PADnoteParameters:public Presets
{
    public:
        PADnoteParameters(const SYNTH_T &synth_, FFTwrapper *fft_,
                          const AbsTime *time_ = nullptr);
        ~PADnoteParameters();

        void defaults();
        void add2XML(XMLwrapper& xml);
        void getfromXML(XMLwrapper& xml);

        void paste(PADnoteParameters &p);
        void pasteRT(PADnoteParameters &p);

        //returns a value between 0.0f-1.0f that represents the estimation perceived bandwidth
        float getprofile(float *smp, int size);

        //parameters

        //the mode: 0 - bandwidth, 1 - discrete (bandwidth=0), 2 - continous
        //the harmonic profile is used only on mode 0
        unsigned char Pmode;

        //Harmonic profile (the frequency distribution of a single harmonic)
        struct {
            struct { //base function
                unsigned char type;
                unsigned char par1;
            } base;
            unsigned char freqmult; //frequency multiplier of the distribution
            struct { //the modulator of the distribution
                unsigned char par1;
                unsigned char freq;
            } modulator;

            unsigned char width; //the width of the resulting function after the modulation
            struct { //the amplitude multiplier of the harmonic profile
                unsigned char mode;
                unsigned char type;
                unsigned char par1;
                unsigned char par2;
            } amp;
            bool autoscale; //if the scale of the harmonic profile is computed automaticaly
            unsigned char onehalf; //what part of the base function is used to make the distribution
        } Php;


        unsigned int  Pbandwidth; //the values are from 0 to 1000
        unsigned char Pbwscale; //how the bandwidth is increased according to the harmonic's frequency

        struct { //where are positioned the harmonics (on integer multimplier or different places)
            unsigned char type;
            unsigned char par1, par2, par3; //0..255
        } Phrpos;

        struct { //quality of the samples (how many samples, the length of them,etc.)
            unsigned char samplesize;
            unsigned char basenote, oct, smpoct;
        } Pquality;

        //frequency parameters
        //If the base frequency is fixed to 440 Hz
        unsigned char Pfixedfreq;

        /* Equal temperate (this is used only if the Pfixedfreq is enabled)
           If this parameter is 0, the frequency is fixed (to 440 Hz);
           if this parameter is 64, 1 MIDI halftone -> 1 frequency halftone */
        unsigned char PfixedfreqET;
        unsigned char PBendAdjust;
        unsigned char POffsetHz;
        unsigned short int PDetune; //fine detune
        unsigned short int PCoarseDetune; //coarse detune+octave
        unsigned char      PDetuneType; //detune type

        EnvelopeParams *FreqEnvelope; //Frequency Envelope
        LFOParams      *FreqLfo; //Frequency LFO

        //Amplitude parameters
        unsigned char PStereo;
        /* Panning -  0 - random
                  1 - left
                 64 - center
                127 - right */
        unsigned char PPanning;

        unsigned char PVolume;

        unsigned char PAmpVelocityScaleFunction;

        EnvelopeParams *AmpEnvelope;

        LFOParams *AmpLfo;

        /* Adjustment factor for anti-pop fadein */
        unsigned char Fadein_adjustment;

        unsigned char PPunchStrength, PPunchTime, PPunchStretch,
                      PPunchVelocitySensing;

        //Filter Parameters
        FilterParams *GlobalFilter;

        // filter velocity sensing
        unsigned char PFilterVelocityScale;

        // filter velocity sensing
        unsigned char PFilterVelocityScaleFunction;

        EnvelopeParams *FilterEnvelope;
        LFOParams      *FilterLfo;




        float setPbandwidth(int Pbandwidth); //returns the BandWidth in cents
        float getNhr(int n); //gets the n-th overtone position relatively to N harmonic

        void applyparameters(void);
        void applyparameters(std::function<bool()> do_abort);
        void export2wav(std::string basefilename);

        OscilGen  *oscilgen;
        Resonance *resonance;

        //RT sample data
        struct Sample {
            int    size;
            float  basefreq;
            float *smp;
        } sample[PAD_MAX_SAMPLES];

        typedef std::function<void(int,PADnoteParameters::Sample&)> callback;
        void sampleGenerator(PADnoteParameters::callback cb,
                             std::function<bool()> do_abort);

        const AbsTime *time;
        int64_t last_update_timestamp;

        static const rtosc::MergePorts ports;
        static const rtosc::Ports     &non_realtime_ports;
        static const rtosc::Ports     &realtime_ports;

    private:
        void generatespectrum_bandwidthMode(float *spectrum,
                                            int size,
                                            float basefreq,
                                            float *profile,
                                            int profilesize,
                                            float bwadjust);
        void generatespectrum_otherModes(float *spectrum,
                                         int size,
                                         float basefreq);
        void deletesamples();
        void deletesample(int n);

        FFTwrapper *fft;
    public:
        const SYNTH_T &synth;
};



#endif
