#include "SceneDataeast.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

#include "SDL/include/SDL.h"

SceneDataeast::SceneDataeast(bool startEnabled) : Module(startEnabled)
{

	for (int i = 0; i < 12; i++)
	{
		dataeast.PushBack({ i * 304, 0, 304, 224 });
	}
	dataeast.loop = false;
	dataeast.speed = 0.7f;
}

SceneDataeast::~SceneDataeast()
{

}

// Load assets
bool SceneDataeast::Start()
{
	//selectFx = 0;
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/UI/dataeast.png");
	currentAnimation = &dataeast;
	//SILENT AUDIO per aturar la música de IntroScreen
	App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");
	//selectFx = App->audio->LoadFx("Assets/FX/Select.wav");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status SceneDataeast::Update()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->audio->PlayFx(selectFx);
		App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 30);
	}

	currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneDataeast::PostUpdate()
{
	// Draw everything --------------------------------------
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->render->Blit(bgTexture, 0, 0, &rect);
	//App->render->Blit(bgTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}