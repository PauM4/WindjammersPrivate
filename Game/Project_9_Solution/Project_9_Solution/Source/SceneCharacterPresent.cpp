#include "SceneCharacterPresent.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include "SceneStageSelect.h"

#include "SDL/include/SDL.h"

SceneCharacterPresent::SceneCharacterPresent(bool startEnabled) : Module(startEnabled)
{
	bgEffectAnim.PushBack({ 0, 0, 304, 224 });
	bgEffectAnim.PushBack({ 0, 224, 304, 224 });
	bgEffectAnim.PushBack({ 0, 672, 304, 224 });
	bgEffectAnim.loop = true;
	bgEffectAnim.pingpong = false;
	bgEffectAnim.speed = 0.8f;
}

SceneCharacterPresent::~SceneCharacterPresent()
{

}

// Load assets
bool SceneCharacterPresent::Start()
{
	timer = 0;
	scape = false;
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/UI/bgEffect_SpriteSheet.png");

	charactersPresentTexture = App->textures->Load("Assets/Sprites/UI/charactersPresent.png");
	App->audio->PlayMusic("Assets/Music/02_Go for Broke! (Round Start).ogg", 0.1f);
	//App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");

	//Debug Font
	char lookupTable[] = { "! ?,_./0123456789?;<??ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
	debugFont = App->fonts->Load("Assets/Sprites/UI/Fonts/debugFont.png", lookupTable, 2);
	isDebugAppear = false;

	currentAnimation = &bgEffectAnim;

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status SceneCharacterPresent::Update()
{
	//4s
	if (timer < 240)
	{
		timer++;
	}
	else if (timer == 240)
	{
		scape = true;
	}

	if (scape)
	{
		App->fade->FadeToBlack(this, (Module*)App->sceneBeachStage, 15);
	}
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->fade->FadeToBlack(this, (Module*)App->sceneBeachStage, 15);
	}

	if (App->input->keys[SDL_SCANCODE_F5] == Key_State::KEY_DOWN)
	{
		if (!isDebugAppear)
			isDebugAppear = true;
		else isDebugAppear = false;
	}

	currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status SceneCharacterPresent::PostUpdate()
{
	// Draw everything --------------------------------------
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->render->Blit(bgTexture, 0, 0, &rect);
	App->render->Blit(charactersPresentTexture, 0, 0, NULL);

	//DEBUG QUIN ESCENARI
	if (isDebugAppear)
	{
		if (App->sceneStageSelect->sceneSelected == Beach)
		{
			App->fonts->BlitText(72, 190, debugFont, "BEACH STAGE");
		}
		else if (App->sceneStageSelect->sceneSelected == Lawn)
		{
			App->fonts->BlitText(72, 190, debugFont, "LAWN STAGE");
		}
		else if (App->sceneStageSelect->sceneSelected == Concrete)
		{
			App->fonts->BlitText(72, 190, debugFont, "CONCRETE STAGE");
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

