/*
  ==============================================================================

    DSPModule.h
    Created: 1 May 2022 12:26:27pm
    Author:  Rose Sun

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DSPModule
{
public:
    DSPModule() : isInitialized(false), processSpec({0, 0, 0}) {}

    virtual ~DSPModule() = default;

    /**
     * Returns true if the object is initialized.
     */
    bool getInitialized() const
    {
        return isInitialized;
    }

    /**
     * Processes the audio buffer
     * @param buffer
     */
    virtual void process(juce::AudioBuffer<float> &buffer) = 0;

    /**
     * Releases the memory and sets the object uninitialized.
     * Only resets when the object is initialized.
     */
    virtual void reset() = 0;

    /**
     * Clears the delay line and resets the internal state variables of the processor.
     * Only clears when the object is initialized.
     */
    virtual void clear() = 0;

protected:
    /**
     * Initializes the basic states of DSPModule. It should be called before the memory allocation
     * @param spec
     * @return
     */
    virtual bool initialize(const juce::dsp::ProcessSpec& spec)
    {
        reset();

        if (spec.numChannels < 0 || spec.maximumBlockSize < 0 || spec.sampleRate < 0)
            return false;

        isInitialized = true;
        processSpec = spec;
        return true;
    }

    bool isInitialized;
    juce::dsp::ProcessSpec processSpec;
};