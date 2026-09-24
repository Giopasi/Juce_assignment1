#pragma once

namespace tremolo {
class Tremolo {
public:
  enum class LfoWaveform : size_t {
    sine = 0,
    triangle = 1,
  };

  Tremolo() { //costruttore
    for (auto& lfo: lfos) {
      lfo.setFrequency(5.f /* Hz*/, true);
    }
  }

  void prepare(double sampleRate, int expectedMaxFramesPerBlock) {
    juce::ignoreUnused(sampleRate, expectedMaxFramesPerBlock);

    const juce::dsp::ProcessSpec processSpec {
      .sampleRate = sampleRate,
      .maximumBlockSize = static_cast<juce::uint32>(expectedMaxFramesPerBlock),
      .numChannels = 1u,
    };
    for (auto& lfo: lfos) {
      lfo.prepare(processSpec);
    }
  }

  void setLfoWaveform(LfoWaveform waveform) {
    jassert(waveform == LfoWaveform::sine || waveform == LfoWaveform::triangle);
    lfoToSet = waveform;
  }

  void setModulationRate(float rateHz) {
    for (auto& lfo : lfos) {
      lfo.setFrequency(rateHz);
    }
  }

  void setGaindB(float gain) {
    Gain.setGainDecibels(gain);
  }

  void process(juce::AudioBuffer<float>& buffer) noexcept {
    updateLfosWaveform();
    // for each frame
    for (const auto frameIndex : std::views::iota(0, buffer.getNumSamples())) {

      const auto lfoValue = getNextLfoValue();

      //calculate the modulation value
      constexpr  auto modulationDepth = 0.4f; //modulazione di profondità del tremolo
      const auto modulationValue = modulationDepth * lfoValue + 1.f; //m[n], modulazione tra 0-1

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
    for (auto& lfo: lfos) {
      lfo.reset();
    }
  }

private:
  // You should put class members and private functions here
  static float triangle(float phase) { //static così non è dipendente dalla istanza della classe tremolo
    const auto ft = phase /juce::MathConstants<float>::twoPi;
    return 4.f * std::abs(ft - std::floor(ft + 0.5f)) - 1.f;
  }

  float getNextLfoValue() {
    return lfos[juce::toUnderlyingType(currentLfo)].processSample(0.f);
  }

  void updateLfosWaveform() {
    if (currentLfo != lfoToSet) {
      currentLfo = lfoToSet;
    }
  }

  /*---------------------------------------------------------*/
  //classe templetizzata con il float, inizializzata con funzione lambda (sine values e phase value)
  std::array<juce::dsp::Oscillator<float>, 2u> lfos{
    juce::dsp::Oscillator<float>{[]( auto phase){ return std::sin(phase); }},
    juce::dsp::Oscillator<float>{ triangle },
  };
  LfoWaveform currentLfo = LfoWaveform::sine;
  LfoWaveform lfoToSet = currentLfo;

  juce::dsp::Gain<float> Gain;
  };
}  // namespace tremolo
