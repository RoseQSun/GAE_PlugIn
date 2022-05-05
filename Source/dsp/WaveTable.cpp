/*
  ==============================================================================

    WaveTable.cpp
    Created: 1 May 2022 12:30:18pm
    Author:  Rose Sun

  ==============================================================================
*/


#include "WaveTable.h"

// ========================================
// WaveTable
// ========================================

WaveTable::WaveTable() : isInitialized(false) {}

void WaveTable::initialize(int tableSize)
{
    if (isInitialized)
        return;

    table.setSize(1, tableSize);
    isInitialized = true;
}

float WaveTable::getSample(float phase) const
{
    if (!isInitialized)
        return 0.f;

    auto tableSize = table.getNumSamples();
    auto tablePointer = getTablePointer();

    // wrap the phase
    float wrappedPhase = phase - floor(phase);
    float index = wrappedPhase * static_cast<float>(tableSize);
    int indexFl = floor(index); float valFl = tablePointer[indexFl];
    int indexCe = ceil(index); indexCe = indexCe % tableSize; float valCe = tablePointer[indexCe];

    // linear interpolation for simplicity
    float value = valFl + (index - static_cast<float>(indexFl)) * (valCe - valFl);
    return value;
}

float WaveTable::getSample(const juce::dsp::Phase<float>& phase) const
{
    float fPhase = phase.phase / (2.f * juce::float_Pi);
    return getSample(fPhase);
}

const float* WaveTable::getTablePointer() const
{
    return table.getReadPointer(0);
}

int WaveTable::getSize() const
{
    return table.getNumSamples();
}

// ========================================
// Special Wave-tables
// ========================================

void SinWaveTable::initialize(int tableSize)
{
    WaveTable::initialize(tableSize);
    auto writePointer = table.getWritePointer(0);
    for (int i=0; i<tableSize; ++i)
        writePointer[i] = static_cast<float>(sin(2 * juce::float_Pi / tableSize * i));
}

void SawWaveTable::initialize(int tableSize)
{
    WaveTable::initialize(tableSize);
    auto writePointer = table.getWritePointer(0);
    int numRampUp = tableSize;
    for (int i=0; i<tableSize; ++i)
    {
        writePointer[i] = 1.f / static_cast<float>(numRampUp) * static_cast<float>(i);
        writePointer[i] = writePointer[i] * 2 - 1;
    }
}

void TriWaveTable::initialize(int tableSize)
{
    WaveTable::initialize(tableSize);
    auto writePointer = table.getWritePointer(0);
    int numRampUp = tableSize / 2 + 1;
    for (int i=0; i<tableSize; ++i)
    {
        writePointer[i] = i < numRampUp ? 1.f / static_cast<float>(numRampUp) * static_cast<float>(i) :
                          2.f - 1.f / static_cast<float>(numRampUp) * static_cast<float>(i);
        writePointer[i] = writePointer[i] * 2 - 1;
    }
}

void SqrWaveTable::initialize(int tableSize)
{
    WaveTable::initialize(tableSize);
    auto writePointer = table.getWritePointer(0);
    for (int i=0; i<tableSize; ++i)
        if (i < tableSize / 2)
            writePointer[i] = -1.f;
        else
            writePointer[i] = 1.f;
}

void EditableWaveTable::editTable(int startSample, const float* source, int numSamples)
{
    table.copyFrom(0, startSample, source, numSamples);
}
