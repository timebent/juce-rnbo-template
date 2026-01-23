#include "../../../src/CustomKnob.h"

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
    auto center = bounds.getCentre();
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 5.0f;
    
    // Calculate the angle for the knob position. The angle assumes clockwise rotation
    // pi * 0.75 is about 7 o'clock (or 135 degrees)
    // pi * 1.5 is 270 degrees, so that is the range of the pointer rotation
    auto angle = juce::MathConstants<float>::pi * 0.75f + 
                 ((getValue() - getMinimum()) / (getMaximum() - getMinimum())) * 
                 juce::MathConstants<float>::pi * 1.5f;
    
    // Draw the outer circle (knob body) - use rotarySliderOutlineColourId
    g.setColour(findColour(juce::Slider::rotarySliderOutlineColourId));
    g.fillEllipse(center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f);
    
    // Draw the inner circle - use backgroundColourId
    g.setColour(findColour(juce::Slider::backgroundColourId));
    g.fillEllipse(center.x - radius * 0.8f, center.y - radius * 0.8f, 
                  radius * 1.6f, radius * 1.6f);
    
    // Draw the pointer line - use rotarySliderFillColourId
    juce::Path pointer;
    pointer.startNewSubPath(center);
    pointer.lineTo(center.x + std::cos(angle) * radius * 0.7f, 
                   center.y + std::sin(angle) * radius * 0.7f);
    
    g.setColour(findColour(juce::Slider::rotarySliderFillColourId));
    g.strokePath(pointer, juce::PathStrokeType(3.0f));
}
