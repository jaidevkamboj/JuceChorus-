#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

/**
 * RotaryKnob
 *
 * A self-contained rotary slider + label + APVTS attachment.
 * Drop it into the editor, set its bounds, and it wires itself to a parameter.
 */
class RotaryKnob : public juce::Component
{
public:
    RotaryKnob (const juce::String& parameterID,
                juce::AudioProcessorValueTreeState& apvts,
                const juce::String& labelText);

    //==========================================================================
    void resized() override;

    juce::Slider& getSlider() noexcept { return slider; }

private:
    juce::Slider slider;
    juce::Label  label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryKnob)
};
