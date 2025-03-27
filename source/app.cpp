module;

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <webgpu/webgpu.h>

#include <libassert/assert.hpp>
#include <magic_enum/magic_enum_format.hpp>
#include <sdl3webgpu.h>
#include <spdlog/spdlog.h>

#include <span>

export module xuan.app;

import xuan.webgpu;

namespace xuan
{

static void
log_limits(WGPUAdapter adapter)
{
	WGPULimits limits{.nextInChain = nullptr};
	ASSERT(wgpuAdapterGetLimits(adapter, &limits) == WGPUStatus_Success);

	spdlog::debug("Adapter limits:");
	spdlog::debug(std::format(" - maxTextureDimension1D: {}", limits.maxTextureDimension1D));
	spdlog::debug(std::format(" - maxTextureDimension2D: {}", limits.maxTextureDimension2D));
	spdlog::debug(std::format(" - maxTextureDimension3D: {}", limits.maxTextureDimension3D));
	spdlog::debug(std::format(" - maxTextureArrayLayers: {}", limits.maxTextureArrayLayers));
}

static void
log_features(WGPUAdapter adapter)
{
	WGPUSupportedFeatures features;
	wgpuAdapterGetFeatures(adapter, &features);

	spdlog::debug("Adapter features:");
	for (WGPUFeatureName feature : std::span{features.features, features.featureCount})
	{
		spdlog::debug(std::format(
			" - {:02x} ({})", std::to_underlying(feature),
			magic_enum::enum_contains(feature) ? magic_enum::enum_name(feature) : "Extension"));
	}
	wgpuSupportedFeaturesFreeMembers(features);
}

static void
log_properties(WGPUAdapter adapter)
{
	WGPUAdapterInfo info{.nextInChain = nullptr};
	wgpuAdapterGetInfo(adapter, &info);

	spdlog::debug(std::format("vendorID: {}", info.vendorID));
	spdlog::debug(std::format("vendor: {}", std::string_view{info.vendor.data, info.vendor.length}));
	spdlog::debug(std::format("architecture: {}", std::string_view{info.architecture.data, info.architecture.length}));
	spdlog::debug(std::format("deviceID: {}", info.deviceID));
	spdlog::debug(std::format("description: {}", std::string_view{info.description.data, info.description.length}));
	spdlog::debug(std::format("adapterType: {}", info.adapterType));
	spdlog::debug(std::format("backendType: {}", info.backendType));

	wgpuAdapterInfoFreeMembers(info);
}

export class App
{
  public:
	App() : _window(SDL_CreateWindow("xuan", 800, 600, SDL_WINDOW_RESIZABLE))
	{
		WGPUInstanceDescriptor desc{};
		desc.nextInChain = nullptr;

		WGPUInstance instance = DEBUG_ASSERT_VAL(wgpuCreateInstance(&desc));
		WGPUAdapter adapter = requestAdapterSync(instance);
		log_limits(adapter);
		log_features(adapter);
		log_properties(adapter);
		_device = requestDeviceSync(instance, adapter);

		// No longer needed
		wgpuInstanceRelease(instance);
		wgpuAdapterRelease(adapter);

		_surface = DEBUG_ASSERT_VAL(SDL_GetWGPUSurface(instance, _window));
	}

	~App()
	{
		wgpuSurfaceRelease(_surface);
		wgpuDeviceRelease(_device);
	}

	SDL_AppResult
	iterate()
	{
		if (_window == nullptr)
		{
			return SDL_APP_SUCCESS;
		}
		return SDL_APP_CONTINUE;
	}

	SDL_AppResult
	event(SDL_Event const &event)
	{
		switch (event.type)
		{
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			SDL_DestroyWindow(_window);
			_window = nullptr;
			break;
		default: break;
		}
		return SDL_APP_CONTINUE;
	}

  private:
	SDL_Window *_window;
	WGPUDevice _device;
	WGPUSurface _surface;
};

} // namespace xuan