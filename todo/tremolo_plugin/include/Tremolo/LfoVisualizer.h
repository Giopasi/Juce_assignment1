namespace tremolo {
    class LfoVisualizer  : public juce::Component {
    public:
        /*
        void paint(juce::Graphics& g) override {
            juce::Path sine;
            const auto strokeWidth = 4.f;
            const auto halfHeight = getHeight() / 2;
            const auto amplitude = halfHeight - strokeWidth / 2.f;
            sine.startNewSubPath(0.f, halfHeight + amplitude * ::sin(phase));

            for (const auto x : std::views::iota(1, getWidth())) {
                const auto angle = phase + 0.05f * x;
                sine.lineTo(x, halfHeight + amplitude * ::sin(angle));
            }
            g.setColour(juce::Colours::orange);
            g.strokePath(sine, juce::PathStrokeType{strokeWidth});
        }
        */

        void setLfoWaveform(Tremolo::LfoWaveform waveform)
        {
            if (currentWaveform != waveform)
            {
                currentWaveform = waveform;
                repaint();
            }
        }

        void paint(juce::Graphics& g)
        {
            juce::Path waveform;

            const auto strokeWidth = 4.0f;
            const auto halfHeight = getHeight() / 2.0f;
            const auto amplitude = halfHeight - strokeWidth / 2.0f;

            const auto frequency = 0.05f;

            auto getWaveformValue = [this](float angle)
            {
                switch (currentWaveform)
                {
                    case Tremolo::LfoWaveform::sine:
                        return std::sin(angle);

                    case Tremolo::LfoWaveform::triangle:
                        return (2.0f / juce::MathConstants<float>::pi) *
                               std::asin(std::sin(angle));
                }

                return 0.0f;
            };

            waveform.startNewSubPath(
                0.0f,
                halfHeight + amplitude * getWaveformValue(phase)
            );

            for (const auto x : std::views::iota(1, getWidth()))
            {
                const auto angle = phase + frequency * x;

                waveform.lineTo(
                    static_cast<float>(x),
                    halfHeight + amplitude * getWaveformValue(angle)
                );
            }

            g.setColour(juce::Colours::orange);

            g.strokePath(
                waveform,
                juce::PathStrokeType{
                    strokeWidth,
                    juce::PathStrokeType::mitered,
                    juce::PathStrokeType::rounded
                }
            );
        }


    private:
        Tremolo::LfoWaveform currentWaveform = Tremolo::LfoWaveform::sine;

        float phase = 0.0f;
    };
}
