/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using namespace juce;

#define TOTAL_CONTROLS 23
#define KNOB_WIDTH 75
#define KNOB_HEIGHT 75
#define KNOB_LABEL_WIDTH 75
#define KNOB_LABEL_HEIGHT 25

//==============================================================================
/**
*/



class AUReverbAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    AUReverbAudioProcessorEditor (AUReverbAudioProcessor&);
    ~AUReverbAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AUReverbAudioProcessor& audioProcessor;

    juce::Slider* m_slider[TOTAL_CONTROLS];
    juce::Label* m_slider_label[TOTAL_CONTROLS];
    juce::SliderParameterAttachment* m_slider_attachment[TOTAL_CONTROLS];
    juce::AudioVisualiserComponent* m_audio_visualizer = NULL;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AUReverbAudioProcessorEditor)
};
