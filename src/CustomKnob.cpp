#include "CustomKnob.h"

CustomKnob::CustomKnob()
{
    setSliderStyle(juce::Slider::RotaryVerticalDrag);
    setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    setRange(0.0, 1.0, 0.01);
}

CustomKnob::~CustomKnob()
{
}

void CustomKnob::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    auto centre = bounds.getCentre();
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 5.0f;
    
    // Calculate the angle for the knob position
    auto angle = juce::MathConstants<float>::pi * 1.25f + 
                 ((getValue() - getMinimum()) / (getMaximum() - getMinimum())) * 
                 juce::MathConstants<float>::pi * 1.5f;
    
    // Draw the outer circle (knob body)
    g.setColour(juce::Colours::darkgrey);
    g.fillEllipse(centre.x - radius, centre.y - radius, radius * 2.0f, radius * 2.0f);
    
    // Draw the inner circle
    g.setColour(juce::Colours::grey);
    g.fillEllipse(centre.x - radius * 0.8f, centre.y - radius * 0.8f, 
                  radius * 1.6f, radius * 1.6f);
    
    // Draw the pointer line
    juce::Path pointer;
    pointer.startNewSubPath(centre);
    pointer.lineTo(centre.x + std::cos(angle) * radius * 0.7f, 
                   centre.y + std::sin(angle) * radius * 0.7f);
    
    g.setColour(juce::Colours::white);
    g.strokePath(pointer, juce::PathStrokeType(3.0f));
}
