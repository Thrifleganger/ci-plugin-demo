#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CiPluginDemoAudioProcessorEditor::CiPluginDemoAudioProcessorEditor (CiPluginDemoAudioProcessor& p) :
    AudioProcessorEditor (&p),
    audioProcessor (p),
    gainSlider(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox),
    gainAttachment(p.getApvts(), "gain", gainSlider),
    versionLabel("versionLabel", "v" + String{ ProjectInfo::versionString })
{
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(versionLabel);

    gainSlider.setPopupDisplayEnabled(true, false, this);
    gainSlider.setMouseCursor(MouseCursor::PointingHandCursor);
    gainSlider.setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::white);

    versionLabel.setFont(Font{ 12.f });
    versionLabel.setColour(Label::ColourIds::textColourId, Colours::white);
    versionLabel.setJustificationType(Justification::centredRight);

    setSize (300, 300);
    setResizable(true, true);
}

CiPluginDemoAudioProcessorEditor::~CiPluginDemoAudioProcessorEditor()
{
}

//==============================================================================
void CiPluginDemoAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setGradientFill(ColourGradient{ Colours::darkgrey, getLocalBounds().toFloat().getCentre(), Colours::darkgrey.darker(0.7f), {}, true });
    g.fillRect(getLocalBounds());
}

void CiPluginDemoAudioProcessorEditor::resized()
{
    const auto sliderWidth = getBounds().proportionOfWidth(0.4f);
    gainSlider.setBounds(getBounds().withSizeKeepingCentre(sliderWidth, sliderWidth));
    versionLabel.setBounds(getBounds().removeFromBottom(proportionOfHeight(0.1f)));
}
