namespace tremolo {
PluginEditor::PluginEditor(PluginProcessor& p) : AudioProcessorEditor(&p) {
    background.setImage(juce::ImageCache::getFromMemory(
      assets::Background_png, assets::Background_pngSize));

    logo.setImage(
      juce::ImageCache::getFromMemory(assets::Logo_png, assets::Logo_pngSize));


    addAndMakeVisible(background);
    addAndMakeVisible(logo);
    addAndMakeVisible(waveformToggle);
    addAndMakeVisible(lfoVisualizer);

    waveformToggle.onClick = [this]()
    {
        if (waveformToggle.getToggleState())
            lfoVisualizer.setLfoWaveform(Tremolo::LfoWaveform::triangle);
        else
            lfoVisualizer.setLfoWaveform(Tremolo::LfoWaveform::sine);
    };

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(540, 270);
}

    void PluginEditor::resized()
{
    const auto bounds = getLocalBounds();

    background.setBounds(bounds);

    const int logoWidth = 105;
    const int logoHeight = 24;
    const int margin = 16;

    logo.setBounds({margin, 16, logoWidth, logoHeight});

    lfoVisualizer.setBounds({18, 149, 504, 92});

    waveformToggle.setBounds(0, 110, 100, 25);
    }
}  // namespace tremolo
