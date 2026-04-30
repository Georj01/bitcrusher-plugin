#include "PluginProcessor.h"
#include "PluginEditor.h"

BitcrusherPluginProcessor::BitcrusherPluginProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

BitcrusherPluginProcessor::~BitcrusherPluginProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout BitcrusherPluginProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"bitDepth", 1},
        "Bit Depth",
        juce::NormalisableRange<float>(2.0f, 32.0f, 0.1f),
        32.0f));

    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID{"downsampleFactor", 1},
        "Downsample Factor",
        1, 50, 1));

    return layout;
}

const juce::String BitcrusherPluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BitcrusherPluginProcessor::acceptsMidi() const
{
    return false;
}

bool BitcrusherPluginProcessor::producesMidi() const
{
    return false;
}

bool BitcrusherPluginProcessor::isMidiEffect() const
{
    return false;
}

double BitcrusherPluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BitcrusherPluginProcessor::getNumPrograms()
{
    return 1;
}

int BitcrusherPluginProcessor::getCurrentProgram()
{
    return 0;
}

void BitcrusherPluginProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String BitcrusherPluginProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void BitcrusherPluginProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void BitcrusherPluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);

    auto numChannels = getTotalNumOutputChannels();
    sampleCounters.assign(static_cast<size_t>(numChannels), 9999);
    heldSamples.assign(static_cast<size_t>(numChannels), 0.0f);
}

void BitcrusherPluginProcessor::releaseResources()
{
}

bool BitcrusherPluginProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void BitcrusherPluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    float bitDepth = apvts.getRawParameterValue("bitDepth")->load();
    int downsampleFactor = static_cast<int>(apvts.getRawParameterValue("downsampleFactor")->load());

    float steps = std::pow(2.0f, bitDepth);

    if (sampleCounters.size() < static_cast<size_t>(totalNumInputChannels))
    {
        sampleCounters.resize(static_cast<size_t>(totalNumInputChannels), 9999);
        heldSamples.resize(static_cast<size_t>(totalNumInputChannels), 0.0f);
    }

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            sampleCounters[static_cast<size_t>(channel)]++;
            if (sampleCounters[static_cast<size_t>(channel)] >= downsampleFactor)
            {
                sampleCounters[static_cast<size_t>(channel)] = 0;

                float quantized = std::floor(channelData[sample] * steps) / steps;
                heldSamples[static_cast<size_t>(channel)] = quantized;
            }

            channelData[sample] = heldSamples[static_cast<size_t>(channel)];
        }
    }
}

bool BitcrusherPluginProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* BitcrusherPluginProcessor::createEditor()
{
    return new BitcrusherPluginEditor(*this);
}

void BitcrusherPluginProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    if (xml != nullptr)
        copyXmlToBinary(*xml, destData);
}

void BitcrusherPluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BitcrusherPluginProcessor();
}
