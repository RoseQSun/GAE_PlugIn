/*
  ==============================================================================

    Flanger.cpp
    Created: 1 May 2022 12:28:38pm
    Author:  Rose Sun

  ==============================================================================
*/

#include "Flanger.h"

Flanger::Flanger(): DSPModule(), maximumDelayInS(0.f)
{
    flangerSpec.mix = 0.f; flangerSpec.delayInS = 0.f; flangerSpec.feedback = 0.f;
    flangerSpec.delayInS = 0.f; flangerSpec.lfoAmp = 0.f; flangerSpec.lfoFreqInHz = 0.f;
}

Flanger::~Flanger()
{
    reset();
}


bool Flanger::initialize(const juce::dsp::ProcessSpec& spec, float maximumDelayInS)
{
    if (maximumDelayInS <= 0.f)
        return false;

    if (!DSPModule::initialize(spec))
        return false;

    int maximumDelayInSamples = ceil(spec.sampleRate * maximumDelayInS);
    pDelayLine = new juce::dsp::DelayLine<float>(maximumDelayInSamples);
    pDelayLine->prepare(spec);

    this->maximumDelayInS = maximumDelayInS;

    sinWaveTable.initialize(4096); // the wave-table only initializes itself once
    sinOscillator.initialize(&sinWaveTable,flangerSpec.lfoFreqInHz, spec.sampleRate);
    sinOscillator.setGain(flangerSpec.lfoAmp);
    return true;
}

void Flanger::clear()
{
    if (!isInitialized)
        return;
    // clear the delay lines
    pDelayLine->reset();
}

void Flanger::reset()
{
    if (!isInitialized)
        return;

    clear();

    // release memory
    // release memory
    delete pDelayLine;
    pDelayLine = nullptr;
    sinOscillator.reset();

    processSpec = {0.0, 0, 0};
    flangerSpec.mix = 0.f; flangerSpec.delayInS = 0.f; flangerSpec.feedback = 0.f;
    flangerSpec.delayInS = 0.f; flangerSpec.lfoAmp = 0.f; flangerSpec.lfoFreqInHz = 0.f;
    maximumDelayInS = 0.f;

    isInitialized = false;
}


void Flanger::process(juce::AudioBuffer<float> &buffer)
{
    auto numChannels = buffer.getNumChannels();
    auto numSamples = buffer.getNumSamples();
    auto ppBuffer = buffer.getArrayOfWritePointers();

    float delayInS;
    for (int i=0; i<numSamples; ++i)
    {
        delayInS = flangerSpec.delayInS;
        pDelayLine->setDelay((delayInS + sinOscillator.getNextSample()) * processSpec.sampleRate);
        for (int c=0; c<numChannels; ++c)
        {
            auto sampleInLine = pDelayLine->popSample(c);
            auto dry = ppBuffer[c][i];
            auto wet = ppBuffer[c][i] + sampleInLine * flangerSpec.feedback;
            pDelayLine->pushSample(c, wet);
            ppBuffer[c][i] = wet * flangerSpec.mix + dry * (1 - flangerSpec.mix);
        }
    }
}

void Flanger::setMix(float value)
{
    if (!isInitialized)
        return;
    if (value > 1.f)
        value = 1.f;
    else if (value < 0.f)
        value = 0.f;
    flangerSpec.mix = value;
}

void Flanger::setFeedback(float value)
{
    if (!isInitialized)
        return;
    if (value > 1.f)
        value = 1.f;
    else if (value < 0.f)
        value = 0.f;
    flangerSpec.feedback = value;
}

void Flanger::setDelayTime(float value)
{
    if (!isInitialized)
        return;
    if (value < 0.f)
        value = 0.f;
    flangerSpec.delayInS = value;
}


void Flanger::setLFOAmp(float value)
{
    if (!isInitialized)
        return;
    sinOscillator.setGain(value);
    flangerSpec.lfoAmp = value;
}

void Flanger::setLFOFreq(float value)
{
    if (!isInitialized)
        return;
    sinOscillator.setFrequency(value);
    flangerSpec.lfoFreqInHz = value;
}

