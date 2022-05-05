/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FlangerPluginAudioProcessorEditor::FlangerPluginAudioProcessorEditor (FlangerPluginAudioProcessor& p):
AudioProcessorEditor (&p),
audioProcessor (p),
parameters(p.getValueTreeState()),
mix {juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow},
feedback {juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow},
delay {juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow},
lfoAmp {juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow},
lfoFreq {juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow}
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    addAndMakeVisible(mix);
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.attachToComponent(&mix, false);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "mix", mix);

    addAndMakeVisible(feedback);
    feedbackLabel.setText("Feedback", juce::dontSendNotification);
    feedbackLabel.attachToComponent(&feedback, false);
    feedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "feedback", feedback);

    addAndMakeVisible(delay);
    delayLabel.setText("Delay", juce::dontSendNotification);
    delayLabel.attachToComponent(&delay, false);
    delayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "delay", delay);

    addAndMakeVisible(lfoAmp);
    lfoAmpLabel.setText("LFOAmp", juce::dontSendNotification);
    lfoAmpLabel.attachToComponent(&lfoAmp, false);
    lfoAmpAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "lfoAmp", lfoAmp);

    addAndMakeVisible(lfoFreq);
    lfoFreqLabel.setText("LFOFreq", juce::dontSendNotification);
    lfoFreqLabel.attachToComponent(&lfoFreq, false);
    lfoFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "lfoFreq", lfoFreq);
}

FlangerPluginAudioProcessorEditor::~FlangerPluginAudioProcessorEditor()
{
}

//==============================================================================
void FlangerPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void FlangerPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    mix.setBounds(50, 50, 70, 70);
    feedback.setBounds(150, 50, 70, 70);
    delay.setBounds(50, 150, 70, 70);
    lfoAmp.setBounds(150, 150, 70, 70);
    lfoFreq.setBounds(250, 150, 70, 70);
}
