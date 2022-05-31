#include "SceneNeogeo.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

#include "SDL/include/SDL.h"

SceneNeogeo::SceneNeogeo(bool startEnabled) : Module(startEnabled)
{
	
	for (int i = 0; i < 50; i++)
	{
		neogeo.PushBack({ i * 304, 0, 304, 224 });
	}
	neogeo.loop = false;
	neogeo.speed = 0.3f;
}

SceneNeogeo::~SceneNeogeo()
{

}

// Load assets
bool SceneNeogeo::Start()
{
	//selectFx = 0;
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/UI/neogeo.png");
	currentAnimation = &neogeo;
	//SILENT AUDIO per aturar la música de IntroScreen
	App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");
	//selectFx = App->audio->LoadFx("Assets/FX/Select.wav");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status SceneNeogeo::Update()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->audio->PlayFx(selectFx);
		App->fade->FadeToBlack(this, (Module*)App->sceneDataeast, 30);
	}

	currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneNeogeo::PostUpdate()
{
	// Draw everything --------------------------------------
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->render->Blit(bgTexture, 0, 0, &rect);
	//App->render->Blit(bgTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}