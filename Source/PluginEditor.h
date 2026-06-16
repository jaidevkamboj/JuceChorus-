#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"
#include "GUI/LookAndFeel.h"
#include "GUI/RotaryKnob.h"

class ChorusAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit ChorusAudioProcessorEditor (ChorusAudioProcessor& p);
    ~ChorusAudioProcessorEditor() override;

    //==========================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ChorusAudioProcessor& processor;
    ChorusLookAndFeel laf;

    RotaryKnob rateKnob,     depthKnob,    delayKnob,
               feedbackKnob, mixKnob;

    juce::ToggleButton stereoButton { "Stereo" };
    juce::ComboBox     voicesBox;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>  stereoAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> voicesAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChorusAudioProcessorEditor)
};
