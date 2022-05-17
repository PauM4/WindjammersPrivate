#include "SceneTitle.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

#include "SDL/include/SDL.h"

SceneTitle::SceneTitle(bool startEnabled) : Module(startEnabled)
{
	for (int i = 0; i < 27; i++)
	{
		title.PushBack({ i * 304, 0, 304, 224 });
	}
	title.loop = false;
	title.speed = 0.35f;
}

SceneTitle::~SceneTitle()
{

}

// Load assets
bool SceneTitle::Start()
{
	title.Reset();
	selectFx = 0;
	LOG("Loading background assets");

	bool ret = true;
	hasPlayed = false;

	bgTexture = App->textures->Load("Assets/Sprites/UI/titleScreen.png");
	currentAnimation = &title;
	//SILENT AUDIO per aturar la música de IntroScreen
	App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");
	selectFx = App->audio->LoadFx("Assets/FX/Select.wav");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status SceneTitle::Update()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		if (!hasPlayed)
		{
			App->audio->PlayFx(selectFx);
			hasPlayed = true;
		}
		
		App->fade->FadeToBlack(this, (Module*)App->sceneCharacterSelect, 30);
	}

	currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneTitle::PostUpdate()
{
	// Draw everything --------------------------------------
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->render->Blit(bgTexture, 0, 0, &rect);

	return Update_Status::UPDATE_CONTINUE;
}