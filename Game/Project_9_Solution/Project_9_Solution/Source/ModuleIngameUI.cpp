#include "ModuleInGameUI.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollisions.h"
#include "ModulePlayer.h"
#include "ModulePlayer2.h"
#include "ModuleFrisbee.h"
#include "ModuleFonts.h"
#include "SceneBeachStage.h"

#include "SDL/include/SDL.h"

#include <stdio.h>


ModuleInGameUI::ModuleInGameUI(bool startEnabled) : Module(startEnabled)
{

}

ModuleInGameUI::~ModuleInGameUI()
{

}

// Load assets
bool ModuleInGameUI::Start()
{
	rectanguletL = { 0, 357, 29, 3 };
	rectanguletR = { 0, 357, 29, 3 };

	winUIRight = { 224, 86, 100, 32 };
	LoseUIRight = { 393, 27, 99, 27 };

	winUILeft = { 324, 86, 112, 32 };
	LoseUILeft = { 0, 54, 100, 27 };

	//Debug Font
	char lookupTable[] = { "! ?,_./0123456789?;<??ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
	debugFont = App->fonts->Load("Assets/Sprites/UI/Fonts/debugFont.png", lookupTable, 2);
	isDebugAppear = false;

	// Load UI Texture
	uiSpriteTexture = App->textures->Load("Assets/Sprites/UI/UISpriteSheet_Upgrade.png");
	bothCharactersTexture = App->textures->Load("Assets/Sprites/UI/charactersPresent2.png");



}

Update_Status ModuleInGameUI::Update()
{

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status ModuleInGameUI::PostUpdate()
{
	//P1 UI
	SDL_Rect p1Rect = { 359, 0, 15, 8 };
	App->render->Blit(uiSpriteTexture, 10, 10, &p1Rect);

	//P2 UI
	SDL_Rect p2Rect = { 374, 0, 16, 8 };
	App->render->Blit(uiSpriteTexture, 200, 10, &p2Rect);

	//CANVIAR AMB ENUM DEPENENT DEL PLAYER
	//Japan Flag x2
	SDL_Rect japanFlagRect = { 460, 0, 15, 9 };
	App->render->Blit(uiSpriteTexture, 40, 10, &japanFlagRect);
	App->render->Blit(uiSpriteTexture, 230, 10, &japanFlagRect);
	
	//Mostra qui ha guanyat i qui ha perdut al final de la partida
	//estadoS nomes depen de sceneBeach, cal canviar
	if (App->sceneBeachStage->estadoS == App->sceneBeachStage->FINAL)
	{
		if (App->sceneBeachStage->winState == 1) {

			App->render->Blit(bothCharactersTexture, 0, 0, NULL);
			App->render->Blit(uiSpriteTexture, 18, 48, &winUILeft);
			App->render->Blit(uiSpriteTexture, 175, 54, &LoseUIRight);
			//winState = 4;

		}
		else if (App->sceneBeachStage->winState == 2) {

			App->render->Blit(bothCharactersTexture, 0, 0, NULL);
			App->render->Blit(uiSpriteTexture, 176, 48, &winUIRight);
			App->render->Blit(uiSpriteTexture, 30, 54, &LoseUILeft);
			//winState = 4;

		}
		else if (App->sceneBeachStage->winState == 3) {

			App->render->Blit(bothCharactersTexture, 0, 0, NULL);
			App->render->Blit(uiSpriteTexture, 18, 48, &LoseUIRight);
			App->render->Blit(uiSpriteTexture, 30, 54, &LoseUILeft);
			//winState = 4;
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

bool ModuleInGameUI::CleanUp()
{

	return true;
}