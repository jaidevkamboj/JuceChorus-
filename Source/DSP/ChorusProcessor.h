#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "../Utils/Constants.h"

/**
 * ChorusProcessor
 *
 * Implements a stereo, multi-voice BBD-style chorus using a single
 * interpolated delay line per channel driven by sine LFOs.
 *
 * All parameters are smoothed to avoid zipper noise.
 */
class ChorusProcessor
{
public:
    ChorusProcessor();

    //==========================================================================
    void prepare (double sampleRate, int samplesPerBlock, int numChannels);
    void reset();

    /** Process a buffer in-place. Call setParameters() before each block. */
    void process (juce::AudioBuffer<float>& buffer);

    //==========================================================================
    struct Parameters
    {
        float rateHz     { 1.0f };
        float depth      { 0.5f };   ///< 0–1
        float delayMs    { 7.0f };   ///< centre delay in ms
        float feedback   { 0.0f };   ///< -0.95 – 0.95
        float mix        { 0.5f };   ///< 0–1 dry/wet
        int   numVoices  { 2 };      ///< 1–4
        bool  stereo     { true };
    };

    void setParameters (const Parameters& p) noexcept { params = p; }

private:
    //==========================================================================
    // One LFO phase per voice per channel
    std::array<std::array<float, Constants::kMaxVoices>, 2> lfoPhase {};

    // Delay lines – one per channel
    std::array<juce::dsp::DelayLine<float,
                    juce::dsp::DelayLineInterpolationTypes::Linear>, 2> delayLines;

    // Feedback state
    std::array<float, 2> feedbackSample {};

    // Smoothed values
    juce::SmoothedValue<float> smoothRate, smoothDepth, smoothDelay,
                               smoothFeedback, smoothMix;

    Parameters params;
    double sampleRate { 44100.0 };

    float advanceLfo (float& phase, float rateHz) noexcept;
};
