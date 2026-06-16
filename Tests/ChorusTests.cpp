#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "../Source/DSP/ChorusProcessor.h"

// Minimal test harness (no external framework required)
static int passed = 0;
static int failed = 0;

#define EXPECT_TRUE(cond)  \
    do { if (cond) { ++passed; }  \
         else { ++failed; juce::Logger::writeToLog ("FAIL: " #cond " [line " + juce::String(__LINE__) + "]"); } } while (false)

#define EXPECT_NEAR(a, b, eps) \
    EXPECT_TRUE (std::abs ((a) - (b)) < (eps))

// ─────────────────────────────────────────────────────────────────────────────
static void testSilenceInSilenceOut()
{
    ChorusProcessor chorus;
    chorus.prepare (44100.0, 512, 2);

    juce::AudioBuffer<float> buf (2, 512);
    buf.clear();

    ChorusProcessor::Parameters p;
    p.mix = 1.0f; // full wet
    chorus.setParameters (p);
    chorus.process (buf);

    for (int ch = 0; ch < 2; ++ch)
        for (int s = 0; s < 512; ++s)
            EXPECT_NEAR (buf.getSample (ch, s), 0.0f, 1e-6f);
}

static void testMixZeroPassesThrough()
{
    ChorusProcessor chorus;
    chorus.prepare (44100.0, 512, 2);

    juce::AudioBuffer<float> buf (2, 512);
    // Fill with ramp
    for (int s = 0; s < 512; ++s)
        buf.setSample (0, s, static_cast<float> (s) / 511.0f);
    buf.copyFrom (1, 0, buf, 0, 0, 512);

    ChorusProcessor::Parameters p;
    p.mix = 0.0f; // full dry
    chorus.setParameters (p);
    chorus.process (buf);

    // After dry processing the signal should be largely unchanged
    // (SmoothedValue ramps from default so just check last quarter)
    for (int s = 384; s < 512; ++s)
        EXPECT_NEAR (buf.getSample (0, s), static_cast<float> (s) / 511.0f, 1e-4f);
}

static void testNoNaNsOrInfs()
{
    ChorusProcessor chorus;
    chorus.prepare (44100.0, 512, 2);

    juce::AudioBuffer<float> buf (2, 512);
    // Fill with white noise
    juce::Random rng (42);
    for (int ch = 0; ch < 2; ++ch)
        for (int s = 0; s < 512; ++s)
            buf.setSample (ch, s, rng.nextFloat() * 2.0f - 1.0f);

    ChorusProcessor::Parameters p;
    p.rateHz    = 3.5f;
    p.depth     = 0.8f;
    p.feedback  = 0.7f;
    p.numVoices = 4;
    chorus.setParameters (p);
    chorus.process (buf);

    for (int ch = 0; ch < 2; ++ch)
        for (int s = 0; s < 512; ++s)
        {
            const float v = buf.getSample (ch, s);
            EXPECT_TRUE (std::isfinite (v));
        }
}

// ─────────────────────────────────────────────────────────────────────────────
int main()
{
    juce::Logger::setCurrentLogger (juce::Logger::getCurrentLogger()); // no-op; suppresses warning

    testSilenceInSilenceOut();
    testMixZeroPassesThrough();
    testNoNaNsOrInfs();

    juce::Logger::writeToLog ("Results: " + juce::String (passed) + " passed, "
                                          + juce::String (failed) + " failed.");
    return failed == 0 ? 0 : 1;
}
