namespace tremolo {
    class LfoVisualizer  : public juce::Component {
    public:
        void paint(juce::Graphics& g) override {
            juce::Path sine;
            const auto strokeWidth = 4.f;
            const auto halfHeight = getHeight() / 2;
            const auto amplitude = halfHeight - strokeWidth / 2.f;
            sine.startNewSubPath(0.f, halfHeight + amplitude * ::sin(0.f));

            for (const auto x : std::views::iota(1, getWidth())) {
                sine.lineTo(x, halfHeight + amplitude * ::sin(0.1 * x));
            }
            g.setColour(juce::Colours::orange);
            g.strokePath(sine, juce::PathStrokeType{strokeWidth});
        }
    };
}
