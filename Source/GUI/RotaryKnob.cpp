#include "RotaryKnob.h"
#include "../Utils/Constants.h"

RotaryKnob::RotaryKnob (const juce::String& paramID,
                         juce::AudioProcessorValueTreeState& apvts,
                         const juce::String& labelText)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 16);
    slider.setPopupDisplayEnabled (true, true, nullptr);
    addAndMakeVisible (slider);

    label.setText (labelText, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (label);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                     (apvts, paramID, slider);
}

void RotaryKnob::resized()
{
    auto bounds = getLocalBounds();
    label.setBounds (bounds.removeFromTop (Constants::kLabelHeight));
    slider.setBounds (bounds);
}
