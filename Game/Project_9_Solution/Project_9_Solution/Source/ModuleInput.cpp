#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"

ModuleInput::ModuleInput(bool startEnabled) : Module(startEnabled)
{}

ModuleInput::~ModuleInput()
{}

bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// Initialize Controller
	/*num_controllers = SDL_NumJoysticks();
	for (int i = 0; i < num_controllers; ++i)
		if (SDL_IsGameController(i)) {
			sdl_controllers[i] = SDL_GameControllerOpen(i);
		}
			*/

	return ret;
}

Update_Status ModuleInput::PreUpdate()
{
	//Read new SDL events, mostly from the window
	//SDL_Event event;
	//if (SDL_PollEvent(&event))
	//{
	//	if (event.type == SDL_QUIT)	return Update_Status::UPDATE_STOP;
	//}

	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || keys[SDL_SCANCODE_ESCAPE] == Key_State::KEY_REPEAT /*|| App->input->pads->l1*/)	return Update_Status::UPDATE_STOP;

	}

	//Read all keyboard data and update our custom array
	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	//SDL_GameControllerUpdate();
	//for (int i = 0; i < num_controllers; ++i)
	//{
	//
	//	controllers[i].a = SDL_GameControllerGetButton(sdl_controllers[i],SDL_CONTROLLER_BUTTON_A); //lanzamiento normal
	//	controllers[i].b = SDL_GameControllerGetButton(sdl_controllers[i],SDL_CONTROLLER_BUTTON_B); //bloqueo y supershot
	//	controllers[i].x = SDL_GameControllerGetButton(sdl_controllers[i],SDL_CONTROLLER_BUTTON_X); //bolea
	//	controllers[i].y = SDL_GameControllerGetButton(sdl_controllers[i],SDL_CONTROLLER_BUTTON_Y); //pasar pantalla
	//	controllers[i].dpad_up = SDL_GameControllerGetButton(sdl_controllers[i], SDL_CONTROLLER_BUTTON_DPAD_UP);
	//	controllers[i].dpad_down = SDL_GameControllerGetButton(sdl_controllers[i], SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	//	controllers[i].dpad_left = SDL_GameControllerGetButton(sdl_controllers[i], SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	//	controllers[i].dpad_right = SDL_GameControllerGetButton(sdl_controllers[i], SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	//	controllers[i].start = SDL_GameControllerGetButton(sdl_controllers[i], SDL_CONTROLLER_BUTTON_START); //escape

	//}

	

	return Update_Status::UPDATE_CONTINUE;
}




bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");

	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
