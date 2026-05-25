#pragma once
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"
class VelvetAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
    VelvetAudioProcessorEditor(VelvetAudioProcessor&);
    ~VelvetAudioProcessorEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    VelvetAudioProcessor& processorRef;
    VelvetLookAndFeel lnf;
    juce::Slider knob_INPUT_GAIN;
    juce::Label lbl_INPUT_GAIN;
    juce::AudioProcessorValueTreeState::SliderAttachment att_INPUT_GAIN;
    juce::Slider knob_DELAY_DELAY_MS_FOK0;
    juce::Label lbl_DELAY_DELAY_MS_FOK0;
    juce::AudioProcessorValueTreeState::SliderAttachment att_DELAY_DELAY_MS_FOK0;
    juce::Slider knob_DELAY_FEEDBACK_FOK0;
    juce::Label lbl_DELAY_FEEDBACK_FOK0;
    juce::AudioProcessorValueTreeState::SliderAttachment att_DELAY_FEEDBACK_FOK0;
    juce::Slider knob_DELAY_MIX_FOK0;
    juce::Label lbl_DELAY_MIX_FOK0;
    juce::AudioProcessorValueTreeState::SliderAttachment att_DELAY_MIX_FOK0;
    juce::Slider knob_GAIN_GAIN_DB_P65U;
    juce::Label lbl_GAIN_GAIN_DB_P65U;
    juce::AudioProcessorValueTreeState::SliderAttachment att_GAIN_GAIN_DB_P65U;
    juce::Slider knob_COMPRESSOR_THRESHOLD_EFNO;
    juce::Label lbl_COMPRESSOR_THRESHOLD_EFNO;
    juce::AudioProcessorValueTreeState::SliderAttachment att_COMPRESSOR_THRESHOLD_EFNO;
    juce::Slider knob_COMPRESSOR_RATIO_EFNO;
    juce::Label lbl_COMPRESSOR_RATIO_EFNO;
    juce::AudioProcessorValueTreeState::SliderAttachment att_COMPRESSOR_RATIO_EFNO;
    juce::Slider knob_COMPRESSOR_ATTACK_EFNO;
    juce::Label lbl_COMPRESSOR_ATTACK_EFNO;
    juce::AudioProcessorValueTreeState::SliderAttachment att_COMPRESSOR_ATTACK_EFNO;
    juce::Slider knob_COMPRESSOR_RELEASE_EFNO;
    juce::Label lbl_COMPRESSOR_RELEASE_EFNO;
    juce::AudioProcessorValueTreeState::SliderAttachment att_COMPRESSOR_RELEASE_EFNO;
    juce::Slider knob_COMPRESSOR_MAKEUP_EFNO;
    juce::Label lbl_COMPRESSOR_MAKEUP_EFNO;
    juce::AudioProcessorValueTreeState::SliderAttachment att_COMPRESSOR_MAKEUP_EFNO;
    juce::Slider knob_BASSBOOSTER_FREQ_L9J7;
    juce::Label lbl_BASSBOOSTER_FREQ_L9J7;
    juce::AudioProcessorValueTreeState::SliderAttachment att_BASSBOOSTER_FREQ_L9J7;
    juce::Slider knob_BASSBOOSTER_GAIN_L9J7;
    juce::Label lbl_BASSBOOSTER_GAIN_L9J7;
    juce::AudioProcessorValueTreeState::SliderAttachment att_BASSBOOSTER_GAIN_L9J7;
    juce::Slider knob_BASSBOOSTER_DRIVE_L9J7;
    juce::Label lbl_BASSBOOSTER_DRIVE_L9J7;
    juce::AudioProcessorValueTreeState::SliderAttachment att_BASSBOOSTER_DRIVE_L9J7;
    juce::Slider knob_STEREOWIDTH_WIDTH_633U;
    juce::Label lbl_STEREOWIDTH_WIDTH_633U;
    juce::AudioProcessorValueTreeState::SliderAttachment att_STEREOWIDTH_WIDTH_633U;
    juce::Slider knob_BITCRUSHER_BITS_9PAI;
    juce::Label lbl_BITCRUSHER_BITS_9PAI;
    juce::AudioProcessorValueTreeState::SliderAttachment att_BITCRUSHER_BITS_9PAI;
    juce::Slider knob_BITCRUSHER_DOWNSAMP_9PAI;
    juce::Label lbl_BITCRUSHER_DOWNSAMP_9PAI;
    juce::AudioProcessorValueTreeState::SliderAttachment att_BITCRUSHER_DOWNSAMP_9PAI;
    juce::Slider knob_OUTPUT_GAIN;
    juce::Label lbl_OUTPUT_GAIN;
    juce::AudioProcessorValueTreeState::SliderAttachment att_OUTPUT_GAIN;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VelvetAudioProcessorEditor)
};
