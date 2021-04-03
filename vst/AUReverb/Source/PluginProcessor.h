/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Reverb.hpp"

#define STRINGMACRO(x) #x

//==============================================================================
/**
*/
class AUReverbAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    AUReverbAudioProcessor();
    ~AUReverbAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::RangedAudioParameter* getRangedParameter(juce::String parameter_name);

    juce::AudioBuffer<float>& getBuffer() { return m_buffer; };

    MoorerReverb* m_reverb_unit;
    juce::AudioParameterFloat* m_gain = NULL;
    juce::AudioParameterFloat* m_mix = NULL;
    juce::AudioParameterFloat* m_a = NULL;
    juce::AudioParameterFloat* m_m = NULL;
    juce::AudioParameterFloat* m_test = NULL;
    juce::AudioParameterFloat* m_g[6] = { NULL };
    juce::AudioParameterFloat* m_r[6] = { NULL };
    juce::AudioParameterFloat* m_l[6] = { NULL };
    juce::AudioBuffer<float> m_buffer;

private:

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AUReverbAudioProcessor)
};
