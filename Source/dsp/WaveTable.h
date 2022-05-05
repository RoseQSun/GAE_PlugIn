/*
  ==============================================================================

    WaveTable.h
    Created: 1 May 2022 12:30:18pm
    Author:  Rose Sun

  ==============================================================================
*/

#pragma once


#include<JuceHeader.h>
// https://docs.juce.com/master/tutorial_wavetable_synth.html

class WaveTable
{
public:
    WaveTable();
    virtual ~WaveTable() = default;

    /**
     * Initializes the wave-table
     * @param tableSize the wave-table size
     */
    virtual void initialize(int tableSize);

    /**
     * Returns the sample according to the phase
     * @param phase
     * @return
     */
    float getSample(float phase) const;

    /**
     * Returns the sample according to the phase
     * @param phase
     * @return
     */
    float getSample(const juce::dsp::Phase<float>& phase) const;

    /**
     * Returns the table pointer
     * @return
     */
    const float* getTablePointer() const;

    /**
     * Returns the table size
     * @return
     */
    int getSize() const;

protected:
    bool isInitialized;
    juce::AudioBuffer<float> table;
};

class SinWaveTable : public WaveTable
{
public:
    SinWaveTable() = default;
    ~SinWaveTable() override = default;
    void initialize(int tableSize) override;
};

class SawWaveTable : public WaveTable
{
public:
    SawWaveTable() = default;
    ~SawWaveTable() override = default;
    void initialize(int tableSize) override;
};

class TriWaveTable : public WaveTable
{
public:
    TriWaveTable() = default;
    ~TriWaveTable() override = default;
    void initialize(int tableSize) override;
};

class SqrWaveTable : public WaveTable
{
public:
    SqrWaveTable() = default;
    ~SqrWaveTable() override = default;
    void initialize(int tableSize) override;
};

class EditableWaveTable : public WaveTable
{
public:
    EditableWaveTable() = default;
    ~EditableWaveTable() override = default;
    void editTable(int startSample, const float* source, int numSamples);
};