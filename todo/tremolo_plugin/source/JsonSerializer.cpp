namespace tremolo {
namespace {
    struct SerailizabileParameters {
        float rate;
        bool bypassed;
        juce::String waveform;

        static constexpr auto marshallingVersion = 1;

        template <typename Archive, typename T>
        static void serialise(Archive& archive, T& t) {
            using namespace juce;

            if (archive.getVersion() != 1) {
                return;
            }

            std::string pluginName = TREMOLO_PLUGIN_NAME;

            if (pluginName != TREMOLO_PLUGIN_NAME) {
                return;
            }

            archive(named("pluginName", pluginName));

            archive(named("modulationRateHz", t.rate), named("bypassed", t.bypassed),
                named("modulationWaveform", t.waveform));
        }
    };

    SerailizabileParameters from(const tremolo::Parameters& parameters) {
       return {
           .rate = parameters.rate.get(),
           .bypassed = parameters.bypassed.get(),
           .waveform = parameters.waveform.getCurrentChoiceName(),
       };
    }
}
void JsonSerializer::serialize(const Parameters& parameters,
                               juce::OutputStream& output) {
  const auto parametersToSerialize = from(parameters);

    const auto json = juce::ToVar::convert(parametersToSerialize);

    if (!json.has_value() ) {
        return;
    }

    juce::JSON::writeToStream(output, *json, juce::JSON::FormatOptions{}.withSpacing
                                                            (juce::JSON::Spacing::multiLine).
                                                            withMaxDecimalPlaces(2));
  //serialize parameters to the output stream as JSON
}

juce::Result JsonSerializer::deserialize(juce::InputStream& input,
                                         Parameters& parameters) {
    //seserialize parameters from the JSON input stream
    juce::var parsedResult;
    const auto result = juce::JSON::parse(input.readEntireStreamAsString(), parsedResult);
    if (result.failed()) {
        return result;
    }

    const auto parsedParameters = juce::FromVar::convert<SerailizabileParameters>(parsedResult);

    if (!parsedParameters.has_value()) {
        return juce::Result::fail("failed to parse parameters from JSON representation");
    }

    const auto moluationWaveformIndex= parameters.waveform.choices.indexOf(parsedParameters->waveform);
    if (moluationWaveformIndex < 0) {
        return juce::Result::fail("invalid modulation waveform name: " + parsedParameters->waveform);
    }
    parameters.waveform = moluationWaveformIndex;
    parameters.rate = parsedParameters->rate;
    parameters.bypassed = parsedParameters->bypassed;

  return juce::Result::ok();
}
}  // namespace tremolo
