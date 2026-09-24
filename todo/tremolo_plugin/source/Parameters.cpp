
namespace tremolo {
  namespace {
    juce::AudioParameterFloat& createModulationRateParameter(juce::AudioProcessor& processor) {
      constexpr auto versionHint = 1;
      auto parameter = std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"modulation.rate", versionHint},
        "Modulation rate",
        juce::NormalisableRange{0.1f, 20.f, 0.01f, 0.4f, },
        5.f,
        juce::AudioParameterFloatAttributes{}.withLabel(" Hz") );

      //meccanismo di ownership
      auto& parameterReference = *parameter;
      processor.addParameter( parameter.release() );
      return parameterReference;
    }

    juce::AudioParameterFloat& createGainParameter(juce::AudioProcessor& processor) {
      constexpr auto versionHint = 1;
      auto parameter = std::make_unique<juce::AudioParameterFloat>(
          juce::ParameterID{"gain", versionHint},
          "Gain",
          juce::NormalisableRange{-12.f, 12.f, 0.01f, 0.4f, },
          0.f,
          juce::AudioParameterFloatAttributes{}.withLabel(" dB"));

      //meccanismo di ownership
      auto& parameterReference = *parameter;
      processor.addParameter( parameter.release() );
      return parameterReference;
    }
  }
Parameters::Parameters(juce::AudioProcessor& processor)
// TODO: create parameters
// TODO: retrieve references to parameters
// TODO: add parameters to the processor
  :rate{createModulationRateParameter(processor)},
   gain{createGainParameter(processor)}
{
}
}  // namespace tremolo
