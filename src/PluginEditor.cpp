#include "PluginProcessor.h"
#include "PluginEditor.h"

BitcrusherPluginEditor::BitcrusherPluginEditor(BitcrusherPluginProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    bitDepthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bitDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(bitDepthSlider);

    downsampleFactorSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    downsampleFactorSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(downsampleFactorSlider);

    bitDepthAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "bitDepth", bitDepthSlider);
    downsampleFactorAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "downsampleFactor", downsampleFactorSlider);

    setSize(400, 300);
}

BitcrusherPluginEditor::~BitcrusherPluginEditor()
{
}

void BitcrusherPluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    
    auto area = getLocalBounds();
    g.drawFittedText("Bitcrusher", area.removeFromTop(40), juce::Justification::centred, 1);
    
    auto sliderArea = area.reduced(20);
    int width = sliderArea.getWidth() / 2;
    
    g.drawFittedText("Bit Depth", sliderArea.removeFromLeft(width).withSizeKeepingCentre(width, 20).translated(0, -70), juce::Justification::centred, 1);
    g.drawFittedText("Downsample", sliderArea.withSizeKeepingCentre(width, 20).translated(0, -70), juce::Justification::centred, 1);
}

void BitcrusherPluginEditor::resized()
{
    auto area = getLocalBounds().reduced(40);
    area.removeFromTop(40);

    int sliderWidth = 100;
    int sliderHeight = 100;

    bitDepthSlider.setBounds(area.removeFromLeft(area.getWidth() / 2).withSizeKeepingCentre(sliderWidth, sliderHeight));
    downsampleFactorSlider.setBounds(area.withSizeKeepingCentre(sliderWidth, sliderHeight));
}
