#include "esp_panel_audio_PCM5102.hpp"
#include "esp_panel_audio_MAX98357A.hpp"
#include "utils/esp_panel_utils_log.h"
#include "driver/i2s.h"

namespace esp_panel::drivers {

Audio::Audio(const AudioConfig &config) : _config(config) {}

Audio::~Audio() {
    // Cleanup resources if necessary
    i2s_driver_uninstall(I2S_NUM_0);
}

bool Audio::init() {
    ESP_UTILS_LOGI("Initializing audio with sample rate: %d", _config.sample_rate);

    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = _config.sample_rate,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = _config.i2s_bck_pin,      // GPIO 26
        .ws_io_num = _config.i2s_ws_pin,        // GPIO 25
        .data_out_num = _config.i2s_data_out_pin, // GPIO 22
        .data_in_num = I2S_PIN_NO_CHANGE
    };

    ESP_ERROR_CHECK(i2s_driver_install(I2S_NUM_0, &i2s_config, 0, nullptr));
    ESP_ERROR_CHECK(i2s_set_pin(I2S_NUM_0, &pin_config));

    return true;
}

bool Audio::begin() {
    ESP_UTILS_LOGI("Starting audio playback");
    // Start audio playback
    return true;
}

bool Audio::stop() {
    ESP_UTILS_LOGI("Stopping audio playback");
    // Stop audio playback
    return true;
}

bool Audio::setVolume(uint8_t volume) {
    ESP_UTILS_LOGI("Setting audio volume to: %d", volume);
    // Set audio volume
    return true;
}

} // namespace esp_panel::drivers
