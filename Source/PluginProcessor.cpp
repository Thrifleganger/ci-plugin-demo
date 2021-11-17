/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CiPluginDemoAudioProcessor::CiPluginDemoAudioProcessor() :
    AudioProcessor(BusesProperties()
        .withInput("Input", AudioChannelSet::stereo(), true)
        .withOutput("Output", AudioChannelSet::stereo(), true)),
    parameters(*this, nullptr, "parameters", {
        std::make_unique<AudioParameterFloat>("gain", "Gain", NormalisableRange<float>{-60.f, 6.f, 0.01f, 1.f}, 0.f)
    })       
{
}

CiPluginDemoAudioProcessor::~CiPluginDemoAudioProcessor()
{
}

//==============================================================================
const String CiPluginDemoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CiPluginDemoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CiPluginDemoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CiPluginDemoAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CiPluginDemoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CiPluginDemoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CiPluginDemoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CiPluginDemoAudioProcessor::setCurrentProgram (int index)
{
}

const String CiPluginDemoAudioProcessor::getProgramName (int index)
{
    return {};
}

void CiPluginDemoAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void CiPluginDemoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    gain.reset(sampleRate, 0.1);
    gain.setCurrentAndTargetValue(Decibels::decibelsToGain(parameters.getRawParameterValue("gain")->load()));

    parameters.addParameterListener("gain", this);
}

void CiPluginDemoAudioProcessor::releaseResources()
{
    parameters.removeParameterListener("gain", this);
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CiPluginDemoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void CiPluginDemoAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;       
    gain.applyGain(buffer, buffer.getNumSamples());    
}

//==============================================================================
bool CiPluginDemoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CiPluginDemoAudioProcessor::createEditor()
{
    return new CiPluginDemoAudioProcessorEditor (*this);
}

//==============================================================================
void CiPluginDemoAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CiPluginDemoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

AudioProcessorValueTreeState& CiPluginDemoAudioProcessor::getApvts()
{
    return parameters;
}

void CiPluginDemoAudioProcessor::parameterChanged(const String& parameterID, float newValue)
{
    if (parameterID.equalsIgnoreCase("gain"))
    {
        gain.setTargetValue(Decibels::decibelsToGain(newValue));
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CiPluginDemoAudioProcessor();
}
