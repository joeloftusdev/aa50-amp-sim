
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GuitarAmpBasicAudioProcessor::GuitarAmpBasicAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), apvts(*this, nullptr, juce::Identifier("PARAMETERS"), createParameterLayout())
#endif
{
    variableTree =
    {
        "Variables", {},
        {
            { "Group", {{"name", "IR Vars"}},
                {
                    {"Parameter", {{"id", "file1"}, {"value", "/"}}},
                    {"Parameter", {{"id", "root"}, {"value", "/"}}}
                }
            }
        }
    };
    isInput = true;
    apvts.state.addListener(this);
}

GuitarAmpBasicAudioProcessor::~GuitarAmpBasicAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout GuitarAmpBasicAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

    //Input and Output parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "INPUTGAIN", 1 }, "InputGain", 0.0f, 40.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "POSTGAIN", 1 }, "PostGain", 0.0f, 40.0f, 0.0f));

    //PreEQ
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PREEQ", 1 }, "PreEQ", 1.0f, 10.0f, 5.0f));

    //Pregain
  
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PREGAIN2", 1 }, "PreGain2", 0.0, 48.0f, 0.0f));

    //EQ
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "PRESENCE", 1 }, "Presence", 0.5f, 1.5f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "HIGH", 1 }, "High", 0.0f, 2.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "MID", 1 }, "Mid", 0.0f, 2.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LOW", 1 }, "Low", 0.0f, 2.0f, 1.0f));

    //Dist type
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "TYPE1", 1 }, "Type1",
        juce::StringArray{ "Tanh", "Amp2" },
        1));
    return { params.begin(), params.end() };

 
}



//==============================================================================
const juce::String GuitarAmpBasicAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GuitarAmpBasicAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool GuitarAmpBasicAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool GuitarAmpBasicAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double GuitarAmpBasicAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GuitarAmpBasicAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int GuitarAmpBasicAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GuitarAmpBasicAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String GuitarAmpBasicAudioProcessor::getProgramName(int index)
{
    return {};
}

void GuitarAmpBasicAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void GuitarAmpBasicAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    isInput = true;

    rmsLevelInput.reset(sampleRate, 0.5f);
    rmsLevelInput.setCurrentAndTargetValue(-100.0f);

    rmsLevelOutput.reset(sampleRate, 0.5f);
    rmsLevelOutput.setCurrentAndTargetValue(-100.0f);

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    reset();

    auto waveshapeInitFunction = apvts.getRawParameterValue("TYPE1");
    switch ((int)*waveshapeInitFunction) {
    case 1:
        setFunctionToUse("Tanh");
        waveshapeFunction = "Tanh";
        break;
    case 2:
        setFunctionToUse("Amp2");
        waveshapeFunction = "Amp2";
        break;

    default:
        setFunctionToUse("Tanh");
        waveshapeFunction = "Tanh";
        break;
    }


    inputGain.setGainDecibels(*apvts.getRawParameterValue("INPUTGAIN"));
    outputGain.setGainDecibels(*apvts.getRawParameterValue("POSTGAIN"));


    //Set up pre gains
    auto& preGain2 = processorChain.get<preGainIndex2>();
    preGain2.setGainDecibels(*apvts.getRawParameterValue("PREGAIN2"));

    //Set up PreEQ
    auto& preEQ = processorChain.get<preEQIndex>();

    preEQ.setMode(juce::dsp::LadderFilterMode::LPF12);
    preEQ.setResonance(0.2f);
    preEQ.setCutoffFrequencyHz(*apvts.getRawParameterValue("PREEQ") * 1000.0f);
    preEQ.setDrive(1.0f);

    presence();

    //Set up internal low shelf
    auto& lowEQ = processorChain.get<lowEQIndex>();
    *lowEQ.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, 400.0f, 0.4f, 0.4f);

    //
  
    //Prepare EQ filters
    updateFilter();


    

    


    //Prepare chain
    processorChain.prepare(spec);

    //Prepare convolution
    irLoader.reset();
    irLoader.prepare(spec);

    if (savedFile.existsAsFile())
    {
        irLoader.loadImpulseResponse(savedFile, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
    }
}



void GuitarAmpBasicAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GuitarAmpBasicAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
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


/*
    Presence Control

*/

void GuitarAmpBasicAudioProcessor::presence() {
    float gainPresence = *apvts.getRawParameterValue("PRESENCE");

    // Adjust center frequency and Q factor based on the presence control   
    float centerFrequency = 3000.0f + gainPresence * 500.0f; // Adjust the center frequency based on the presence control
    float qFactor = 0.6f + gainPresence * 0.05f;              // Adjust the Q factor based on the presence control

    // Make sure the parameters are within reasonable bounds
    centerFrequency = juce::jlimit(1000.0f, 6000.0f, centerFrequency);
    qFactor = juce::jlimit(0.1f, 1.0f, qFactor);

    // Calculate the new filter coefficients
    auto& filterPresence = processorChain.get<presenceIndex>();
    *filterPresence.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), centerFrequency, qFactor, gainPresence);
    
}

/*
    This function gets the current parameter values for the high, mid and low
    EQs, creates new filter with new values and set the state
 */
void GuitarAmpBasicAudioProcessor::updateFilter()

{
   // float gainPresence = *apvts.getRawParameterValue("PRESENCE");
    float gainHigh = *apvts.getRawParameterValue("HIGH");
    float gainMid = *apvts.getRawParameterValue("MID");
    float gainLow = *apvts.getRawParameterValue("LOW");
    

  //  if (gainPresence == 0.00f)
    //    gainPresence = 0.01f;

    if (gainHigh == 0.00f)
        gainHigh = 0.01f;

    if (gainMid == 0.00f)
        gainMid = 0.01f;

    if (gainLow == 0.00f)
        gainLow = 0.01f;

   


   // auto& filterPresence = processorChain.get<presenceIndex>();
   // *filterPresence.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 8000.0f, 0.6f, gainPresence);

    auto& filterHigh = processorChain.get<filterHighIndex>();
    *filterHigh.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 5000.0f, 0.6f, gainHigh);

    auto& filterMid = processorChain.get<filterMidIndex>();
    *filterMid.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 500.0f, 0.9f, gainMid);

    auto& filterLow = processorChain.get<filterLowIndex>();
    *filterLow.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 100.0f, 0.6f, gainLow);

}

void GuitarAmpBasicAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    //Set waveshaping function if changed
    if (waveshapeFunction != waveshapeFunctionCurrent)
        setFunctionToUse(waveshapeFunction);

    //Set and process input gain
    inputGain.setGainDecibels(*apvts.getRawParameterValue("INPUTGAIN"));

    //Process context replacing
    juce::dsp::AudioBlock<float> inputGainBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> inputContextReplacing(inputGainBlock);

    inputGain.process(inputContextReplacing);


    /*********Set input meter values*********/
    //Set val for input level meter
    isInput = true;
    rmsLevelInput.skip(buffer.getNumSamples());

    const auto valueIn = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
    if (valueIn < rmsLevelInput.getCurrentValue())
        rmsLevelInput.setTargetValue(valueIn);
    else
        rmsLevelInput.setCurrentAndTargetValue(valueIn);


    //Set values for pre gains
  
    auto& preGain2 = processorChain.get<preGainIndex2>();
    preGain2.setGainDecibels(*apvts.getRawParameterValue("PREGAIN2"));


    //Set value for pre EQ
    auto& preEQ = processorChain.get<preEQIndex>();
    preEQ.setCutoffFrequencyHz(*apvts.getRawParameterValue("PREEQ") * 1000.0f);

  //  
   //   DBG("Before Presence Filter: " << buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
    presence();


   // buffer.setSample(0, 0, 0.5f); // Set a non-zero sample at the beginning of channel 0


   //   DBG("Before EQ Filter: " << buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
   
  
    updateFilter();

   //   DBG("After EQ Filter: " << buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
 

    //Process waveshaper chain context replacing
    juce::dsp::AudioBlock<float> processorChainBlock(buffer);
    processorChain.process(juce::dsp::ProcessContextReplacing<float>(processorChainBlock));

    //Process convolver
    juce::dsp::AudioBlock<float> convolverBlock(buffer);

    //If there is an IR loaded, process it
    if (irLoader.getCurrentIRSize() > 0)
        irLoader.process(juce::dsp::ProcessContextReplacing<float>(convolverBlock));

    //Process output gain
    outputGain.setGainDecibels(*apvts.getRawParameterValue("POSTGAIN"));
    juce::dsp::AudioBlock<float> outputGainBlock(buffer);
    outputGain.process(juce::dsp::ProcessContextReplacing<float>(outputGainBlock));

    /*********Set output meter values*********/
    //Set val for output level meter
    rmsLevelOutput.skip(buffer.getNumSamples());
    isInput = false;

    const auto valueOut = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
    if (valueOut < rmsLevelOutput.getCurrentValue())
        rmsLevelOutput.setTargetValue(valueOut);
    else
        rmsLevelOutput.setCurrentAndTargetValue(valueOut);

    isInput = true;
}

void GuitarAmpBasicAudioProcessor::reset()
{
    processorChain.reset();
    irLoader.reset(); // [3]
}

/*
    This function returns the current RMS input level
 */
float GuitarAmpBasicAudioProcessor::getRMSValueInput(const int channel) const
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0)
        return rmsLevelInput.getCurrentValue();
    if (channel == 1)
        return rmsLevelInput.getCurrentValue();
    return 0;
}

/*
    This function returns the current RMS output level
 */
float GuitarAmpBasicAudioProcessor::getRMSValueOutput(const int channel) const
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0)
        return rmsLevelOutput.getCurrentValue();
    if (channel == 1)
        return rmsLevelOutput.getCurrentValue();
    return 0;
}

//==============================================================================
bool GuitarAmpBasicAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GuitarAmpBasicAudioProcessor::createEditor()
{
    return new GuitarAmpBasicAudioProcessorEditor(*this);
}

//==============================================================================
void GuitarAmpBasicAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    apvts.state.appendChild(variableTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    apvts.state.writeToStream(stream);

}

void GuitarAmpBasicAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));
    variableTree = tree.getChildWithName("Variables");

    if (tree.isValid())
    {
        apvts.state = tree;

        savedFile = juce::File(variableTree.getProperty("file1"));
        root = juce::File(variableTree.getProperty("root"));

        irLoader.loadImpulseResponse(savedFile, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
    }

}

/*
    This function takes a string corresponding to the waveshaping function
    to be used. It then sets the functionToUse lambda in the waveshaper.
 */
void GuitarAmpBasicAudioProcessor::setFunctionToUse(std::string func)
{
    auto& waveshaper1 = processorChain.get<waveshaperIndex1>();
    
    



    if (func == "Tanh")
    {
        waveshaper1.functionToUse = [](float x)
            {
                float a, x2, y;
                x = x * 0.25f;
                a = std::abs(x);
                x2 = x * x;
                y = 1 - 1 / (1 + a + x2 + 0.66422417311781f * x2 * a + 0.36483285408241f * x2 * x2);
                return (x >= 0) ? y : -y;
            };
        waveshapeFunctionCurrent = "Tanh";
    }
    else if (func == "Amp2")
    {
        waveshaper1.functionToUse = [](float x)
            {
                


                return std::tanh(x);


            };
        waveshapeFunctionCurrent = "Amp2";
    }
    return;
}




//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GuitarAmpBasicAudioProcessor();
}
