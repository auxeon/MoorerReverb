/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


juce::StringArray controls_desc_string = {  "gain", "mix", "a", "m",
                                            "g0", "g1", "g2", "g3", "g4", "g5", 
                                            "R0", "R1", "R2", "R3", "R4", "R5", 
                                            "L0", "L1", "L2", "L3", "L4", "L5", 
                                        };
std::vector<int> controls_row = {   0,0,0,0,
                                    1,1,1,1,1,1,
                                    2,2,2,2,2,2,
                                    3,3,3,3,3,3 
                                };
std::vector<int> controls_col = {   0,1,2,3,
                                    0,1,2,3,4,5,
                                    0,1,2,3,4,5,
                                    0,1,2,3,4,5 
                                };

//==============================================================================
AUReverbAudioProcessorEditor::AUReverbAudioProcessorEditor (AUReverbAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    // banner
    m_banner = new juce::Label();
    m_banner->setName(juce::String("label_banner"));
    m_banner->setText("Abhikalp Unakal", juce::NotificationType::sendNotificationAsync);
    m_banner->setJustificationType(juce::Justification::centred);
    m_banner->setSize(150, 100);
    addAndMakeVisible(m_banner);
    // controls
    for (int i = 0; i < TOTAL_CONTROLS; ++i) {
        m_slider[i] = new juce::Slider();
        m_slider[i]->setSliderStyle(juce::Slider::SliderStyle::Rotary);
        m_slider[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 25);
        m_slider[i]->setRange(juce::Range<double>(0.0f, 200.0f), 0.01f);
        m_slider[i]->setName(juce::String("slider_")+controls_desc_string[i]);
        m_slider_attachment[i] = new juce::SliderParameterAttachment(*p.getRangedParameter(juce::String("m_")+controls_desc_string[i]), *m_slider[i]);
        m_slider[i]->addListener(this);

        m_slider_label[i] = new juce::Label();
        m_slider_label[i]->setName(juce::String("slider_label_")+ controls_desc_string[i]);
        m_slider_label[i]->setText(controls_desc_string[i], juce::NotificationType::sendNotificationAsync);
        m_slider_label[i]->setJustificationType(juce::Justification::centred);
        if (i>15 && i<22 || i == 3) {
            m_slider[i]->setTextValueSuffix("ms");
        }
        juce::AudioProcessorEditor::addAndMakeVisible(m_slider[i]);
        juce::AudioProcessorEditor::addAndMakeVisible(m_slider_label[i]);
    }


    // audio waveform
    m_audio_visualizer = new AudioVisualiserComponent(p.getNumInputChannels());
    m_audio_visualizer->setNumChannels(audioProcessor.getNumInputChannels());
    m_audio_visualizer->setBufferSize(512);
    m_audio_visualizer->setSamplesPerBlock(256);
    m_audio_visualizer->setColours(juce::Colours::black, juce::Colours::cyan);
    juce::AudioProcessorEditor::addAndMakeVisible(m_audio_visualizer);
    // canvas size
    juce::AudioProcessorEditor::setSize (450, 500);

    //console();


    Timer::startTimer(16);
}

AUReverbAudioProcessorEditor::~AUReverbAudioProcessorEditor()
{
    for (int i = 0; i < TOTAL_CONTROLS;++i) {
        // make sure to delete attachment before the slider
        delete m_slider_attachment[i];
        delete m_slider_label[i];
        delete m_slider[i];
    }

    delete m_audio_visualizer;
}

//==============================================================================
void AUReverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::black);

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    
}

void AUReverbAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    m_banner->setBounds(300, 25, 150, 75);

    //controls 
    for (int i = 0; i < TOTAL_CONTROLS; ++i) {
        m_slider[i]->setBounds(controls_col[i]*KNOB_WIDTH, controls_row[i]*(KNOB_HEIGHT + KNOB_LABEL_HEIGHT), KNOB_WIDTH,KNOB_HEIGHT);
        m_slider_label[i]->setBounds(controls_col[i]*KNOB_LABEL_WIDTH, controls_row[i]*(KNOB_HEIGHT + KNOB_LABEL_HEIGHT) + KNOB_HEIGHT, KNOB_LABEL_WIDTH, KNOB_LABEL_HEIGHT);
    }

    // visualizer
    m_audio_visualizer->setBounds(0, juce::AudioProcessorEditor::getHeight()-100, juce::AudioProcessorEditor::getWidth(), 100);

}

void AUReverbAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    // sync up g and R controls
    for (int i = 4; i < 10;++i) {
        // if g slider changed
        if (slider == m_slider[i]) {
            // update corresponding r 
            float r_new = g_to_r(slider->getValue());
            m_slider[i + 6]->setValue(r_new, juce::dontSendNotification);
        }
        if (slider == m_slider[i + 6]) {
            float g_new = r_to_g(slider->getValue());
            m_slider[i]->setValue(g_new, juce::dontSendNotification);
        }
    }
}

void AUReverbAudioProcessorEditor::timerCallback()
{
    m_audio_visualizer->pushBuffer(audioProcessor.getBuffer());
    m_audio_visualizer->repaint();
}
