module;

#include <webgpu/webgpu.h>

export module xuan.engine;

export namespace xuan
{

class Engine;

class View
{
  public:
	virtual ~View() = default;

	virtual void
	render(Engine &) = 0;
};

class Engine
{
  public:
	Engine(WGPUDevice device, WGPUSurface surface) : _device{device}, _surface{surface} {}

	~Engine()
	{
		wgpuSurfaceRelease(_surface);
		wgpuDeviceRelease(_device);
	}

  private:
	WGPUDevice _device;
	WGPUSurface _surface;
};

class TextView : public View
{
	void
	render(Engine &engine) override
	{
	}
};

} // namespace xuan