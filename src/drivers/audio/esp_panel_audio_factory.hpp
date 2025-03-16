#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "esp_panel_audio.hpp"
#include "esp_panel_audio_PCM5102.hpp"
#include "esp_panel_audio_MAX98357A.hpp"

namespace esp_panel::drivers {

class AudioFactory {
public:
    using Config = AudioConfig;
    using FunctionDeviceConstructor = std::shared_ptr<Audio> (*)(const Config &config);

    static std::shared_ptr<Audio> create(const std::string &device_name, const Config &config);

private:
    static const std::unordered_map<std::string, FunctionDeviceConstructor> _type_constructor_map;
};

} // namespace esp_panel::drivers
