#include "PluginProcessor.h"
#include "PluginEditor.h"
VelvetAudioProcessor::VelvetAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input",juce::AudioChannelSet::stereo()).withOutput("Output",juce::AudioChannelSet::stereo())),
      apvts(*this,nullptr,"Parameters",createParameterLayout()) {}
VelvetAudioProcessor::~VelvetAudioProcessor() {}
juce::AudioProcessorValueTreeState::ParameterLayout VelvetAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("INPUT_GAIN","Input Gain",juce::NormalisableRange<float>(-24.0f,24.0f,0.01f),0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("OUTPUT_GAIN","Output Gain",juce::NormalisableRange<float>(-24.0f,24.0f,0.01f),0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("DELAY_DELAY_MS_FOK0","Delay Time",juce::NormalisableRange<float>(1.0f,2000.0f,0.01f),250.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("DELAY_FEEDBACK_FOK0","Delay Feedback",juce::NormalisableRange<float>(0.0f,95.0f,0.01f),40.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("DELAY_MIX_FOK0","Delay Mix",juce::NormalisableRange<float>(0.0f,100.0f,0.01f),30.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("GAIN_GAIN_DB_P65U","Gain Gain",juce::NormalisableRange<float>(-24.0f,24.0f,0.01f),0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("COMPRESSOR_THRESHOLD_EFNO","Compressor Threshold",juce::NormalisableRange<float>(-60.0f,0.0f,0.01f),-18.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("COMPRESSOR_RATIO_EFNO","Compressor Ratio",juce::NormalisableRange<float>(1.0f,20.0f,0.1f),4.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("COMPRESSOR_ATTACK_EFNO","Compressor Attack",juce::NormalisableRange<float>(0.1f,200.0f,0.01f),10.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("COMPRESSOR_RELEASE_EFNO","Compressor Release",juce::NormalisableRange<float>(10.0f,2000.0f,0.01f),150.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("COMPRESSOR_MAKEUP_EFNO","Compressor Makeup",juce::NormalisableRange<float>(0.0f,24.0f,0.01f),0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BASSBOOSTER_FREQ_L9J7","Bass Boost Freq",juce::NormalisableRange<float>(20.0f,300.0f,1.0f,0.5f),120.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BASSBOOSTER_GAIN_L9J7","Bass Boost Boost",juce::NormalisableRange<float>(0.0f,18.0f,0.01f),6.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BASSBOOSTER_DRIVE_L9J7","Bass Boost Drive",juce::NormalisableRange<float>(0.0f,100.0f,0.01f),20.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("STEREOWIDTH_WIDTH_633U","Width Width",juce::NormalisableRange<float>(0.0f,200.0f,0.01f),120.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BITCRUSHER_BITS_9PAI","Bit Crush Bit Depth",juce::NormalisableRange<float>(1.0f,16.0f,0.01f),8.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BITCRUSHER_DOWNSAMP_9PAI","Bit Crush Downsample",juce::NormalisableRange<float>(1.0f,32.0f,0.01f),1.0f));
    return layout;
}
void VelvetAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    currentSampleRate = sampleRate;
    juce::dsp::ProcessSpec spec;
    spec.sampleRate=(uint32_t)sampleRate; spec.maximumBlockSize=(uint32_t)samplesPerBlock; spec.numChannels=(uint32_t)getTotalNumOutputChannels();
    inputGainDsp.prepare(spec); inputGainDsp.setGainDecibels(0.0f);
    outputGainDsp.prepare(spec); outputGainDsp.setGainDecibels(0.0f);
    delay_FOK0.prepare(spec); delay_FOK0.setMaximumDelayInSamples((int)(spec.sampleRate*2.0)); delayMix_FOK0.prepare(spec); delayMix_FOK0.setWetMixProportion(0.3f);
    gain_P65U.prepare(spec); gain_P65U.setGainDecibels(0.0f);
    comp_EFNO.prepare(spec); comp_EFNO.setThreshold(-18.0f); comp_EFNO.setRatio(4.0f); comp_EFNO.setAttack(10.0f); comp_EFNO.setRelease(150.0f); compMakeup_EFNO.prepare(spec); compMakeup_EFNO.setGainDecibels(0.0f);
    *bbShelf_L9J7.state=*juce::dsp::IIR::Coefficients<float>::makeLowShelf(spec.sampleRate,120.0f,0.707,juce::Decibels::decibelsToGain(6.0f)); bbShelf_L9J7.prepare(spec);
    // stereowidth_633U no prepare
    // bitcrusher_9PAI no prepare
}
void VelvetAudioProcessor::releaseResources() {}
bool VelvetAudioProcessor::isBusesLayoutSupported(const BusesLayout& l) const {
    return l.getMainOutputChannelSet()==juce::AudioChannelSet::stereo()&&l.getMainInputChannelSet()==juce::AudioChannelSet::stereo();
}
void VelvetAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    juce::ScopedNoDenormals noDenormals;
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    inputGainDsp.setGainDecibels(*apvts.getRawParameterValue("INPUT_GAIN")); inputGainDsp.process(context);
    { delay_FOK0.setDelay((float)(*apvts.getRawParameterValue("DELAY_DELAY_MS_FOK0")*currentSampleRate/1000.0)); delayMix_FOK0.setWetMixProportion(*apvts.getRawParameterValue("DELAY_MIX_FOK0")/100.0f); delayMix_FOK0.pushDrySamples(block); delay_FOK0.process(context); delayMix_FOK0.mixWetSamples(block); }
    gain_P65U.setGainDecibels(*apvts.getRawParameterValue("GAIN_GAIN_DB_P65U")); gain_P65U.process(context);
    comp_EFNO.setThreshold(*apvts.getRawParameterValue("COMPRESSOR_THRESHOLD_EFNO")); comp_EFNO.setRatio(*apvts.getRawParameterValue("COMPRESSOR_RATIO_EFNO")); comp_EFNO.setAttack(*apvts.getRawParameterValue("COMPRESSOR_ATTACK_EFNO")); comp_EFNO.setRelease(*apvts.getRawParameterValue("COMPRESSOR_RELEASE_EFNO")); compMakeup_EFNO.setGainDecibels(*apvts.getRawParameterValue("COMPRESSOR_MAKEUP_EFNO")); comp_EFNO.process(context); compMakeup_EFNO.process(context);
    { *bbShelf_L9J7.state=*juce::dsp::IIR::Coefficients<float>::makeLowShelf(currentSampleRate,juce::jmax(20.0f,*apvts.getRawParameterValue("BASSBOOSTER_FREQ_L9J7")),0.707f,juce::Decibels::decibelsToGain(*apvts.getRawParameterValue("BASSBOOSTER_GAIN_L9J7"))); bbShelf_L9J7.process(context); float bbDrv=*apvts.getRawParameterValue("BASSBOOSTER_DRIVE_L9J7")/100.0f*3.0f; if(bbDrv>0.01f){float bbS=1.0f+bbDrv; for(int ch=0;ch<buffer.getNumChannels();++ch){auto*d=buffer.getWritePointer(ch);for(int i=0;i<buffer.getNumSamples();++i){d[i]=std::tanh(d[i]*bbS)/bbS;}}} }
    { float swW=*apvts.getRawParameterValue("STEREOWIDTH_WIDTH_633U")/100.0f; if(block.getNumChannels()>=2){for(int i=0;i<(int)block.getNumSamples();++i){float swL=block.getSample(0,i),swR=block.getSample(1,i),swMid=(swL+swR)*.5f,swSide=(swL-swR)*.5f*swW;block.setSample(0,i,swMid+swSide);block.setSample(1,i,swMid-swSide);}} }
    { float bcBits=juce::jmax(1.0f,*apvts.getRawParameterValue("BITCRUSHER_BITS_9PAI")); float bcLvl=std::pow(2.0f,bcBits)-1.0f; if(bcLvl<1.0f)bcLvl=1.0f; for(int ch=0;ch<buffer.getNumChannels();++ch){auto*d=buffer.getWritePointer(ch);for(int i=0;i<buffer.getNumSamples();++i){d[i]=std::round(d[i]*bcLvl)/bcLvl;}} }
    outputGainDsp.setGainDecibels(*apvts.getRawParameterValue("OUTPUT_GAIN")); outputGainDsp.process(context);
}
void VelvetAudioProcessor::getStateInformation(juce::MemoryBlock& d) {
    auto s=apvts.copyState(); std::unique_ptr<juce::XmlElement> xml(s.createXml()); copyXmlToBinary(*xml,d);
}
void VelvetAudioProcessor::setStateInformation(const void* d,int sz) {
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(d,sz));
    if(xml&&xml->hasTagName(apvts.state.getType())) apvts.replaceState(juce::ValueTree::fromXml(*xml));
}
juce::AudioProcessorEditor* VelvetAudioProcessor::createEditor() { return new VelvetAudioProcessorEditor(*this); }
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new VelvetAudioProcessor(); }
