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
#include "SceneStageSelect.h"
#include "SceneBeachStage.h"
#include "SceneCharacterSelect.h"

#include "SDL/include/SDL.h"

#include <stdio.h>


ModuleInGameUI::ModuleInGameUI(bool startEnabled) : Module(startEnabled)
{
	//Load animation Timer test
	int posX = 15;
	for (int i = 0; i < 32; ++i)
	{
		timerAnim.PushBack({ posX, 0, 15, 15 });
		posX += 15;
	}
	timerAnim.loop = false;
	timerAnim.pingpong = false;
	timerAnim.speed = 0.017f;

}

ModuleInGameUI::~ModuleInGameUI()
{

}

// Load assets
bool ModuleInGameUI::Start()
{
	bool ret = true;

	rectanguletL = { 0, 357, 29, 3 };
	rectanguletR = { 0, 357, 29, 3 };

	winUIRight = { 224, 86, 100, 32 };
	LoseUIRight = { 393, 27, 99, 27 };

	winUILeft = { 324, 86, 112, 32 };
	LoseUILeft = { 0, 54, 100, 27 };

	tresPuntsL = { 0, 188, 63, 34 };
	cincPuntsL = { 76, 258, 63, 35 };

	tresPuntsR = { 112, 120, 63, 34 };
	cincPuntsR = { 74, 224, 63, 35 };

	//Debug Font
	char lookupTable[] = { "! ?,_./0123456789?;<??ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
	debugFont = App->fonts->Load("Assets/Sprites/UI/Fonts/debugFont.png", lookupTable, 2);
	isDebugAppear = false;

	// Load UI Texture
	uiSpriteTexture = App->textures->Load("Assets/Sprites/UI/UISpriteSheet_Upgrade.png");
	bothCharactersTexture = App->textures->Load("Assets/Sprites/UI/charactersPresent2.png");

	// Load timer texture
	timerTexture = App->textures->Load("Assets/Sprites/UI/Fonts/timerSpriteSheet.png");
	currentTimerAnim = &timerAnim;

	return ret;
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
	case Beach:
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

		//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

		// Rectangulets. S'encen si a la seguent ronda el player guanyara
		if (App->player->round > App->player2->round)
		{
			App->render->Blit(uiSpriteTexture, 113, 12, &rectanguletL);
		}
		else if (App->player->round < App->player2->round)
		{
			App->render->Blit(uiSpriteTexture, 161, 12, &rectanguletR);
		}
		else if (App->player->round == App->player2->round && App->player->round >= 1 && App->player2->round >= 1)
		{
			App->render->Blit(uiSpriteTexture, 113, 12, &rectanguletL);
			App->render->Blit(uiSpriteTexture, 161, 12, &rectanguletR);
		}

		//INICI PARTIDA
		if (App->sceneBeachStage->estadoS == App->sceneBeachStage->INICIO)
		{

			////Timer
			//SDL_Rect rectTimer = currentTimerAnim->GetCurrentFrame();
			//App->render->Blit(timerTexture, 144, 13, &rectTimer);

			//12 points / 30 sec
			rectNormes = { 207, 11, 151, 15 };
			App->render->Blit(uiSpriteTexture, 77, 168, &rectNormes);

			//puntuaci� tots gols L
			App->render->Blit(uiSpriteTexture, 7, 30, &tresPuntsL);
			App->render->Blit(uiSpriteTexture, 7, 92, &cincPuntsL);
			App->render->Blit(uiSpriteTexture, 7, 158, &tresPuntsL);

			//puntuaci� tots gols R
			App->render->Blit(uiSpriteTexture, 236, 30, &tresPuntsR);
			App->render->Blit(uiSpriteTexture, 236, 92, &cincPuntsR);
			App->render->Blit(uiSpriteTexture, 236, 158, &tresPuntsR);

			//Set1
			set1Rect = { 160, 300, 160, 56 };
			App->render->Blit(uiSpriteTexture, 72, 80, &set1Rect);

			//Time88
			rectTimer88 = { 0, 0, 15, 15 };
			App->render->Blit(timerTexture, 144, 13, &rectTimer88);

			//Score 00-00 preGame
			App->fonts->BlitText(114, 17, App->player->scoreFont, "0");
			App->fonts->BlitText(130, 17, App->player->scoreFont, "0");
			App->fonts->BlitText(162, 17, App->player->scoreFont, "0");
			App->fonts->BlitText(178, 17, App->player->scoreFont, "0");

			//App->fonts->BlitText(115, 16, scoreFont, scoreText);
		}
		//COMEN�A PARTIDA
		else if (App->sceneBeachStage->estadoS == App->sceneBeachStage->RONDA)
		{
			//Timer
			rectTimer = currentTimerAnim->GetCurrentFrame();
			App->render->Blit(timerTexture, 144, 13, &rectTimer);
		}

		App->render->Blit(uiSpriteTexture, 150, 150, &rounds);

		if (App->input->keys[SDL_SCANCODE_F5] == Key_State::KEY_DOWN)
		{
			if (!isDebugAppear)
				isDebugAppear = true;
			else isDebugAppear = false;
		}

		if (isDebugAppear)
		{
			// Despres de qualsevol blit perque estigui per sobre de tot
			// En "", posar la variable que es vulgui imprimir per pantalla (scoreExemple)
			// Si no es fan servir variables, comentar aquesta linia
			sprintf_s(debugText, 10, "%2d", App->sceneBeachStage->initialTimeS);

	SDL_Rect rounds = { 0,0,0,0 };
	App->render->Blit(uiSpriteTexture, 150, 150, &rounds);

			App->fonts->BlitText(72, 190, debugFont, debugText);
			App->fonts->BlitText(0, 0, debugFont, "BEACH STAGE");
		}

		switch (App->sceneBeachStage->estadoGolScore)
		{
		case (0):
			App->render->Blit(uiSpriteTexture, 7, 30, &tresPuntsL);
			break;
		case(1):
			App->render->Blit(uiSpriteTexture, 7, 92, &cincPuntsL);
			break;
		case(2):
			App->render->Blit(uiSpriteTexture, 7, 158, &tresPuntsL);
			break;
		case(3):
			App->render->Blit(uiSpriteTexture, 236, 30, &tresPuntsR);
			break;
		case(4):
			App->render->Blit(uiSpriteTexture, 236, 92, &cincPuntsR);
			break;
		case(5):
			App->render->Blit(uiSpriteTexture, 236, 158, &tresPuntsR);
			break;
		case(6):
			break;
		}

		if (App->sceneBeachStage->estadoTGol == App->sceneBeachStage->FINGOL)
		{
			App->fonts->BlitText(72, 200, debugFont, "FINGOL");
		}
		if (App->sceneBeachStage->estadoTGol == App->sceneBeachStage->EJECUTANDOGOL)
		{
			App->fonts->BlitText(72, 200, debugFont, "EJECUTANDOGOL");
		}
		if (App->sceneBeachStage->estadoTGol == App->sceneBeachStage->INICIOGOL)
		{
			App->fonts->BlitText(72, 200, debugFont, "INICIOGOL");
		}
		sprintf_s(debugText, 10, "%2d", App->sceneBeachStage->timeLimitGol);
		App->fonts->BlitText(72, 210, debugFont, debugText);
		sprintf_s(debugText, 10, "%2d", App->sceneBeachStage->currentTimeGol);
		App->fonts->BlitText(112, 210, debugFont, debugText);

		break;
	case Lawn:
		if (App->sceneLawnStage->estadoS == App->sceneLawnStage->FINAL)
		{
			if (App->sceneLawnStage->winState == 1) {

				App->render->Blit(bothCharactersTexture, 0, 0, NULL);
				App->render->Blit(uiSpriteTexture, 18, 48, &winUILeft);
				App->render->Blit(uiSpriteTexture, 175, 54, &LoseUIRight);
				//winState = 4;

			}
			else if (App->sceneLawnStage->winState == 2) {

				App->render->Blit(bothCharactersTexture, 0, 0, NULL);
				App->render->Blit(uiSpriteTexture, 176, 48, &winUIRight);
				App->render->Blit(uiSpriteTexture, 30, 54, &LoseUILeft);
				//winState = 4;

			}
			else if (App->sceneLawnStage->winState == 3) {

				App->render->Blit(bothCharactersTexture, 0, 0, NULL);
				App->render->Blit(uiSpriteTexture, 18, 48, &LoseUIRight);
				App->render->Blit(uiSpriteTexture, 30, 54, &LoseUILeft);
				//winState = 4;
			}
		}

		//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

		// Rectangulets. S'encen si a la seguent ronda el player guanyara
		if (App->player->round > App->player2->round)
		{
			App->render->Blit(uiSpriteTexture, 113, 12, &rectanguletL);
		}
		else if (App->player->round < App->player2->round)
		{
			App->render->Blit(uiSpriteTexture, 161, 12, &rectanguletR);
		}
		else if (App->player->round == App->player2->round && App->player->round >= 1 && App->player2->round >= 1)
		{
			App->render->Blit(uiSpriteTexture, 113, 12, &rectanguletL);
			App->render->Blit(uiSpriteTexture, 161, 12, &rectanguletR);
		}

		//INICI PARTIDA
		if (App->sceneLawnStage->estadoS == App->sceneLawnStage->INICIO)
		{

			////Timer
			//SDL_Rect rectTimer = currentTimerAnim->GetCurrentFrame();
			//App->render->Blit(timerTexture, 144, 13, &rectTimer);

			//12 points / 30 sec
			rectNormes = { 207, 11, 151, 15 };
			App->render->Blit(uiSpriteTexture, 77, 168, &rectNormes);

			//puntuaci� tots gols L
			App->render->Blit(uiSpriteTexture, 7, 30, &tresPuntsL);
			App->render->Blit(uiSpriteTexture, 7, 92, &cincPuntsL);
			App->render->Blit(uiSpriteTexture, 7, 158, &tresPuntsL);

			//puntuaci� tots gols R
			App->render->Blit(uiSpriteTexture, 236, 30, &tresPuntsR);
			App->render->Blit(uiSpriteTexture, 236, 92, &cincPuntsR);
			App->render->Blit(uiSpriteTexture, 236, 158, &tresPuntsR);

			//Set1
			set1Rect = { 160, 300, 160, 56 };
			App->render->Blit(uiSpriteTexture, 72, 80, &set1Rect);

			//Time88
			rectTimer88 = { 0, 0, 15, 15 };
			App->render->Blit(timerTexture, 144, 13, &rectTimer88);

			//Score 00-00 preGame
			App->fonts->BlitText(114, 17, App->player->scoreFont, "0");
			App->fonts->BlitText(130, 17, App->player->scoreFont, "0");
			App->fonts->BlitText(162, 17, App->player->scoreFont, "0");
			App->fonts->BlitText(178, 17, App->player->scoreFont, "0");

			//App->fonts->BlitText(115, 16, scoreFont, scoreText);
		}
		//COMEN�A PARTIDA
		else if (App->sceneLawnStage->estadoS == App->sceneLawnStage->RONDA)
		{
			//Timer
			rectTimer = currentTimerAnim->GetCurrentFrame();
			App->render->Blit(timerTexture, 144, 13, &rectTimer);
		}

		App->render->Blit(uiSpriteTexture, 150, 150, &rounds);

		if (App->input->keys[SDL_SCANCODE_F5] == Key_State::KEY_DOWN)
		{
			if (!isDebugAppear)
				isDebugAppear = true;
			else isDebugAppear = false;
		}

		if (isDebugAppear)
		{
			// Despres de qualsevol blit perque estigui per sobre de tot
			// En "", posar la variable que es vulgui imprimir per pantalla (scoreExemple)
			// Si no es fan servir variables, comentar aquesta linia
			sprintf_s(debugText, 10, "%2d", App->sceneLawnStage->initialTimeS);

			// A "TEST TEXT", escriure el que es vulgui: una string (igual que l'exempel) o la variable debugText,
			// que correspon a la variable que s'hagi posat al quart parametre de sprintf_s, "".

			App->fonts->BlitText(72, 190, debugFont, debugText);
			App->fonts->BlitText(0, 0, debugFont, "LAWN STAGE");
		}

		switch (App->sceneLawnStage->estadoGolScore)
		{
		case (0):
			App->render->Blit(uiSpriteTexture, 7, 30, &tresPuntsL);
			break;
		case(1):
			App->render->Blit(uiSpriteTexture, 7, 92, &cincPuntsL);
			break;
		case(2):
			App->render->Blit(uiSpriteTexture, 7, 158, &tresPuntsL);
			break;
		case(3):
			App->render->Blit(uiSpriteTexture, 236, 30, &tresPuntsR);
			break;
		case(4):
			App->render->Blit(uiSpriteTexture, 236, 92, &cincPuntsR);
			break;
		case(5):
			App->render->Blit(uiSpriteTexture, 236, 158, &tresPuntsR);
			break;
		case(6):
			break;
		}

		if (App->sceneLawnStage->estadoTGol == App->sceneLawnStage->FINGOL)
		{
			App->fonts->BlitText(72, 200, debugFont, "FINGOL");
		}
		if (App->sceneLawnStage->estadoTGol == App->sceneLawnStage->EJECUTANDOGOL)
		{
			App->fonts->BlitText(72, 200, debugFont, "EJECUTANDOGOL");
		}
		if (App->sceneLawnStage->estadoTGol == App->sceneLawnStage->INICIOGOL)
		{
			App->fonts->BlitText(72, 200, debugFont, "INICIOGOL");
		}
		sprintf_s(debugText, 10, "%2d", App->sceneLawnStage->timeLimitGol);
		App->fonts->BlitText(72, 210, debugFont, debugText);
		sprintf_s(debugText, 10, "%2d", App->sceneLawnStage->currentTimeGol);
		App->fonts->BlitText(112, 210, debugFont, debugText);

		break;
	}

	return Update_Status::UPDATE_CONTINUE;
}

bool ModuleInGameUI::CleanUp()
{

	return true;
}

void ModuleInGameUI::TimerS() {
	currentTimeS = SDL_GetTicks();

	if (currentTimeS - initialTimeS >= timeLimitS) {
		estadoTS = estadoTimerS::FIN;
	}
}