#include "ParameterLayout.h"
#include "ParameterIDs.h"

namespace Parameters
{

juce::AudioProcessorValueTreeState::ParameterLayout createLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Rate  0.1 – 10 Hz  (default 1 Hz, skew toward lower values)
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        ParameterIDs::rate, "Rate",
        juce::NormalisableRange<float> (0.1f, 10.0f, 0.01f, 0.4f),
        1.0f,
        juce::AudioParameterFloatAttributes{}.withLabel ("Hz")));

    // Depth  0 – 100 %
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        ParameterIDs::depth, "Depth",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
        0.5f,
        juce::AudioParameterFloatAttributes{}.withLabel ("%")));

    // Delay  1 – 30 ms
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        ParameterIDs::delay, "Delay",
        juce::NormalisableRange<float> (1.0f, 30.0f, 0.1f),
        7.0f,
        juce::AudioParameterFloatAttributes{}.withLabel ("ms")));

    // Feedback  -0.95 – 0.95
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        ParameterIDs::feedback, "Feedback",
        juce::NormalisableRange<float> (-0.95f, 0.95f, 0.01f),
        0.0f,
        juce::AudioParameterFloatAttributes{}.withLabel ("%")));

    // Mix  0 – 100 %
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        ParameterIDs::mix, "Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
        0.5f,
        juce::AudioParameterFloatAttributes{}.withLabel ("%")));

    // Voices  1 – 4
    params.push_back (std::make_unique<juce::AudioParameterInt> (
        ParameterIDs::voices, "Voices", 1, 4, 2));

    // Stereo toggle
    params.push_back (std::make_unique<juce::AudioParameterBool> (
        ParameterIDs::stereo, "Stereo", true));

    return { params.begin(), params.end() };
}

} // namespace Parameters
