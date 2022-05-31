#include <stdio.h>

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
	/*for (int i = 0; i < 27; i++)
	{
		title.PushBack({ i * 304, 0, 304, 224 });
	}
	title.loop = false;
	title.speed = 0.35f;*/
}

SceneTitle::~SceneTitle()
{

}

// Load assets
bool SceneTitle::Start()
{
	/*title.Reset();*/
	/*selectFx = 0;*/
	LOG("Loading background assets");

	bool ret = true;
	hasPlayed = false;

	char z[64];

	for (int i = 0; i < NUM_IMAGES; i++) {
		sprintf_s(z, "Assets/Sprites/UI/Title/title%d.png", i);
		bgTexture[i] = App->textures->Load(z);
	}
	frame = 0;
	timer = 0;

	/*bgTexture = App->textures->Load("Assets/Sprites/UI/titleScreen.png");
	currentAnimation = &title;*/
	//SILENT AUDIO per aturar la música de IntroScreen
	/*App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");
	selectFx = App->audio->LoadFx("Assets/FX/Select.wav");*/

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status SceneTitle::Update()
{
	if (timer == 2) {
		if (frame < NUM_IMAGES - 1) {
			frame++;
			timer = 0;
		}
	}
	timer++;
	
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		if (!hasPlayed)
		{
			//App->audio->PlayFx(selectFx);
			hasPlayed = true;
		}
		
		App->fade->FadeToBlack(this, (Module*)App->sceneCharacterSelect, 30);
	}

	//currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneTitle::PostUpdate()
{
	// Draw everything --------------------------------------
	//SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->render->Blit(bgTexture[frame], 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}