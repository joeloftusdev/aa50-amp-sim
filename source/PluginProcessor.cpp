
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AA50AudioProcessor::AA50AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), apvts(*this, nullptr, juce::Identifier("PARAMETERS"), createParameterLayout()),
      valueTree ("Variables", {}, { { "Group", { { "name", "IR Vars" } }, { { "Parameter", { { "id", "file1" }, { "value", "/" } } }, { "Parameter", { { "id", "root" }, { "value", "/" } } } } } }),
      input (true)
{
    apvts.state.addListener (this);
}
#endif


AA50AudioProcessor::~AA50AudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout AA50AudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> parameters;

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "INPUT", 1 }, "Input", 0.0f, 40.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "POSTGAIN", 1 }, "PostGain", 0.0f, 40.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "RESONANCE", 1 }, "Resonance", 1.0f, 10.0f, 5.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PREGAIN", 1 }, "PreGain", 0.0f, 48.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PRESENCE", 1 }, "Presence", 0.5f, 1.5f, 1.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "TREBLE", 1 }, "Treble", 0.0f, 2.0f, 1.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "MID", 1 }, "Mid", 0.0f, 2.0f, 1.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "BASS", 1 }, "Bass", 0.0f, 2.0f, 1.0f));
    parameters.push_back (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { "LEADCHANNEL", 1 },
        "LeadChannel",
        juce::StringArray { "Lead" }, 
        1));
    return { parameters.begin(), parameters.end() };
}



//==============================================================================
const juce::String AA50AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AA50AudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AA50AudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AA50AudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AA50AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AA50AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int AA50AudioProcessor::getCurrentProgram()
{
    return 0;
}

void AA50AudioProcessor::setCurrentProgram(int index)
{
    (void)index;
}

const juce::String AA50AudioProcessor::getProgramName(int index)
{
    (void) index;
    return {};
}

void AA50AudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    (void) index;
    (void) newName;
}

//==============================================================================



void AA50AudioProcessor::presence()
{
    float presenceEq = *apvts.getRawParameterValue ("PRESENCE");

    float centerFrequency = 3000.0f + presenceEq * 500.0f;
    float qFactor = 0.6f + presenceEq * 0.05f;

    centerFrequency = juce::jlimit (1000.0f, 6000.0f, centerFrequency);
    qFactor = juce::jlimit (0.1f, 1.0f, qFactor);

    auto& presenceFilter = processorChain.get<presenceIndex>();
    *presenceFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (getSampleRate(), centerFrequency, qFactor, presenceEq);
}

void AA50AudioProcessor::resonance()
{
    auto& resonance = processorChain.get<resonanceIndex>();
    resonance.setMode (juce::dsp::LadderFilterMode::LPF12);
    resonance.setResonance (0.2f);
    resonance.setCutoffFrequencyHz (*apvts.getRawParameterValue ("RESONANCE") * 1000.0f);
    resonance.setDrive (1.0f);
}

void AA50AudioProcessor::bottomEnd()
{
    auto& bottomEnd = processorChain.get<bottomEndIndex>();
    *bottomEnd.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf (getSampleRate(), 400.0f, 0.4f, 0.4f);
}


void AA50AudioProcessor::equalize()

{
    float treble = *apvts.getRawParameterValue ("TREBLE");
    float mid = *apvts.getRawParameterValue ("MID");
    float bass = *apvts.getRawParameterValue ("BASS");

    treble = juce::jlimit (0.01f, 2.0f, treble);
    mid = juce::jlimit (0.01f, 2.0f, mid);
    bass = juce::jlimit (0.01f, 2.0f, bass);

    auto& trebleFilter = processorChain.get<trebleIndex>();
    *trebleFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (getSampleRate(), 5000.0f, 0.6f, treble);

    auto& midFilter = processorChain.get<midIndex>();
    *midFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (getSampleRate(), 500.0f, 0.9f, mid);

    auto& bassFilter = processorChain.get<bassIndex>();
    *bassFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (getSampleRate(), 100.0f, 0.6f, bass);
}


void AA50AudioProcessor::setWaveshaper()
{
    auto& waveshaper = processorChain.get<waveshaperIndex>();

    waveshaper.functionToUse = [] (float x) {
        float a, x2, y;
        x = x * 0.25f;
        a = std::abs (x);
        x2 = x * x;
        y = 1 - 1 / (1 + a + x2 + 0.66422417311781f * x2 * a + 0.36483285408241f * x2 * x2);
        return (x >= 0) ? y : -y;
    };

    currentWaveshapeFunction = "Lead";
}



//==============================================================================
void AA50AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    input = true;

    _rmsOutput.reset(sampleRate, 0.5f);
    _rmsOutput.setCurrentAndTargetValue(-100.0f);

    _spec.sampleRate = sampleRate;
    _spec.maximumBlockSize = samplesPerBlock;
    _spec.numChannels = getTotalNumOutputChannels();
    reset();

    setWaveshaper();

    _input.setGainDecibels(*apvts.getRawParameterValue("INPUT"));
    _output.setGainDecibels(*apvts.getRawParameterValue("POSTGAIN"));
    auto& preGain = processorChain.get<preGainIndex>();
    preGain.setGainDecibels(*apvts.getRawParameterValue("PREGAIN"));

    resonance();

    presence();

    bottomEnd();

    equalize();

    processorChain.prepare(_spec);

    irLoader.reset();
    irLoader.prepare(_spec);

    if (savedFile.existsAsFile())
    {
        irLoader.loadImpulseResponse(savedFile, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
    }
}



void AA50AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AA50AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
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




void AA50AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    (void) midiMessages;

    juce::ScopedNoDenormals noDenormals;
    auto inputChannels = getTotalNumInputChannels();
    auto outputChannels = getTotalNumOutputChannels();

    for (auto i = inputChannels; i < outputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    setWaveshaper();

    _input.setGainDecibels(*apvts.getRawParameterValue("INPUT"));

    juce::dsp::AudioBlock<float> inputGainBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> inputContextReplacing(inputGainBlock);

    _input.process(inputContextReplacing);

    input = true;

    auto& preGain = processorChain.get<preGainIndex>();
    preGain.setGainDecibels(*apvts.getRawParameterValue("PREGAIN"));

    resonance();

    presence(); 
  
    equalize();

    juce::dsp::AudioBlock<float> processorChainBlock(buffer);
    processorChain.process(juce::dsp::ProcessContextReplacing<float>(processorChainBlock));

    juce::dsp::AudioBlock<float> convolverBlock(buffer);

    if (irLoader.getCurrentIRSize() > 0)
        irLoader.process(juce::dsp::ProcessContextReplacing<float>(convolverBlock));

    _output.setGainDecibels(*apvts.getRawParameterValue("POSTGAIN"));
    juce::dsp::AudioBlock<float> outputGainBlock(buffer);
    _output.process(juce::dsp::ProcessContextReplacing<float>(outputGainBlock));

    _rmsOutput.skip(buffer.getNumSamples());
    input = false;

    const auto valueOut = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
    if (valueOut < _rmsOutput.getCurrentValue())
        _rmsOutput.setTargetValue(valueOut);
    else
        _rmsOutput.setCurrentAndTargetValue(valueOut);

    input = true;
}

void AA50AudioProcessor::reset()
{
    processorChain.reset();
    irLoader.reset(); 
}


float AA50AudioProcessor::getRMSOutputValue(const int channel) const
{
    jassert(channel == 0 || channel == 1);
    return _rmsOutput.getCurrentValue();
}

//==============================================================================
bool AA50AudioProcessor::hasEditor() const
{
    return true; 
}

juce::AudioProcessorEditor* AA50AudioProcessor::createEditor()
{
    return new AA50AudioProcessorEditor(*this);
}

//==============================================================================
void AA50AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    apvts.state.appendChild(valueTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    apvts.state.writeToStream(stream);

}

void AA50AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));
    valueTree = tree.getChildWithName("Variables");

    if (tree.isValid())
    {
        apvts.state = tree;
        savedFile = juce::File(valueTree.getProperty("file1"));
        root = juce::File(valueTree.getProperty("root"));
        irLoader.loadImpulseResponse(savedFile, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
    }

}






//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AA50AudioProcessor();
}
