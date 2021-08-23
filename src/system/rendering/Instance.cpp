#include "Instance.h"
#include "utils.h"

#include <vector>

#ifdef NDEBUG
    constexpr static bool ENABLE_VALIDATION_LAYERS = false;
#else
    constexpr static bool ENABLE_VALIDATION_LAYERS = true;

    const std::vector<const char *> required_validation_layers = {
        "VK_LAYER_KHRONOS_validation"
    };

#endif

loid::system::rendering::Instance::Instance() {
    if constexpr (ENABLE_VALIDATION_LAYERS) {
        if (!this->check_validation_layer_support()) {
            throw RenderingError("Validation layers requested, but not available!");
        }
    }
    
    instance_wrapper = std::make_shared<wrapper::InstanceWrapper>();
    debug_messanger_wrapper = std::make_shared<wrapper::DebugMessangerWrapper>(instance_wrapper);
}

bool loid::system::rendering::Instance::check_validation_layer_support() const noexcept {
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    std::vector<std::string> available_layers_names{};
    std::transform(available_layers.cbegin(), available_layers.cend(), std::back_inserter(available_layers_names), [](const VkLayerProperties& layer) -> std::string { return layer.layerName; });

    for (auto layer_name : required_validation_layers) {
        if (std::find(available_layers_names.begin(), available_layers_names.end(), layer_name) == available_layers_names.end()) {
            return false;
        }
    }

    return true;
}
