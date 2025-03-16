#pragma once

#include <cstdint>

namespace esp_panel::drivers {

struct AudioConfig {
    uint32_t sample_rate;
    uint8_t channels;
    int i2s_bck_pin;      // GPIO 26
    int i2s_ws_pin;       // GPIO 25
    int i2s_data_out_pin; // GPIO 22
    // Add other audio configuration parameters as needed
};

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
