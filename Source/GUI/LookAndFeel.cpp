#include "LookAndFeel.h"

ChorusLookAndFeel::ChorusLookAndFeel()
{
    setColour (juce::Slider::rotarySliderFillColourId,  accentColour);
    setColour (juce::Slider::rotarySliderOutlineColourId, trackColour);
    setColour (juce::Slider::thumbColourId,             accentColour);
    setColour (juce::Label::textColourId,               textColour);
    setColour (juce::ResizableWindow::backgroundColourId, backgroundColour);
}

void ChorusLookAndFeel::drawRotarySlider (juce::Graphics& g,
                                           int x, int y, int width, int height,
                                           float sliderPos,
                                           float startAngle, float endAngle,
                                           juce::Slider&)
{
    const float radius  = static_cast<float> (juce::jmin (width, height)) * 0.5f - 4.0f;
    const float centreX = static_cast<float> (x) + static_cast<float> (width)  * 0.5f;
    const float centreY = static_cast<float> (y) + static_cast<float> (height) * 0.5f;
    const float strokeW = radius * 0.15f;

    // Background track
    juce::Path trackPath;
    trackPath.addArc (centreX - radius, centreY - radius,
                      radius * 2.0f, radius * 2.0f,
                      startAngle, endAngle, true);

    g.setColour (trackColour);
    g.strokePath (trackPath,
                  juce::PathStrokeType (strokeW,
                                        juce::PathStrokeType::curved,
                                        juce::PathStrokeType::rounded));

    // Filled arc up to current value
    const float toAngle  = startAngle + sliderPos * (endAngle - startAngle);
    juce::Path filledArc;
    filledArc.addArc (centreX - radius, centreY - radius,
                      radius * 2.0f, radius * 2.0f,
                      startAngle, toAngle, true);

    g.setColour (accentColour);
    g.strokePath (filledArc,
                  juce::PathStrokeType (strokeW,
                                        juce::PathStrokeType::curved,
                                        juce::PathStrokeType::rounded));

    // Thumb dot
    const float thumbRadius = strokeW * 0.6f;
    const float thumbX = centreX + (radius - strokeW * 0.5f) * std::cos (toAngle - juce::MathConstants<float>::halfPi);
    const float thumbY = centreY + (radius - strokeW * 0.5f) * std::sin (toAngle - juce::MathConstants<float>::halfPi);

    g.setColour (juce::Colours::white);
    g.fillEllipse (thumbX - thumbRadius, thumbY - thumbRadius,
                   thumbRadius * 2.0f, thumbRadius * 2.0f);
}

void ChorusLookAndFeel::drawLabel (juce::Graphics& g, juce::Label& label)
{
    g.setColour (textColour);
    g.setFont (getLabelFont (label));
    g.drawText (label.getText(),
                label.getLocalBounds(),
                label.getJustificationType(),
                true);
}

juce::Font ChorusLookAndFeel::getLabelFont (juce::Label&)
{
    return juce::Font (12.0f, juce::Font::plain);
}
