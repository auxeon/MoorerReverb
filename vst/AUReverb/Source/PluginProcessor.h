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

    juce::RangedAudioParameter* getRangedParameter(juce::String parameter_name) { 
        juce::AudioParameterFloat* res = NULL;
        if (parameter_name == STRINGMACRO(m_gain)) {
            res = m_gain;
        }
        else if (parameter_name == STRINGMACRO(m_mix)) {
            res = m_mix;
        }
        else if (parameter_name == STRINGMACRO(m_test)) {
            res = m_test;
        }
        else if (parameter_name == "m_g0") {
            res = m_g[0];
        }
        else if (parameter_name == "m_g1") {
            res = m_g[1];
        }
        else if (parameter_name == "m_g2") {
            res = m_g[2];
        }
        else if (parameter_name == "m_g3") {
            res = m_g[3];
        }
        else if (parameter_name == "m_g4") {
            res = m_g[4];
        }
        else if (parameter_name == "m_g5") {
            res = m_g[5];
        }
        else if (parameter_name == "m_R0") {
            res = m_r[0];
        }
        else if (parameter_name == "m_R1") {
            res = m_r[1];
        }
        else if (parameter_name == "m_R2") {
            res = m_r[2];
        }
        else if (parameter_name == "m_R3") {
            res = m_r[3];
        }
        else if (parameter_name == "m_R4") {
            res = m_r[4];
        }
        else if (parameter_name == "m_R5") {
            res = m_r[5];
        }
        else if (parameter_name == "m_L0") {
            res = m_l[0];
        }
        else if (parameter_name == "m_L1") {
            res = m_l[1];
        }
        else if (parameter_name == "m_L2") {
            res = m_l[2];
        }
        else if (parameter_name == "m_L3") {
            res = m_l[3];
        }
        else if (parameter_name == "m_L4") {
            res = m_l[4];
        }
        else if (parameter_name == "m_L5") {
            res = m_l[5];
        }
        return (juce::RangedAudioParameter*)res;
    };

    juce::AudioBuffer<float>& getBuffer() { return m_buffer; };
    MoorerReverb* m_reverb_unit;
    juce::AudioParameterFloat* m_gain = NULL;
    juce::AudioParameterFloat* m_mix = NULL;
    juce::AudioParameterFloat* m_test = NULL;
    juce::AudioParameterFloat* m_g[6] = { NULL };
    juce::AudioParameterFloat* m_r[6] = { NULL };
    juce::AudioParameterFloat* m_l[6] = { NULL };
    juce::AudioBuffer<float> m_buffer;

private:

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AUReverbAudioProcessor)
};
