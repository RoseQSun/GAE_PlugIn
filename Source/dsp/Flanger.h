/*
  ==============================================================================

    Flanger.h
    Created: 1 May 2022 12:28:38pm
    Author:  Rose Sun

  ==============================================================================
*/

#pragma once

#include "DSPModule.h"
#include "Oscillator.h"


class Flanger : public DSPModule
{
public:
    Flanger();
    ~Flanger() override;

    /**
     * Initializes the Delay object
     * @param spec
     * @param maximumDelayInS
     * @return
     */
    bool initialize(const juce::dsp::ProcessSpec& spec, float maximumDelayInS);
    void clear() override;
    void reset() override;
    void process(juce::AudioBuffer<float> &buffer) override;

    /**
     * Sets the FX mix
     * @param value
     */
    void setMix(float value);

    /**
     * Sets the delay feedback
     * @param value
     */
    void setFeedback(float value);

    /**
     * Sets the delay time
     * @param delayInS
     */
    void setDelayTime(float delayInS);

    void setLFOAmp(float lfoAmp);
    void setLFOFreq(float freqInHz);

private:
    struct FlangerSpec
    {
        std::atomic<float> mix; std::atomic<float> feedback; std::atomic<float> delayInS;
        std::atomic<float> lfoAmp; std::atomic<float> lfoFreqInHz;
    } flangerSpec;

    float maximumDelayInS;
//    Delay delayModule;
    juce::dsp::DelayLine<float>* pDelayLine;

    SinWaveTable sinWaveTable;
    WaveTableOscillator sinOscillator;

};