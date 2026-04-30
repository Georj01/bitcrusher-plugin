#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class BitcrusherPluginEditor : public juce::AudioProcessorEditor
{
public:
    BitcrusherPluginEditor(BitcrusherPluginProcessor&);
    ~BitcrusherPluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    BitcrusherPluginProcessor& audioProcessor;

    juce::Slider bitDepthSlider;
    juce::Slider downsampleFactorSlider;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> bitDepthAttachment;
    std::unique_ptr<Attachment> downsampleFactorAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BitcrusherPluginEditor)
};
