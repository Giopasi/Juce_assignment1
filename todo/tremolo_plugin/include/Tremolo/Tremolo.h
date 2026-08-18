#pragma once

namespace tremolo {
class Tremolo {
public:
  Tremolo() { //costruttore
    lfo.setFrequency(440.f /* Hz*/, true);
  }

  void prepare(double sampleRate, int expectedMaxFramesPerBlock) {
    juce::ignoreUnused(sampleRate, expectedMaxFramesPerBlock);

    const juce::dsp::ProcessSpec processSpec {
      .sampleRate = sampleRate,
      .maximumBlockSize = static_cast<juce::uint32>(expectedMaxFramesPerBlock),
      .numChannels = 1u,
    };
    lfo.prepare(processSpec);
  }

  void process(juce::AudioBuffer<float>& buffer) noexcept {
    // for each frame
    for (const auto frameIndex : std::views::iota(0, buffer.getNumSamples())) {
      const auto lfoValue = lfo.processSample(0.f);

      //calculate the modulation value
      constexpr  auto modulationDepth = 0.4f; //modulazione di profondità del tremolo
      constexpr  auto modulationValue = modulationDepth * lfoValue + 1.f; //m[n], modulazione tra 0-1

      // for each channel sample in the frame
      for (const auto channelIndex :
           std::views::iota(0, buffer.getNumChannels())) {
        // get the input sample
        const auto inputSample = buffer.getSample(channelIndex, frameIndex);

        //modulate the sample
        const auto outputSample = inputSample * modulationValue; // y[n] = x[n] * m[n],
                                                                      //equazione alle differenze del tremolo
        // set the output sample
        buffer.setSample(channelIndex, frameIndex, outputSample);
      }
    }
  }

  void reset() noexcept {
    lfo.reset();
  }

private:
  // You should put class members and private functions here
    juce::dsp::Oscillator<float> lfo {
      []( auto phase){ return std::sin(phase); }
      }; //classe templetizzata con il float, inizializzata con funzione lambda (sine values e phase value)
  };
}  // namespace tremolo
