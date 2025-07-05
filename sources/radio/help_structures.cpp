#include "help_structures.h"

#include <string.h>
#include <utils.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

const std::string freqTable = {"Hz", "KHz", "MHz", "GHz"}

std::string frequencyToString(const Frequency &frequency, const std::string &label) {
  char ret[16]; // Should be at most 12 chars

  float truncated = std::trunc(frequency * 1000.0) / 1000.0; // Knocks off most decimals

  if(frequency <= 1000) { // Hz
    sprintf(buf, "%.3f" Hz", truncated/1000);
    return std::string(buf);
  }

  if(frequency <= 1000000) { // kHz
    sprintf(buf, "%.3f kHz", truncated/1000000);
    return std::string(buf);
  }

  if(frequency <= 1000000000) { // MHz
    sprintf(buf, "%.3f MHz", truncated/1000000000);
    return std::string(buf);
  }

    if(frequency <= 1000000000000) { // GHz
    sprintf(buf, "%.3f GHz", truncated/1000000000000);
    return std::string(buf);
  }

  return std::string(frequency);
}

std::string powerToString(const Power &power) {
  constexpr auto MIN_POWER = -70.0f;
  constexpr auto MAX_POWER = -10.0f;
  constexpr auto BAR_SIZE = 30;

  const auto p = std::lround(std::min(std::max((power - MIN_POWER) / (MAX_POWER - MIN_POWER), 0.0f), 1.0f) * BAR_SIZE);
  char buf[1024];
  sprintf(buf, "power: %6.2f dB ", power);
  return std::string(buf) + std::string(p, '#') + std::string(BAR_SIZE - p, '_');
}

std::string Signal::toString() const { return frequencyToString(frequency) + ", " + powerToString(power); }

FrequencyRange::FrequencyRange(const Frequency _start, const Frequency _stop, const Frequency _sampleRate, const uint32_t _fft)
    : start(_start), stop(_stop), sampleRate(_sampleRate), fft(_fft == 0 ? countFft(sampleRate) : _fft) {}

std::string FrequencyRange::toString() const {
  char buf[1024];
  sprintf(buf, "%s, %s, fft: %d", frequencyToString(start, "start").c_str(), frequencyToString(stop, "stop").c_str(), fft);
  return std::string(buf);
}

Frequency FrequencyRange::center() const { return (start + stop) / 2; }

uint32_t FrequencyRange::step() const { return sampleRate / fft; }

bool FrequencyRange::operator==(const FrequencyRange &rhs) const { return start == rhs.start && stop == rhs.stop && sampleRate == rhs.sampleRate && fft == rhs.fft; }

bool FrequencyRange::operator<(const FrequencyRange &rhs) const {
  return start < rhs.start || (start == rhs.start && stop < rhs.stop) || (stop == rhs.stop && sampleRate < rhs.sampleRate) || (sampleRate == rhs.sampleRate && fft < rhs.fft);
}
