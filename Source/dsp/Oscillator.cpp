/*
  ==============================================================================

    Oscillator.cpp
    Created: 1 May 2022 12:28:18pm
    Author:  Rose Sun

  ==============================================================================
*/


#include "Oscillator.h"

WaveTableOscillator::WaveTableOscillator():
        isInitialized(false), table(nullptr), sampleRate(0)
{
    waveTableOscSpec.freqInHz = 0.f; waveTableOscSpec.gain = 1.f; waveTableOscSpec.shiftInCent = 0.f;
}

void WaveTableOscillator::initialize(const WaveTable *waveTable, float freqInHz, double sampleRate)
{
    table = waveTable;
    waveTableOscSpec.freqInHz = freqInHz;
    this->sampleRate = sampleRate;
    phase.reset();

    isInitialized = true;
}

void WaveTableOscillator::resetWaveform()
{
    phase.reset();
}

void WaveTableOscillator::reset()
{
    table = nullptr;
    waveTableOscSpec.freqInHz = 0.f; waveTableOscSpec.gain = 1.f; waveTableOscSpec.shiftInCent = 0.f;
    sampleRate = 0;
    phase.reset();

    isInitialized = false;
}

float WaveTableOscillator::getNextSample()
{
    if (!isInitialized || sampleRate == 0)
        return 0.f;

    // get a sample
    auto sample = table->getSample(phase) * waveTableOscSpec.gain;
    float freq = 0.f;
    if (waveTableOscSpec.freqInHz > 0.f)
    {
        float note = log(waveTableOscSpec.freqInHz/440.0)/log(2) * 12 + 69 + waveTableOscSpec.shiftInCent/100;
        freq = 440.f * pow(2.0, (note - 69)/12);
    }
    // advance the phase
    phase.advance(freq / static_cast<float>(sampleRate) * 2 * juce::float_Pi);

    return sample;
}

void WaveTableOscillator::getNextBlock(juce::AudioBuffer<float> &buffer)
{
    if (!isInitialized || sampleRate == 0)
        return;

    const auto numSamples = buffer.getNumSamples();
    auto writePointer = buffer.getWritePointer(0);
    for (int i=0; i<numSamples; ++i)
        writePointer[i] = getNextSample();
}

void WaveTableOscillator::getNextBlock(float *buffer, int numSamples)
{
    if (!isInitialized || sampleRate == 0)
        return;

    for (int i=0; i<numSamples; ++i)
        buffer[i] = getNextSample();
}

void WaveTableOscillator::setFrequency(float freqInHz)
{
    waveTableOscSpec.freqInHz = freqInHz;
}

float WaveTableOscillator::getFrequency() const
{
    return waveTableOscSpec.freqInHz;
}

void WaveTableOscillator::setGain(float gain)
{
    waveTableOscSpec.gain = gain;
}

float WaveTableOscillator::getGain() const
{
    return waveTableOscSpec.gain;
}

void WaveTableOscillator::shiftPitch(float cents)
{
    waveTableOscSpec.shiftInCent = cents;
}

float WaveTableOscillator::getPitchShift() const
{
    return waveTableOscSpec.shiftInCent;
}