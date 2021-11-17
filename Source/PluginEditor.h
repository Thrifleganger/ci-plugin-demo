/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class CiPluginDemoAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    CiPluginDemoAudioProcessorEditor (CiPluginDemoAudioProcessor&);
    ~CiPluginDemoAudioProcessorEditor() override;
   
    void paint (Graphics&) override;
    void resized() override;

private:
    CiPluginDemoAudioProcessor& audioProcessor;
    Slider gainSlider;
    AudioProcessorValueTreeState::SliderAttachment gainAttachment;
    Label versionLabel;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CiPluginDemoAudioProcessorEditor)
};
