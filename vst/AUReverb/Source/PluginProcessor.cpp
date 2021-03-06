/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Reverb.hpp"

//==============================================================================
AUReverbAudioProcessor::AUReverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    if (max_delay_ms < 1000.0f) {
        max_delay_ms = 1000.0f;
    }
    
    addParameter(m_gain = new juce::AudioParameterFloat("gain", "gain", 0.0f, 2.0f, 1.0f));
    addParameter(m_mix = new juce::AudioParameterFloat("mix", "mix", 0.0f, 1.0f, 1.0f));
    addParameter(m_m = new juce::AudioParameterFloat("m", "m", 0.0f, max_delay_ms - 500, 10.0f));
    addParameter(m_a = new juce::AudioParameterFloat("a", "a", 0.0f, 0.99f, 0.7f));
    addParameter(m_test = new juce::AudioParameterFloat("test", "test", 0.0f, 2.0f, 1.0f));

    // calculate coefficients 
    calc_g();
    calc_coeff();
    m_reverb_unit = new MoorerReverb();
    m_reverb_unit->init();
    m_reverb_unit->set_sample_rate(48100);
    float epsilon = 0.001f;
    for (int i = 0; i < 6; ++i) {
        // 1-epsilon to prevent divide by zero
        addParameter(m_g[i] = new juce::AudioParameterFloat("g_" + juce::String(i), "g_" + juce::String(i), 0.0f, 1.0f-epsilon, m_reverb_unit->cf[i].gain_filter.gain));
        addParameter(m_r[i] = new juce::AudioParameterFloat("r_" + juce::String(i), "r_" + juce::String(i), 0.0f, 1.0f, m_reverb_unit->cf[i].low_pass_filter.coeff));
        addParameter(m_l[i] = new juce::AudioParameterFloat("l_" + juce::String(i), "l_" + juce::String(i), 0.0f, max_delay_ms-500, m_reverb_unit->cf[i].delay_filter.delay_ms));
    }
}

AUReverbAudioProcessor::~AUReverbAudioProcessor()
{

    delete m_reverb_unit;

}

//==============================================================================
const juce::String AUReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AUReverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AUReverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AUReverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AUReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AUReverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AUReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AUReverbAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AUReverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void AUReverbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AUReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AUReverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AUReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AUReverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    // update moorer reverb values for comb filter
    for (int i = 0; i < 6;++i) {
        // g
        m_reverb_unit->cf[i].gain_filter.set_gain(m_g[i]->get());
        // R
        m_reverb_unit->cf[i].low_pass_filter.set_coeff(m_r[i]->get());
        // L
        m_reverb_unit->cf[i].delay_filter.set_delay_ms(m_l[i]->get());
    }

    // update moorer reverb values for all pass filter
    m_reverb_unit->ap[0].gain_filter.set_gain(m_a->get());
    m_reverb_unit->ap[0].delay_filter.set_delay_ms(m_m->get());

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();



    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    float gain = m_gain->get();
    float mix = m_mix->get();
    float test = m_test->get();
    float m = m_m->get();
    float a = m_a->get();


    // interleaving across channels to keep the same state 
    for (int i = 0; i < buffer.getNumSamples(); ++i) {

        for (int c = 0; c < totalNumInputChannels; ++c)
        {
            auto* sample = buffer.getWritePointer(c);
            // perform moorer reverb
            sample[i] = (1.0f - mix)*sample[i] + mix * m_reverb_unit->process(sample[i]);

            // apply gain [0.0f, 2.0f]
            sample[i] = sample[i] * gain;
            
        }
    }

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // output signal 
    m_buffer = buffer;
}

//==============================================================================
bool AUReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AUReverbAudioProcessor::createEditor()
{
    return new AUReverbAudioProcessorEditor (*this);
}

//==============================================================================
void AUReverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AUReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::RangedAudioParameter* AUReverbAudioProcessor::getRangedParameter(juce::String parameter_name)
{
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
    else if (parameter_name == STRINGMACRO(m_m)) {
        res = m_m;
    }
    else if (parameter_name == STRINGMACRO(m_a)) {
        res = m_a;
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
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AUReverbAudioProcessor();
}
