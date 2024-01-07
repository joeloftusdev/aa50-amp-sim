#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_events/juce_events.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_dsp/juce_dsp.h>

//==============================================================================
/**
*/
class AA50AudioProcessor : public juce::AudioProcessor, public juce::ValueTree::Listener
#if JucePlugin_Enable_ARA
    , public juce::AudioProcessorARAExtension
#endif
{
public:
    //==============================================================================
    AA50AudioProcessor();
    ~AA50AudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void setWaveshaper();

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    void reset() override;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void presence();
    void equalize();
    void resonance();
    void bottomEnd();

    float getRMSOutputValue(const int channel) const;

    juce::AudioProcessorValueTreeState apvts;
    juce::ValueTree valueTree;

    std::string waveshaperFunction;
    std::string currentWaveshapeFunction;

    bool input;

    juce::File root, savedFile;
    juce::dsp::Convolution irLoader;

private:
    enum 
    {
        resonanceIndex,
        bottomEndIndex,
        preGainIndex,
        waveshaperIndex,
        presenceIndex,
        bassIndex,
        midIndex,
        trebleIndex
    };

    using IIRFilter = juce::dsp::IIR::Filter<float>;
    using IIRCoefs = juce::dsp::IIR::Coefficients<float>;

    juce::dsp::Gain<float> _input;
    juce::dsp::ProcessorChain<juce::dsp::LadderFilter<float>, //Resonance
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs>, //Bottom End
        juce::dsp::Gain<float>, //Pre Gain
        juce::dsp::WaveShaper<float>, //Distortion
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs>,//Presence
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs>, //Bass
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs>,  //Mid
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> //Treble
        > processorChain;
    juce::dsp::Gain<float> _output;

    juce::dsp::ProcessSpec _spec;
    juce::LinearSmoothedValue<float> _rmsOutput;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AA50AudioProcessor)
};
