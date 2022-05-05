/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FlangerPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FlangerPluginAudioProcessorEditor (FlangerPluginAudioProcessor&);
    ~FlangerPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FlangerPluginAudioProcessor& audioProcessor;
    
    
    juce::AudioProcessorValueTreeState& parameters;

    juce::Slider mix;
    juce::Label mixLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;

    juce::Slider feedback;
    juce::Label feedbackLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;

    juce::Slider delay;
    juce::Label delayLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayAttachment;

    juce::Slider lfoAmp;
    juce::Label lfoAmpLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoAmpAttachment;

    juce::Slider lfoFreq;
    juce::Label lfoFreqLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoFreqAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlangerPluginAudioProcessorEditor)
};
