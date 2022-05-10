#include "SceneBeachStage.h"

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

#include "SDL/include/SDL.h"

#include <stdio.h>


SceneBeachStage::SceneBeachStage(bool startEnabled) : Module(startEnabled)
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

	//Load beach bg animation
	bgBeachAnim.PushBack({ 0, 0, 304, 224 });
	bgBeachAnim.PushBack({ 304, 0, 304, 224 });
	bgBeachAnim.PushBack({ 608, 0, 304, 224 });
	bgBeachAnim.PushBack({ 0, 224, 304, 224 });
	//bgBeachAnim.PushBack({ 304, 224, 304, 224 });
	bgBeachAnim.speed = 0.2f;

	

}

SceneBeachStage::~SceneBeachStage()
{

}

// Load assets
bool SceneBeachStage::Start()
{
	round1FX = 0;
	time = 0;
	LOG("Loading background assets");

	//Debug Font
	char lookupTable[] = { "! ?,_./0123456789?;<??ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
	debugFont = App->fonts->Load("Assets/Sprites/UI/Fonts/debugFont.png", lookupTable, 2);
	isDebugAppear = false;

	bool ret = true;

	suddenDeath = false;
	initialTime = 0;
	startTheGame = false;

	//FX de rounds
	round1FX = App->audio->LoadFx("Assets/Fx/round1.wav");


	//beachTexture = App->textures->Load("Assets/Sprites/Levels/PH_Beach.png");
	bgBeachTexture = App->textures->Load("Assets/Sprites/Levels/bgBeachSpriteSheet.png");
	currentBeachAnim = &bgBeachAnim;

	App->audio->PlayMusic("Assets/Music/03_Flying Power Disc (Beach Court).ogg", 1.0f);
	
	// Load timer texture
	timerTexture = App->textures->Load("Assets/Sprites/UI/Fonts/timerSpriteSheet.png");
	currentTimerAnim = &timerAnim;

	// Load UI Texture
	uiSpriteTexture = App->textures->Load("Assets/Sprites/UI/UISpriteSheet_Upgrade.png");
	Winn = App->textures->Load("Assets/Sprites/UI/charactersPresent2.png"); 

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	App->player->Enable();
	App->player2->Enable();
	App->frisbee->Enable();
	App->collisions->Enable();

	debugwinP1 = false;
	debugwinP2 = false;
	winState = 0;
	godMode = false;
	a = 0;
	estadoS = INICIO; 
	arbitroFinalRonda = 0;


	return ret;
}

Update_Status SceneBeachStage::Update()
{
	initialTimeS = SDL_GetTicks();


	switch (estadoS) {
	case (INICIO):

		//bullshit animaciones texturas etc - inicio partida, primer momento, solo ocurre una vez en cada partida

    	SceneBeachStage::Arbitro(1);
		estadoS = INICIORONDA;
		break;

	case (INICIORONDA):
		//Animacion Ronda 1.
		
		estadoTS = estadoTimerS::INICIOT;
		timerAnim.Reset();
		App->player->score = 0;
		App->player2->score = 0;
		initialTimeS = SDL_GetTicks();
		timeLimitS = 5 * 1000;
		Arbitro(arbitroFinalRonda);
		estadoS = RONDA;
		break;

	case (RONDA):
		TimerS();
		Round();

		//Tendremos que poner una condicion para cuando se marquen puntosq ue aqui se ejecuten unas texuras/animaciones - MARCARPUNTO

		break;

	case (FINALRONDA):
		//Animaciones caundo se gana/pierde una ronda
		Win();
		estadoS = INICIORONDA;
		break;

	case (FINAL):

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
	//}

	//currentBeachAnim->Update();

	//// DEBUG INSTANT WIN
	//if (App->input->keys[SDL_SCANCODE_F3] == Key_State::KEY_DOWN)
	//{
	//	App->audio->PlayMusic("Assets/Music/06_Set Clear.ogg", 0.0f);
	//	
	//	App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");
	//	//App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);
	//	debugwinP1 = true;
	//	Win();
	//}
	//// DEBUG INSTANT LOSE
	//if (App->input->keys[SDL_SCANCODE_F4] == Key_State::KEY_DOWN)
	//{
	//	App->audio->PlayMusic("Assets/Music/09_Lost Set.ogg", 0.0f);
	//	
	//	App->audio->PlayMusic("Assets/Music/silenceAudio.ogg");
	//	//App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);
	//	debugwinP2 = true;
	//	Win();
	//}
	//if (App->input->keys[SDL_SCANCODE_F1] == Key_State::KEY_DOWN)
	//{

	//	debugwinP2 = true;
	//	Win();
	//}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneBeachStage::PostUpdate()
{

	// Draw everything --------------------------------------
		//App->render->Blit(beachTexture, 0, 0, NULL);
		//Beach background
	SDL_Rect rectBeach = currentBeachAnim->GetCurrentFrame();
	App->render->Blit(bgBeachTexture, 0, 0, &rectBeach);

	//P1 UI
	SDL_Rect p1Rect = { 359, 0, 15, 8 };
	App->render->Blit(uiSpriteTexture, 10, 10, &p1Rect);

	//P2 UI
	SDL_Rect p2Rect = { 374, 0, 16, 8 };
	App->render->Blit(uiSpriteTexture, 200, 10, &p2Rect);

	//Japan Flag x2
	SDL_Rect japanFlagRect = { 460, 0, 15, 9 };
	App->render->Blit(uiSpriteTexture, 40, 10, &japanFlagRect);
	App->render->Blit(uiSpriteTexture, 230, 10, &japanFlagRect);

	SDL_Rect winUIRight = { 224, 86, 100, 32 };
	SDL_Rect LoseUIRight = { 393, 27, 99, 27 };

	SDL_Rect winUILeft = { 324, 86, 112, 32 };
	SDL_Rect LoseUILeft = { 0, 54, 100, 27 };
	

	if (winState == 4) {
		App->fade->FadeToBlack(this, (Module*)App->sceneTitle, 15);
	}


	if (winState == 1) {

		App->render->Blit(Winn, 0, 0, NULL);
		App->render->Blit(uiSpriteTexture, 18, 48, &winUILeft);
		App->render->Blit(uiSpriteTexture, 175,54, &LoseUIRight);
		winState = 4;

	}
	else if (winState == 2) {

		App->render->Blit(Winn, 0, 0, NULL);
		App->render->Blit(uiSpriteTexture, 176, 48, &winUIRight);
		App->render->Blit(uiSpriteTexture, 30, 54, &LoseUILeft);
		winState = 4;
		
	}
	else if (winState == 3) {

		App->render->Blit(Winn, 0, 0, NULL);
		App->render->Blit(uiSpriteTexture, 18, 48, &LoseUIRight);
		App->render->Blit(uiSpriteTexture, 30, 54, &LoseUILeft);
		winState = 4;

	}

	if (a == 1) {
		App->render->Blit(uiSpriteTexture, 113, 12, &rectanguletL);
	}
	else if (a == 2) {
		App->render->Blit(uiSpriteTexture, 161, 12, &rectanguletR);
	}
	else if (a == 3) {
		App->render->Blit(uiSpriteTexture, 113, 12, &rectanguletL);
		App->render->Blit(uiSpriteTexture, 161, 12, &rectanguletR);
	}
	

	SDL_Rect rounds = { 0,0,0,0 };
	App->render->Blit(uiSpriteTexture, 150, 150, &rounds);

	/*WIIIIIIIIIIIIIIIIIIIIIIN Y LOSEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE*//*WIIIIIIIIIIIIIIIIIIIIIIN Y LOSEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE*//*WIIIIIIIIIIIIIIIIIIIIIIN Y LOSEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE*/

	if (startTheGame)
	{

		//Timer
		SDL_Rect rectTimer = currentTimerAnim->GetCurrentFrame();
		App->render->Blit(timerTexture, 144, 13, &rectTimer);

		//12 points / 30 sec
		rectNormes = { 207, 11, 151, 15 };
		App->render->Blit(uiSpriteTexture, 77, 168, &rectNormes);

		//puntuació tots gols L
		tresPuntsL = { 0, 188, 63, 34 };
		cincPuntsL = { 76, 258, 63, 35 };
		App->render->Blit(uiSpriteTexture, 7, 30, &tresPuntsL);
		//App->render->Blit(uiSpriteTexture, 7, 92, &cincPuntsL);
		App->render->Blit(uiSpriteTexture, 7, 158, &tresPuntsL);

		//puntuació tots gols R
		tresPuntsR = { 112, 120, 63, 34 };
		cincPuntsR = { 74, 224, 63, 35 };
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
	else
	{
		//12 points / 30 sec
		SDL_Rect rectNormes = { 207, 11, 151, 15 };
		App->render->Blit(uiSpriteTexture, 77, 168, &rectNormes);

		//puntuació tots gols L
		SDL_Rect tresPuntsL = { 0, 188, 63, 34 };
		SDL_Rect cincPuntsL = { 76, 258, 63, 35 };
		App->render->Blit(uiSpriteTexture, 7, 30, &tresPuntsL);
		App->render->Blit(uiSpriteTexture, 7, 92, &cincPuntsL);
		App->render->Blit(uiSpriteTexture, 7, 158, &tresPuntsL);

		//puntuació tots gols R
		SDL_Rect tresPuntsR = { 112, 120, 63, 34 };
		SDL_Rect cincPuntsR = { 74,	224, 63, 35 };
		App->render->Blit(uiSpriteTexture, 236, 30, &tresPuntsR);
		App->render->Blit(uiSpriteTexture, 236, 92, &cincPuntsR);
		App->render->Blit(uiSpriteTexture, 236, 158, &tresPuntsR);

		//Set1
		SDL_Rect set1Rect = { 160, 300, 160, 56 };
		App->render->Blit(uiSpriteTexture, 72, 80, &set1Rect);

		//Time88
		SDL_Rect rectTimer88 = { 0, 0, 15, 15 };
		App->render->Blit(timerTexture, 144, 13, &rectTimer88);

		//Score 00-00
		App->fonts->BlitText(155, 16, App->player->scoreFont, App->player->scoreText);

		//App->fonts->BlitText(115, 16, scoreFont, scoreText);

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

		sprintf_s(debugText, 10, "%2d", initialTimeS);

		// A "TEST TEXT", escriure el que es vulgui: una string (igual que l'exempel) o la variable debugText,
		// que correspon a la variable que s'hagi posat al quart parametre de sprintf_s, "".

		App->fonts->BlitText(72, 190, debugFont, debugText);
	}

	return Update_Status::UPDATE_CONTINUE;
}

bool SceneBeachStage::CleanUp()
{
	App->player->Disable();
	App->player2->Disable();
	App->frisbee->Disable();
	App->collisions->Disable();

	return true;
}

//En cuanto mete un jugador un gol, se llama a esto y se determina el valor de arbitro. Lo ponemos aqui como funcion externa en vez de dentro del update de frisbee
//Ya que en este .cpp también llamaremos a esta función en función de las rondas/sets ganados~

void SceneBeachStage::Arbitro(int arbitro) {  //cambiar esta funcion a arbitro
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
		

	} else if (arbitro == 2) {

		App->frisbee->xspeed = 3;
		App->frisbee->yspeed = -2;
		App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO; 
		App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::ARBITRO;
	}
}

void SceneBeachStage::Round() {

	if (App->player->score >= 12 || App->player2->score >= 12) {

		if (App->player->score > App->player2->score + 2) {
			App->player->round += 1;
			
			//Llamar animación de jugador ganador 1 y las texturas
			App->player->score = 0;
			App->player2->score = 0;
			estadoS = FINALRONDA;
		}

		if (App->player2->score > App->player->score + 2) {
			App->player2->round += 1;
			
			//Llamar animación de jugador ganador 2 y las texturas
			App->player->score = 0;
			App->player2->score = 0;
			estadoS = FINALRONDA;
		}

	}
	else if (estadoTS == FIN && (App->frisbee->estadoF == ModuleFrisbee::estadoFrisbee::STOP)) { //FALTA TIMER


		if (App->player->score > App->player2->score) {
			App->player->round += 1;
			
			//Llamar animación de jugador ganador 1 y las texturas
			arbitroFinalRonda = 2;
			estadoS = FINALRONDA;

		}
		else if (App->player2->score > App->player->score) {
			App->player2->round += 1;
			
			//Llamar animación de jugador ganador 2 y las texturas
			arbitroFinalRonda = 1;
			estadoS = FINALRONDA;

		}
		else if (App->player->score == App->player2->score) {
			App->player->round += 1;
			App->player2->round += 1;

			arbitroFinalRonda = 1;
			estadoS = FINALRONDA;
			
			//Animación de cuando los dos acaban una ronda en puntuacion empate

		}

	}



}

void SceneBeachStage::Win() { //AQUI SE TENDRÍA QUE CAMBIAR EL ESTADO EN SWITCH FINAL PARA QUE SE EJECUTEN LAS ANIMACIONES/TEXTURAS CONCRETAS ~ ~~ o ponerlas aquid riectamente las anim/text

	if (App->player->round == App->player2->round && App->player->round == 2 && App->player2->round ==2 && !suddenDeath) {
		suddenDeath = true;
		Arbitro(1);

	} 
	else if (App->player->score != 0 && suddenDeath) {
		//llamar animación y texturas de que ha ganado el primer jugador la partida
		//SDL Delay
		winState = 1;
		estadoS = FINAL;

	}
	else if (App->player2->score != 0 && suddenDeath) {
		//llamar animación y texturas de que ha ganado el segundo jugador la partida
		//SDL Delay
		winState = 2;
		estadoS = FINAL;
		
	}
	else if ((App->player->round == 2 && !suddenDeath) || debugwinP1) {
		//llamar animación y texturas de que ha ganado el primer jugador la partida
		//SDL Delay
		winState = 1;
		estadoS = FINAL;
		}
	else if ((App->player2->round == 2&&!suddenDeath) || debugwinP2) {
		//llamar animación y texturas de que ha ganado el segundo jugador la partida
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

}

void SceneBeachStage::Score(){ //Tendremos que cambiar estado en el switch - MARCARPUNTO, en cada momento en que se meten puntos para que se realicen las animaciones 

	if (App->frisbee->position.x <= 19) {
		if (App->frisbee->position.y >= 94 && App->frisbee->position.y <= 144) {
			App->player2->score += 5;
			if (suddenDeath) {
				Win();
			}
			Round();
			Arbitro(1);

		}
		else {
			App->player2->score += 3;
			if (suddenDeath) {
				Win();
			}
			Round();
			Arbitro(1);
			
		}
	}
	if (App->frisbee->position.x >= 276) {
		if (App->frisbee->position.y >= 94 && App->frisbee->position.y <= 144) {
			App->player->score += 5;
			if (suddenDeath) {
				Win();
			}

			Round();
			Arbitro(2);
		}
		else {
			App->player->score += 3;
			if (suddenDeath) {
				Win();
			}
			Round();
			Arbitro(2);
		}
	}

	App->frisbee->position.x = 150;
	App->frisbee->position.y = 200;
}

void SceneBeachStage::TimerS() {
	currentTimeS = SDL_GetTicks();

	if (currentTimeS - initialTimeS >= timeLimitS) {
		estadoTS = estadoTimerS::FIN;
	}
}