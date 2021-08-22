#include "utils.h"


void loid::system::rendering::check_vulkan_call(VkResult result, const char* exc_text) {
    if (result != VK_SUCCESS) {
        throw VulkanException(result, exc_text);
    }
}
