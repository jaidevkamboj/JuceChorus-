#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "../Utils/Constants.h"

/**
 * ChorusLookAndFeel
 *
 * Custom JUCE LookAndFeel v4 subclass.
 * Draws flat, rounded rotary sliders with an arc track and accent fill.
 */
class ChorusLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ChorusLookAndFeel();

    //==========================================================================
    void drawRotarySlider (juce::Graphics& g,
                           int x, int y, int width, int height,
                           float sliderPosProportional,
                           float rotaryStartAngle,
                           float rotaryEndAngle,
                           juce::Slider& slider) override;

    void drawLabel (juce::Graphics& g, juce::Label& label) override;

    juce::Font getLabelFont (juce::Label&) override;

private:
    juce::Colour backgroundColour { Constants::kColourBackground };
    juce::Colour accentColour     { Constants::kColourAccent };
    juce::Colour trackColour      { Constants::kColourKnobTrack };
    juce::Colour textColour       { Constants::kColourText };
};
