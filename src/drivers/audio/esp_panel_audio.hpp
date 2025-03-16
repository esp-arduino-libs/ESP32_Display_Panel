#pragma once

#include "esp_panel_audio_PCM5102.hpp"
#include "esp_panel_audio_MAX98357A.hpp"
#include "esp_panel_audio_conf_internal.h"

namespace esp_panel::drivers {

class Audio {
public:
    Audio(const AudioConfig &config);
    ~Audio();

    bool init();
    bool begin();
    bool stop();
    bool setVolume(uint8_t volume);

private:
    AudioConfig _config;
};

} // namespace esp_panel::drivers
