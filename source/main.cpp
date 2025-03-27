#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_hints.h>

#include <gsl-lite/gsl-lite.hpp>
#include <spdlog/spdlog.h>
#include <webgpu/webgpu.h>

#include <utility>

import xuan.app;

SDL_AppResult
SDL_AppInit(gsl::owner<void *> *appstate, [[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
	spdlog::set_level(spdlog::level::debug);
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	SDL_Init(SDL_INIT_VIDEO);
	*appstate = new xuan::App{};

	return SDL_APP_CONTINUE;
}

SDL_AppResult
SDL_AppIterate(void *appstate)
{
	return static_cast<xuan::App *>(appstate)->iterate();
}

SDL_AppResult
SDL_AppEvent(void *appstate, SDL_Event *event)
{
	return static_cast<xuan::App *>(appstate)->event(*event);
}

void
SDL_AppQuit(gsl::owner<void *> appstate, SDL_AppResult result)
{
	switch (result)
	{
	case SDL_APP_SUCCESS: spdlog::debug("Quit with success"); break;
	case SDL_APP_FAILURE: spdlog::error("Quit with failure"); break;
	case SDL_APP_CONTINUE: std::unreachable();
	}
	delete static_cast<xuan::App *>(appstate);
}