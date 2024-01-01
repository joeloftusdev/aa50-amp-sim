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
class GuitarAmpBasicAudioProcessor : public juce::AudioProcessor, public juce::ValueTree::Listener
#if JucePlugin_Enable_ARA
    , public juce::AudioProcessorARAExtension
#endif
{
public:
    //==============================================================================
    GuitarAmpBasicAudioProcessor();
    ~GuitarAmpBasicAudioProcessor() override;

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

    void setFunctionToUse(std::string func);

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    void reset() override;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void presence();
    void updateFilter();


    //Level meter functions
    float getRMSValueInput(const int channel) const;
    float getRMSValueOutput(const int channel) const;

    juce::AudioProcessorValueTreeState apvts;
    juce::ValueTree variableTree;

    std::string waveshapeFunction;
    std::string waveshapeFunctionCurrent;

    bool isInput;

    juce::File root, savedFile;
    juce::dsp::Convolution irLoader;
private:
    enum
    {
        preEQIndex,
        lowEQIndex,
        preGainIndex2,
        waveshaperIndex1,
        presenceIndex,
        filterLowIndex,
        filterMidIndex,
        filterHighIndex
        

    };

    using IIRFilter = juce::dsp::IIR::Filter<float>;
    using IIRCoefs = juce::dsp::IIR::Coefficients<float>;

    juce::dsp::Gain<float> inputGain;

    juce::dsp::ProcessorChain<juce::dsp::LadderFilter<float>, //PreEQ
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs>, //LowEQ
        juce::dsp::Gain<float>, //Pregain
        juce::dsp::WaveShaper<float>, //Waveshaper
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs>,//presence
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs>, //low
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs>,  //mid
        juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefs> //high
    > processorChain;

    juce::dsp::Gain<float> outputGain;

    juce::dsp::ProcessSpec spec;
    juce::LinearSmoothedValue<float> rmsLevelInput;
    juce::LinearSmoothedValue<float> rmsLevelOutput;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuitarAmpBasicAudioProcessor)
};
