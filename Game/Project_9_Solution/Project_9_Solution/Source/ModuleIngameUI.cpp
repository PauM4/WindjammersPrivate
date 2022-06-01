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
#include "SceneCharacterSelect.h"
#include "SceneStageSelect.h"

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

	set1Rect = { 160, 300, 160, 56 };
	set2Rect = { 320, 300, 160, 56 };
	suddenRect = { 179, 258, 270, 42 };
	setFinalRect = { 0, 300, 160, 56 };
	rectTimer88 = { 0, 0, 15, 15 };

	rectNormes = { 207, 11, 151, 15 };

	p1Rect = { 359, 0, 15, 8 };
	p2Rect = { 374, 0, 16, 8 };

	japanFlagRect = { 460, 0, 15, 9 };
	koreanFlagRect = { 430, 0, 15, 9 };
	germanyFlagRect = { 445, 0, 15, 9 };

	setCountRect = { 315, 27, 78, 26 };
	setNum0 = { 135, 27, 45, 26 };
	setNum1 = { 180, 27, 45, 26 };
	setNum2 = { 225, 27, 45, 26 };
	setNum3 = { 270, 27, 45, 26 };

	rounds = { 0,0,0,0 };

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
	App->render->Blit(uiSpriteTexture, 10, 10, &p1Rect);
	switch (App->sceneCharacterSelect->p1Char)
	{
	case Mita:
		App->render->Blit(uiSpriteTexture, 40, 10, &japanFlagRect);
		break;
	case Yoo:
		App->render->Blit(uiSpriteTexture, 40, 10, &koreanFlagRect);
		break;
	case Wessel:
		App->render->Blit(uiSpriteTexture, 40, 10, &germanyFlagRect);
		break;
	}

	//P2 UI
	App->render->Blit(uiSpriteTexture, 200, 10, &p2Rect);
	switch (App->sceneCharacterSelect->p2Char)
	{
	case Mita:
		App->render->Blit(uiSpriteTexture, 230, 10, &japanFlagRect);
		break;
	case Yoo:
		App->render->Blit(uiSpriteTexture, 230, 10, &koreanFlagRect);
		break;
	case Wessel:
		App->render->Blit(uiSpriteTexture, 230, 10, &germanyFlagRect);
		break;
	}



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

	App->render->Blit(uiSpriteTexture, 150, 150, &rounds);

	//INICI PARTIDA
	if (App->sceneBeachStage->estadoS == App->sceneBeachStage->INICIO)
	{

		////Timer
		//SDL_Rect rectTimer = currentTimerAnim->GetCurrentFrame();
		//App->render->Blit(timerTexture, 144, 13, &rectTimer);

		//12 points / 30 sec
		App->render->Blit(uiSpriteTexture, 77, 168, &rectNormes);

		//Diferents puntuacions
		if (App->sceneStageSelect->sceneSelected == Concrete)
		{
			//puntuació tots gols L
			App->render->Blit(uiSpriteTexture, 12, 26, &cincPuntsL);
			App->render->Blit(uiSpriteTexture, 12, 92, &tresPuntsL);
			App->render->Blit(uiSpriteTexture, 12, 158, &cincPuntsL);

			//puntuació tots gols R
			App->render->Blit(uiSpriteTexture, 232, 26, &cincPuntsR);
			App->render->Blit(uiSpriteTexture, 232, 92, &tresPuntsR);
			App->render->Blit(uiSpriteTexture, 232, 158, &cincPuntsR);
		}
		else
		{
			//puntuació tots gols L
			App->render->Blit(uiSpriteTexture, 7, 30, &tresPuntsL);
			App->render->Blit(uiSpriteTexture, 7, 92, &cincPuntsL);
			App->render->Blit(uiSpriteTexture, 7, 158, &tresPuntsL);

			//puntuació tots gols R
			App->render->Blit(uiSpriteTexture, 236, 30, &tresPuntsR);
			App->render->Blit(uiSpriteTexture, 236, 92, &cincPuntsR);
			App->render->Blit(uiSpriteTexture, 236, 158, &tresPuntsR);
		}

		//Set1

		App->render->Blit(uiSpriteTexture, 72, 80, &set1Rect);

		//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
		/*App->render->Blit(uiSpriteTexture, 72, 80, &set2Rect);
		App->render->Blit(uiSpriteTexture, 72, 80, &setFinalRect);
		App->render->Blit(uiSpriteTexture, 17, 95, &suddenRect);*/

		//Time88

		App->render->Blit(timerTexture, 144, 13, &rectTimer88);

		//Score 00-00 preGame
		App->fonts->BlitText(114, 17, App->player->scoreFont, "0");
		App->fonts->BlitText(130, 17, App->player->scoreFont, "0");
		App->fonts->BlitText(162, 17, App->player->scoreFont, "0");
		App->fonts->BlitText(178, 17, App->player->scoreFont, "0");

		//App->fonts->BlitText(115, 16, scoreFont, scoreText);
	}
	//COMENÇA PARTIDA
	else if (App->sceneBeachStage->estadoS == App->sceneBeachStage->RONDA)
	{
		//Timer
		rectTimer = currentTimerAnim->GetCurrentFrame();
		App->render->Blit(timerTexture, 144, 13, &rectTimer);
	}

	//ENTRE RONDES BLIT DE QUINA RONDA ES
	if (App->sceneBeachStage->roundSpriteAppear == true)
	{
		//1 a 0 __ 0 a 1 SET 2
		if ((App->player->round == 1 && App->player2->round == 0) || (App->player->round == 0 && App->player2->round == 1))
		{
			App->render->Blit(uiSpriteTexture, 72, 80, &set2Rect);
		}
		//1 a 2 __ 2 a 1 __ 1 a 1 SET FINAL
		else if ((App->player->round == 1 && App->player2->round == 2) || (App->player->round == 2 && App->player2->round == 1)
			|| (App->player->round == 1 && App->player2->round == 1))
		{
			App->render->Blit(uiSpriteTexture, 72, 80, &setFinalRect);
		}
		//2 a 2 SET SUDDENDEATH
		else if ((App->player->round == 2 && App->player2->round == 2))
		{
			App->render->Blit(uiSpriteTexture, 17, 94, &suddenRect);
		}
		//3 a 3 DRAW GAME
		else if ((App->player->round == 3 && App->player2->round == 3))
		{
			//DRAW GAME
		}
	}

	//Entre Rondes indicar quants sets porta cadascú
	if (App->sceneBeachStage->setsSpriteAppear == true)
	{
		App->render->Blit(uiSpriteTexture, 113, 124, &setCountRect);

		//Num of sets P1
		if (App->player->round == 0)
		{
			App->render->Blit(uiSpriteTexture, 50, 124, &setNum0);
		}
		else if (App->player->round == 1)
		{
			App->render->Blit(uiSpriteTexture, 50, 124, &setNum1);
		}
		else if (App->player->round == 2)
		{
			App->render->Blit(uiSpriteTexture, 50, 124, &setNum2);
		}
		else if (App->player->round == 3)
		{
			App->render->Blit(uiSpriteTexture, 50, 124, &setNum3);
		}

		//Num of sets P2
		if (App->player2->round == 0)
		{
			App->render->Blit(uiSpriteTexture, 210, 124, &setNum0);
		}
		else if (App->player2->round == 1)
		{
			App->render->Blit(uiSpriteTexture, 210, 124, &setNum1);
		}
		else if (App->player2->round == 2)
		{
			App->render->Blit(uiSpriteTexture, 210, 124, &setNum2);
		}
		else if (App->player2->round == 3)
		{
			App->render->Blit(uiSpriteTexture, 210, 124, &setNum3);
		}

	}


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

		// A "TEST TEXT", escriure el que es vulgui: una string (igual que l'exempel) o la variable debugText,
		// que correspon a la variable que s'hagi posat al quart parametre de sprintf_s, "".

		App->fonts->BlitText(72, 190, debugFont, debugText);
	}

	//Depenent de quin stage is selected (punts gol posicions)
	if (App->sceneStageSelect->sceneSelected == Beach)
	{
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
	}
	else if (App->sceneStageSelect->sceneSelected == Lawn)
	{
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
	}
	else
	{
		switch (App->sceneBeachStage->estadoGolScore)
		{
		case (0):
			App->render->Blit(uiSpriteTexture, 12, 26, &cincPuntsL);
			break;
		case(1):
			App->render->Blit(uiSpriteTexture, 12, 92, &tresPuntsL);
			break;
		case(2):
			App->render->Blit(uiSpriteTexture, 12, 158, &cincPuntsL);
			break;
		case(3):
			App->render->Blit(uiSpriteTexture, 236, 44, &cincPuntsR);
			break;
		case(4):
			App->render->Blit(uiSpriteTexture, 236, 92, &tresPuntsR);
			break;
		case(5):
			App->render->Blit(uiSpriteTexture, 236, 158, &cincPuntsR);
			break;
		case(6):
			break;
		}
	}

	if (isDebugAppear)
	{
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