#include "ChorusProcessor.h"
#include "../Utils/Helpers.h"
#include <cmath>

static constexpr float kTwoPi = 6.283185307f;

ChorusProcessor::ChorusProcessor()
{
    for (auto& dl : delayLines)
        dl.setMaximumDelayInSamples (Constants::kMaxDelayLineSamples);
}

void ChorusProcessor::prepare (double sr, int samplesPerBlock, int numChannels)
{
    sampleRate = sr;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate       = sr;
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    spec.numChannels      = 1; // each delay line is mono

    for (auto& dl : delayLines)
        dl.prepare (spec);

    // Ramp time ~20 ms
    const float rampSecs = 0.02f;
    smoothRate    .reset (sr, rampSecs);
    smoothDepth   .reset (sr, rampSecs);
    smoothDelay   .reset (sr, rampSecs);
    smoothFeedback.reset (sr, rampSecs);
    smoothMix     .reset (sr, rampSecs);

    reset();
}

void ChorusProcessor::reset()
{
    for (auto& dl : delayLines)
        dl.reset();

    feedbackSample.fill (0.0f);
    lfoPhase[0].fill (0.0f);

    // Spread right-channel LFO phases across voices for width
    for (int v = 0; v < Constants::kMaxVoices; ++v)
        lfoPhase[1][v] = (kTwoPi * Constants::kLfoPhaseOffsetDeg / 360.0f)
                         * static_cast<float> (v + 1);
}

float ChorusProcessor::advanceLfo (float& phase, float rateHz) noexcept
{
    const float out = std::sin (phase);
    phase += kTwoPi * rateHz / static_cast<float> (sampleRate);
    if (phase >= kTwoPi) phase -= kTwoPi;
    return out;
}

void ChorusProcessor::process (juce::AudioBuffer<float>& buffer)
{
    const int numSamples  = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    smoothRate    .setTargetValue (params.rateHz);
    smoothDepth   .setTargetValue (params.depth);
    smoothDelay   .setTargetValue (params.delayMs);
    smoothFeedback.setTargetValue (params.feedback);
    smoothMix     .setTargetValue (params.mix);

    const int voices = Helpers::clamp (params.numVoices, 1, Constants::kMaxVoices);

    for (int s = 0; s < numSamples; ++s)
    {
        const float rate     = smoothRate    .getNextValue();
        const float depth    = smoothDepth   .getNextValue();
        const float delayMs  = smoothDelay   .getNextValue();
        const float feedback = smoothFeedback.getNextValue();
        const float mix      = smoothMix     .getNextValue();

        const float centreDelaySamples =
            static_cast<float> (Helpers::msToSamples (delayMs, sampleRate));
        const float depthSamples =
            depth * static_cast<float> (Helpers::msToSamples (5.0f, sampleRate));

        for (int ch = 0; ch < juce::jmin (numChannels, 2); ++ch)
        {
            const bool isRight = (ch == 1) && params.stereo;
            float wetSum = 0.0f;

            for (int v = 0; v < voices; ++v)
            {
                const float voicePhaseOffset =
                    isRight ? lfoPhase[1][v] : lfoPhase[0][v];

                // Advance LFO for channel 0 only (ch 1 reads offset values)
                float lfoOut;
                if (ch == 0)
                {
                    lfoOut = advanceLfo (lfoPhase[0][v], rate);
                }
                else
                {
                    // Read pre-advanced phase for stereo spread
                    lfoOut = std::sin (lfoPhase[0][v] + voicePhaseOffset);
                }

                const float delaySamples =
                    Helpers::clamp (centreDelaySamples + lfoOut * depthSamples,
                                    1.0f,
                                    static_cast<float> (Constants::kMaxDelayLineSamples - 1));

                delayLines[ch].setDelay (delaySamples);

                float* writePtr = buffer.getWritePointer (ch);
                const float dryIn = writePtr[s] + feedbackSample[ch] * feedback;
                delayLines[ch].pushSample (0, dryIn);
                wetSum += delayLines[ch].popSample (0);
            }

            wetSum /= static_cast<float> (voices);
            feedbackSample[ch] = wetSum;

            float* writePtr = buffer.getWritePointer (ch);
            writePtr[s] = writePtr[s] * (1.0f - mix) + wetSum * mix;
        }
    }
}
