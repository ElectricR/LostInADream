#include "RenderSystem.h"

#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

loid::system::Renderer::~Renderer() {
    vkDeviceWaitIdle(logical_device->get_device());
}

loid::system::Renderer::Renderer() {
    window = std::make_unique<rendering::Window>();
    vulkan_instance = std::make_shared<rendering::Instance>();
    window->create_surface(vulkan_instance);
    logical_device = std::make_shared<rendering::LogicalDevice>(vulkan_instance->get_instance(), window->get_surface());
    swap_chain = std::make_unique<rendering::SwapChain>(*window, logical_device);
}
/*
void loid::system::Renderer::_draw_temp() {
    vkWaitForFences(logical_device.get_device(), 1, &in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);

    uint32_t image_index;
    VkResult result = vkAcquireNextImageKHR(logical_device.get_device(), swap_chain, UINT64_MAX, image_available_semaphores[current_frame], VK_NULL_HANDLE, &image_index);
    
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreate_swap_chain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    if (in_flight_images[image_index] != VK_NULL_HANDLE) {
        vkWaitForFences(device, 1, &in_flight_images[image_index], VK_TRUE, UINT64_MAX);
    }

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore wait_semaphores[] = {image_available_semaphores[current_frame]};
    VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffers[image_index];

    VkSemaphore signal_semaphores[] = {render_finished_semaphores[current_frame]};

    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    vkResetFences(device, 1, &in_flight_fences[current_frame]);

    if (vkQueueSubmit(graphics_queue, 1, &submit_info, in_flight_fences[current_frame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphores;

    VkSwapchainKHR swap_chains[] = {swap_chain};
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swap_chains;
    present_info.pImageIndices = &image_index;

    result = vkQueuePresentKHR(present_queue, &present_info);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebuffer_resized) {
        framebuffer_resized = false;
        recreate_swap_chain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}



void loid::system::Renderer::render() {
    glfwPollEvents();
    this->_draw_temp();
}

*/
