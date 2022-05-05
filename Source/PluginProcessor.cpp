/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FlangerPluginAudioProcessor::FlangerPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
parameters(*this, nullptr, juce::Identifier("FlangerPlugin"), {
    std::make_unique<juce::AudioParameterFloat>("mix", "Mix", juce::NormalisableRange<float>(0.f, 1.f, 0.f), 0.3f),
    std::make_unique<juce::AudioParameterFloat>("feedback", "Feedback", juce::NormalisableRange<float>(0.f, 1.f, 0.f), 0.3f),
    std::make_unique<juce::AudioParameterFloat>("delay", "Delay", juce::NormalisableRange<float>(0.f, 0.01f, 0.f), 0.003f),
    std::make_unique<juce::AudioParameterFloat>("lfoAmp", "LFOAmp", juce::NormalisableRange<float>(0.f, 0.005f, 0.f), 0.003f),
    std::make_unique<juce::AudioParameterFloat>("lfoFreq", "LFOFreq", juce::NormalisableRange<float>(0.f, 20.f, 0.f), 1.f),
})
{
    parameters.addParameterListener("mix", this);
    parameters.addParameterListener("feedback", this);
    parameters.addParameterListener("delay", this);
    parameters.addParameterListener("lfoAmp", this);
    parameters.addParameterListener("lfoFreq", this);
}

FlangerPluginAudioProcessor::~FlangerPluginAudioProcessor()
{
}

//==============================================================================
const juce::String FlangerPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FlangerPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FlangerPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FlangerPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FlangerPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FlangerPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FlangerPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FlangerPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FlangerPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void FlangerPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FlangerPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::uint32 numChannels = getTotalNumOutputChannels();
    processSpec = {sampleRate, static_cast<juce::uint32>(samplesPerBlock), numChannels};
    flangerModule.initialize(processSpec, 1.f);
    flangerModule.setDelayTime(parameters.getParameterAsValue("delay").getValue());
    flangerModule.setFeedback(parameters.getParameterAsValue("feedback").getValue());
    flangerModule.setMix(parameters.getParameterAsValue("mix").getValue());
    flangerModule.setLFOAmp(parameters.getParameterAsValue("lfoAmp").getValue());
    flangerModule.setLFOFreq(parameters.getParameterAsValue("lfoFreq").getValue());
}

void FlangerPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    flangerModule.reset();
    processSpec = {0.0, 0, 0};
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FlangerPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FlangerPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
//    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
//        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
//    for (int channel = 0; channel < totalNumInputChannels; ++channel)
//    {
//        auto* channelData = buffer.getWritePointer (channel);
//
//        // ..do something to the data...
//    }
    flangerModule.process(buffer);
}

//==============================================================================
bool FlangerPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FlangerPluginAudioProcessor::createEditor()
{
    return new FlangerPluginAudioProcessorEditor (*this);
}

//==============================================================================
void FlangerPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FlangerPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
juce::AudioProcessorValueTreeState& FlangerPluginAudioProcessor::getValueTreeState()
{
    return parameters;
}

void FlangerPluginAudioProcessor::parameterChanged (const juce::String &parameterID, float newValue)
{
    if (parameterID == "mix")
    {
        flangerModule.setMix(newValue);
    }
    else if (parameterID == "feedback")
    {
        flangerModule.setFeedback(newValue);
    }
    else if (parameterID == "delay")
    {
        flangerModule.setDelayTime(newValue);
    }
    else if (parameterID == "lfoAmp")
    {
        flangerModule.setLFOAmp(newValue);
    }
    else if (parameterID == "lfoFreq")
    {
        flangerModule.setLFOFreq(newValue);
    }
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FlangerPluginAudioProcessor();
}
