// Helper functions

module;

#include <libassert/assert.hpp>
#include <webgpu/webgpu.h>

#include <future>
#include <stdexcept>

export module xuan.webgpu;

export namespace xuan
{

WGPUAdapter
requestAdapterSync(WGPUInstance instance)
{
	std::promise<WGPUAdapter> promise;
	std::future<WGPUAdapter> future = promise.get_future();

	WGPURequestAdapterOptions options{
		.nextInChain = nullptr,
	};

	WGPURequestAdapterCallbackInfo info{
		.nextInChain = nullptr,
		.mode = WGPUCallbackMode_AllowSpontaneous,
		.callback =
			[](WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message, void *userdata1, void *)
		{
			std::promise<WGPUAdapter> &promise = *static_cast<std::promise<WGPUAdapter> *>(userdata1);
			if (status == WGPURequestAdapterStatus_Success)
			{
				promise.set_value(DEBUG_ASSERT_VAL(adapter));
			}
			else
			{
				promise.set_exception(std::make_exception_ptr(std::runtime_error{{message.data, message.length}}));
			}
		},
		.userdata1 = &promise,
	};

	std::ignore = wgpuInstanceRequestAdapter(instance, &options, info);
	wgpuInstanceProcessEvents(instance);

	return future.get();
}

WGPUDevice
requestDeviceSync(WGPUInstance instance, WGPUAdapter adapter)
{

	std::promise<WGPUDevice> promise;
	std::future<WGPUDevice> future = promise.get_future();

	WGPUDeviceDescriptor descriptor{
		.nextInChain = nullptr,
	};

	WGPURequestDeviceCallbackInfo info{
		.nextInChain = nullptr,
		.mode = WGPUCallbackMode_AllowSpontaneous,
		.callback =
			[](WGPURequestDeviceStatus status, WGPUDevice adapter, WGPUStringView message, void *userdata1, void *)
		{
			std::promise<WGPUDevice> &promise = *static_cast<std::promise<WGPUDevice> *>(userdata1);
			if (status == WGPURequestDeviceStatus_Success)
			{
				promise.set_value(DEBUG_ASSERT_VAL(adapter));
			}
			else
			{
				promise.set_exception(std::make_exception_ptr(std::runtime_error{{message.data, message.length}}));
			}
		},
		.userdata1 = &promise,
	};

	std::ignore = wgpuAdapterRequestDevice(adapter, &descriptor, info);
	wgpuInstanceProcessEvents(instance);

	return future.get();
}

} // namespace xuan