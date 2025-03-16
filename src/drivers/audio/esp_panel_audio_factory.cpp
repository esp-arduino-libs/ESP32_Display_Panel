#include "esp_panel_audio_factory.hpp"
#include "utils/esp_panel_utils_log.h"

namespace esp_panel::drivers {

#define DEVICE_CREATOR(type_name) \
    [](const Config &config) -> std::shared_ptr<Audio> { \
        ESP_UTILS_LOG_TRACE_ENTER(); \
        std::shared_ptr<Audio> device = nullptr; \
        ESP_UTILS_CHECK_EXCEPTION_RETURN( \
            (device = std::make_shared<Audio ##type_name>(config)), nullptr, "Create " #type_name " failed" \
        ); \
        ESP_UTILS_LOG_TRACE_EXIT(); \
        return device; \
    }

const std::unordered_map<std::string, AudioFactory::FunctionDeviceConstructor> AudioFactory::_type_constructor_map = {
    {"PCM5102", DEVICE_CREATOR(PCM5102)},
    {"MAX98357A", DEVICE_CREATOR(MAX98357A)},
};

std::shared_ptr<Audio> AudioFactory::create(const std::string &device_name, const Config &config) {
    ESP_UTILS_LOG_TRACE_ENTER();

    auto it = _type_constructor_map.find(device_name);
    ESP_UTILS_CHECK_FALSE_RETURN(it != _type_constructor_map.end(), nullptr, "Unsupported audio device: %s", device_name.c_str());

    std::shared_ptr<Audio> device = it->second(config);
    ESP_UTILS_CHECK_NULL_RETURN(device, nullptr, "Create audio device failed");

    ESP_UTILS_LOG_TRACE_EXIT();

    return device;
}

} // namespace esp_panel::drivers
