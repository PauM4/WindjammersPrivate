﻿#include "SceneLawnStage.h"

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
#include "SceneCharacterPresent.h"
#include "ModuleIngameUI.h"

#include "SDL/include/SDL.h"

#include <stdio.h>


SceneLawnStage::SceneLawnStage(bool startEnabled) : Module(startEnabled)
{
	//Load lawn bg animation
	bgLawnAnim.PushBack({ 0, 0, 304, 224 });
	bgLawnAnim.PushBack({ 304, 0, 304, 224 });
	bgLawnAnim.PushBack({ 608, 0, 304, 224 });
	bgLawnAnim.PushBack({ 0, 224, 304, 224 });
	//bgLawnAnim.PushBack({ 304, 224, 304, 224 });
	bgLawnAnim.speed = 0.2f;



}

SceneLawnStage::~SceneLawnStage()
{

}

// Load assets
bool SceneLawnStage::Start()
{
	round1FX = 0;
	time = 0;
	LOG("Loading background assets");

	rectLawn = { 0, 0, 304, 224 };

	//Debug Font
	char lookupTable[] = { "! ?,_./0123456789?;<??ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
	debugFont = App->fonts->Load("Assets/Sprites/UI/Fonts/debugFont.png", lookupTable, 2);
	isDebugAppear = false;

	bool ret = true;

	suddenDeath = false;
	initialTime = 0;
	startTheGame = false;

	estadoGolScore = CLEAR;

	//FX de rounds
	round1FX = App->audio->LoadFx("Assets/Fx/round1.wav");


	//lawnexture = App->textures->Load("Assets/Sprites/Levels/PH_Lawn.png");
	//bgLawnTexture = App->textures->Load("Assets/Sprites/Levels/bgLawnSpriteSheet.png");
	bgLawnTexture = App->textures->Load("Assets/Sprites/Levels/PH_Lawn.png");
	currentLawnAnim = &bgLawnAnim;

	//CANVIAR MUSICA A LAWN SCENE
	App->audio->PlayMusic("Assets/Music/03_Flying Power Disc (Beach Court).ogg", 1.0f);

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	App->player->Enable();
	App->player2->Enable();
	App->frisbee->Enable();
	App->collisions->Enable();
	App->ingameUI->Enable();

	debugwinP1 = false;
	debugwinP2 = false;
	winState = 0;
	godMode = false;
	estadoS = INICIO;
	estadoTGol == INICIOGOL;
	arbitroFinalRonda = 1;

	return ret;
}

Update_Status SceneLawnStage::Update()
{
	switch (estadoS) {
		//4 segons inicicials (puntuacions, ui general)
	case (INICIO):

		//bullshit animaciones texturas etc - inicio partida, primer momento, solo ocurre una vez en cada partida
		if (estadoTS == INICIOT)
		{
			initialTimeS = SDL_GetTicks();
			timeLimitS = 4 * 1000;
			estadoTS = EJECUTANDO;
		}

		TimerS();
		if (estadoTS == FIN)
		{
			//SceneLawnStage::Arbitro(1);
			estadoS = INICIORONDA;
		}
		break;

		//Iniciar ronda (round 2, final o suddendeath)
	case (INICIORONDA):
		//Animacion Ronda 1.

		estadoTS = estadoTimerS::INICIOT;
		App->ingameUI->timerAnim.Reset();
		App->player->score = 0;
		App->player2->score = 0;
		initialTimeS = SDL_GetTicks();
		timeLimitS = 30 * 1000;
		Arbitro(arbitroFinalRonda);
		estadoS = RONDA;
		break;

		//INGAME Gemplei 
	case (RONDA):
		TimerS();
		Round();
		App->ingameUI->currentTimerAnim->Update();

		//Tendremos que poner una condicion para cuando se marquen puntosq ue aqui se ejecuten unas texuras/animaciones - MARCARPUNTO
		if (estadoTGol == EJECUTANDOGOL)
		{
			TimerGol();
		}
		else if (estadoTGol == FINGOL)
		{
			if (suddenDeath) {
				Win();
			}
			estadoTGol = INICIOGOL;
			Round();
			Arbitro(arbitroFinalRonda);
		}


		break;

		//Animacions qui ha guanyat bailecito chingon
	case (FINALRONDA):
		//Animaciones caundo se gana/pierde una ronda
		//App->player->estadoP1 = App->player->STOP;
		//App->player2->estadoP2 = App->player2->STOP;
		Win();


		break;

		//Animacions qui ha guanyat bailecito chingon i reiniciar
	case (FINAL):
		App->player->estadoP1 = App->player->STOP;
		App->player2->estadoP2 = App->player2->STOP;
		App->frisbee->estadoF = App->frisbee->ARBITROF;
		if (estadoTS == INICIOT)
		{
			initialTimeS = SDL_GetTicks();
			timeLimitS = 4 * 1000;
			estadoTS = EJECUTANDO;
		}

		TimerS();
		if (estadoTS == FIN)
		{
			//SceneLawnStage::Arbitro(1);
			estadoS = INICIO;
			App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);
		}

		break;
	}





	////240 == 4s
	//if (initialTime < 240)
	//{
	//	initialTime++;
	//}
	//else if (initialTime == 240)
	//{
	//	startTheGame = true;
	//	App->audio->PlayFx(round1FX, 0);
	//	//EndRound(1);
	//}

	//if (startTheGame)
	//{
	//	if (time <= 1860)
	//	{
	//		time++;
	//	}
	//	currentTimerAnim->Update();
	//	currentTimerAnim->Update();
	//}

	//currentLawnAnim->Update();

	// DEBUG INSTANT WIN
	if (App->input->keys[SDL_SCANCODE_F3] == Key_State::KEY_DOWN)
	{
		App->audio->PlayMusic("Assets/Music/06_Set Clear.ogg", 0.0f);

		App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");
		//App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);
		debugwinP1 = true;
		Win();
	}
	// DEBUG INSTANT LOSE
	if (App->input->keys[SDL_SCANCODE_F4] == Key_State::KEY_DOWN)
	{
		App->audio->PlayMusic("Assets/Music/09_Lost Set.ogg", 0.0f);

		App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");
		//App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);
		debugwinP2 = true;
		Win();
	}
	if (App->input->keys[SDL_SCANCODE_F1] == Key_State::KEY_DOWN)
	{
		if (estadoS == RONDA) {
			godMode = true;
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneLawnStage::PostUpdate()
{

	// Draw everything --------------------------------------
		//App->render->Blit(beachTexture, 0, 0, NULL);
		//Lawn background

	//SDL_Rect rectLawn = currentLawnAnim->GetCurrentFrame();
	App->render->Blit(bgLawnTexture, 0, 0, &rectLawn);

	//if (winState == 4) {
	//	App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);
	//}

	/*WIIIIIIIIIIIIIIIIIIIIIIN Y LOSEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE*//*WIIIIIIIIIIIIIIIIIIIIIIN Y LOSEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE*//*WIIIIIIIIIIIIIIIIIIIIIIN Y LOSEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE*/


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

		sprintf_s(debugText, 10, "%2d", App->frisbee->position.x - (App->player->position.x + 20));

		// A "TEST TEXT", escriure el que es vulgui: una string (igual que l'exempel) o la variable debugText,
		// que correspon a la variable que s'hagi posat al quart parametre de sprintf_s, "".

		App->fonts->BlitText(72, 190, debugFont, debugText);
	}

	//BLIT 5 punts a ModuleInGameUI.cpp

	return Update_Status::UPDATE_CONTINUE;
}

bool SceneLawnStage::CleanUp()
{
	App->player->Disable();
	App->player2->Disable();
	App->frisbee->Disable();
	App->collisions->Disable();
	App->ingameUI->Disable();

	return true;
}

//En cuanto mete un jugador un gol, se llama a esto y se determina el valor de arbitro. Lo ponemos aqui como funcion externa en vez de dentro del update de frisbee
//Ya que en este .cpp tambi�n llamaremos a esta funci�n en funci�n de las rondas/sets ganados~

void SceneLawnStage::Arbitro(int arbitro) {  //cambiar esta funcion a arbitro
	App->player->position.x = 20;
	App->player->position.y = 100;
	App->player2->position.x = 259;
	App->player2->position.y = 100;
	App->player->currentAnimation = &App->player->idleRAnim;
	App->player2->currentAnimation = &App->player2->idleLAnim;

	if (arbitro == 1) {

		App->frisbee->xspeed = -3;
		App->frisbee->yspeed = -2;
		App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
		App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::ARBITRO;


	}
	else if (arbitro == 2) {

		App->frisbee->xspeed = 3;
		App->frisbee->yspeed = -2;
		App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
		App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::ARBITRO;
	}
}

void SceneLawnStage::Round() {
	if (!godMode) {

		if (App->player->score >= 12 || App->player2->score >= 12) {

			if (App->player->score > App->player2->score + 2) {
				App->player->round += 1;

				//Llamar animaci�n de jugador ganador 1 y las texturas
				App->player->score = 0;
				App->player2->score = 0;
				estadoS = FINALRONDA;
			}

			if (App->player2->score > App->player->score + 2) {
				App->player2->round += 1;

				//Llamar animaci�n de jugador ganador 2 y las texturas
				App->player->score = 0;
				App->player2->score = 0;
				estadoS = FINALRONDA;
			}

		}
		else if (estadoTS == FIN && (App->frisbee->estadoF == ModuleFrisbee::estadoFrisbee::WITHPLAYER)) { //FALTA TIMER


			if (App->player->score > App->player2->score) {
				App->player->round += 1;

				//Llamar animaci�n de jugador ganador 1 y las texturas
				arbitroFinalRonda = 2;
				estadoS = FINALRONDA;

			}
			else if (App->player2->score > App->player->score) {
				App->player2->round += 1;

				//Llamar animaci�n de jugador ganador 2 y las texturas
				arbitroFinalRonda = 1;
				estadoS = FINALRONDA;

			}
			else if (App->player->score == App->player2->score) {
				App->player->round += 1;
				App->player2->round += 1;

				arbitroFinalRonda = 1;
				estadoS = FINALRONDA;

				//Animaci�n de cuando los dos acaban una ronda en puntuacion empate

			}

		}

	}

}

void SceneLawnStage::Win() { //AQUI SE TENDR�A QUE CAMBIAR EL ESTADO EN SWITCH FINAL PARA QUE SE EJECUTEN LAS ANIMACIONES/TEXTURAS CONCRETAS ~ ~~ o ponerlas aquid riectamente las anim/text

	if (App->player->round == App->player2->round && App->player->round == 2 && App->player2->round == 2 && !suddenDeath) {
		suddenDeath = true;
		estadoS = INICIORONDA;

	}
	else if (App->player->score != 0 && suddenDeath) {
		//llamar animaci�n y texturas de que ha ganado el primer jugador la partida
		//SDL Delay
		winState = 1;
		estadoS = FINAL;

	}
	else if (App->player2->score != 0 && suddenDeath) {
		//llamar animaci�n y texturas de que ha ganado el segundo jugador la partida
		//SDL Delay
		winState = 2;
		estadoS = FINAL;

	}
	else if ((App->player->round == 2 && !suddenDeath) || debugwinP1) {
		//llamar animaci�n y texturas de que ha ganado el primer jugador la partida
		//SDL Delay
		winState = 1;
		estadoS = FINAL;
	}
	else if ((App->player2->round == 2 && !suddenDeath) || debugwinP2) {
		//llamar animaci�n y texturas de que ha ganado el segundo jugador la partida
			//SDL Delay
		winState = 2;
		estadoS = FINAL;

	}
	else if (suddenDeath && App->player->score == App->player2->score) {
		//Animacion y texturas de que los dos han perdido
		//SDL Delay
		winState = 3;
		estadoS = FINAL;

	}
	else if (!godMode) {
		estadoS = INICIORONDA;
	}

}

void SceneLawnStage::Score() { //Tendremos que cambiar estado en el switch - MARCARPUNTO, en cada momento en que se meten puntos para que se realicen las animaciones 
	//Score esquerra
	if (App->frisbee->position.x <= 19) {
		//5 punts
		if (App->frisbee->position.y >= 94 && App->frisbee->position.y <= 144) {
			App->player2->score += 5;

			//Just despres d'afegir score, UI Textura d'on ha marcat
			initialTimeGol = SDL_GetTicks();
			timeLimitGol = 2 * 1000;
			estadoTGol = EJECUTANDOGOL;
			arbitroFinalRonda = 1;
			estadoGolScore = MIDLEFT;
		}
		//3 punts adalt
		else if (App->frisbee->position.y < 94)
		{
			App->player2->score += 3;
			//Just despres d'afegir score, UI Textura d'on ha marcat
			initialTimeGol = SDL_GetTicks();
			timeLimitGol = 2 * 1000;
			estadoTGol = EJECUTANDOGOL;
			arbitroFinalRonda = 1;
			estadoGolScore = UPLEFT;
		}
		//3 punts abaix
		else if (App->frisbee->position.y > 144)
		{
			App->player2->score += 3;
			//Just despres d'afegir score, UI Textura d'on ha marcat
			initialTimeGol = SDL_GetTicks();
			timeLimitGol = 2 * 1000;
			estadoTGol = EJECUTANDOGOL;
			arbitroFinalRonda = 1;
			estadoGolScore = DOWNLEFT;
		}
	}
	else if (App->frisbee->position.x >= 276) {
		if (App->frisbee->position.y >= 94 && App->frisbee->position.y <= 144) {
			App->player->score += 5;
			//Just despres d'afegir score, UI Textura d'on ha marcat
			initialTimeGol = SDL_GetTicks();
			timeLimitGol = 2 * 1000;
			estadoTGol = EJECUTANDOGOL;
			arbitroFinalRonda = 2;
			estadoGolScore = MIDRIGHT;
		}
		// 3 punts UP
		else if (App->frisbee->position.y < 94)
		{
			App->player->score += 3;
			//Just despres d'afegir score, UI Textura d'on ha marcat
			initialTimeGol = SDL_GetTicks();
			timeLimitGol = 2 * 1000;
			estadoTGol = EJECUTANDOGOL;
			arbitroFinalRonda = 2;
			estadoGolScore = UPRIGHT;
		}
		else if (App->frisbee->position.y > 144)
		{
			//Just despres d'afegir score, UI Textura d'on ha marcat
			initialTimeGol = SDL_GetTicks();
			timeLimitGol = 2 * 1000;
			estadoTGol = EJECUTANDOGOL;
			arbitroFinalRonda = 2;
			estadoGolScore = DOWNRIGHT;
		}
	}
	else if (App->frisbee->position.x > 19 && App->frisbee->position.x < 150) {
		App->player2->score += 2;
		if (suddenDeath) {
			Win();
		}
		Round();
		Arbitro(1);

	}
	else if (App->frisbee->position.x < 276 && App->frisbee->position.x > 150) {
		App->player->score += 2;
		if (suddenDeath) {
			Win();
		}
		Round();
		Arbitro(2);
	}



	App->frisbee->position.x = 150;
	App->frisbee->position.y = 200;
}

void SceneLawnStage::TimerS() {
	currentTimeS = SDL_GetTicks();

	if (currentTimeS - initialTimeS >= timeLimitS) {
		estadoTS = estadoTimerS::FIN;
	}
}

void SceneLawnStage::TimerGol() {
	currentTimeGol = SDL_GetTicks();

	if (currentTimeGol - initialTimeGol >= timeLimitGol) {
		estadoTGol = estadoTimerGol::FINGOL;

		//Elimina textura quan estadoTGol sigui FINGOL
		estadoGolScore = CLEAR;
	}
}