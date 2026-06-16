#include "PluginEditor.h"
#include "Parameters/ParameterIDs.h"
#include "Utils/Constants.h"

ChorusAudioProcessorEditor::ChorusAudioProcessorEditor (ChorusAudioProcessor& p)
    : AudioProcessorEditor (&p),
      processor (p),
      rateKnob     (ParameterIDs::rate,     p.apvts, "Rate"),
      depthKnob    (ParameterIDs::depth,    p.apvts, "Depth"),
      delayKnob    (ParameterIDs::delay,    p.apvts, "Delay"),
      feedbackKnob (ParameterIDs::feedback, p.apvts, "Feedback"),
      mixKnob      (ParameterIDs::mix,      p.apvts, "Mix")
{
    setLookAndFeel (&laf);
    setSize (Constants::kEditorWidth, Constants::kEditorHeight);

    for (auto* knob : { &rateKnob, &depthKnob, &delayKnob, &feedbackKnob, &mixKnob })
        addAndMakeVisible (knob);

    // Voices combo box
    voicesBox.addItemList ({ "1 Voice", "2 Voices", "3 Voices", "4 Voices" }, 1);
    addAndMakeVisible (voicesBox);
    voicesAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
                           (p.apvts, ParameterIDs::voices, voicesBox);

    // Stereo toggle
    addAndMakeVisible (stereoButton);
    stereoAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
                           (p.apvts, ParameterIDs::stereo, stereoButton);
}

ChorusAudioProcessorEditor::~ChorusAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

void ChorusAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (Constants::kColourBackground));

    g.setColour (juce::Colour (Constants::kColourAccent));
    g.setFont (18.0f);
    g.drawText ("Chorus", getLocalBounds().removeFromTop (40),
                juce::Justification::centred, true);
}

void ChorusAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (16);
    area.removeFromTop (40); // title

    const int knobW = Constants::kKnobSize;
    const int knobH = Constants::kKnobSize + Constants::kLabelHeight;
    const int spacing = (area.getWidth() - knobW * 5) / 4;

    auto knobRow = area.removeFromTop (knobH);

    auto placeKnob = [&] (RotaryKnob* k)
    {
        k->setBounds (knobRow.removeFromLeft (knobW));
        knobRow.removeFromLeft (spacing);
    };

    placeKnob (&rateKnob);
    placeKnob (&depthKnob);
    placeKnob (&delayKnob);
    placeKnob (&feedbackKnob);
    placeKnob (&mixKnob);

    area.removeFromTop (16);
    auto controlRow = area.removeFromTop (32);
    voicesBox    .setBounds (controlRow.removeFromLeft (120));
    controlRow.removeFromLeft (16);
    stereoButton.setBounds (controlRow.removeFromLeft (80));
}
