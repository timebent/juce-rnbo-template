#include "DebugComponent.h"
#include "CustomAudioProcessor.h"

DebugComponent::DebugComponent(juce::AudioProcessor* processor)
    : audioProcessor(processor)
{
    oscilloscope = std::make_unique<foleys::MagicOscilloscope>();
    analyser = magicState.createAndAddObject<foleys::MagicAnalyser>("input");
    
    oscilloscopeDisplay = std::make_unique<foleys::MagicPlotComponent>(oscilloscope.get());
    analyserDisplay = std::make_unique<foleys::MagicPlotComponent>(analyser);
    
    addAndMakeVisible(*oscilloscopeDisplay);
    addAndMakeVisible(*analyserDisplay);
    
    // Setup colours
    oscilloscopeDisplay->setColour(foleys::MagicPlotComponent::plotColourId, juce::Colours::lime);
    analyserDisplay->setColour(foleys::MagicPlotComponent::plotColourId, juce::Colours::magenta);
    
    // Set fill colors transparent
    oscilloscopeDisplay->setColour(foleys::MagicPlotComponent::plotFillColourId, juce::Colours::transparentBlack);
    analyserDisplay->setColour(foleys::MagicPlotComponent::plotFillColourId, juce::Colours::transparentBlack);
    
    // Prepare the plot sources with default sample rate
    oscilloscope->prepareToPlay(44100.0, 512);
    analyser->prepareToPlay(44100.0, 512);
    
    // Register the audio processing callback
    if (auto* customProc = dynamic_cast<CustomAudioProcessor*>(processor))
    {
        customProc->setAudioProcessCallback([this](const juce::AudioBuffer<float>& buffer)
        {
            this->processAudioBlock(buffer);
        });
    }
}

DebugComponent::~DebugComponent()
{
}

void DebugComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void DebugComponent::resized()
{
    auto area = getLocalBounds();
    
    // Split vertically
    auto oscArea = area.removeFromTop(area.getHeight() / 2);
    
    // Add labels
    oscArea.removeFromTop(20);
    analyserDisplay->setBounds(area);
    oscilloscopeDisplay->setBounds(oscArea);
}

void DebugComponent::processAudioBlock(const juce::AudioBuffer<float>& buffer)
{
    oscilloscope->pushSamples(buffer);
    analyser->pushSamples(buffer);
}
