#include "ModulePlayer.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include "ModuleFrisbee.h"
#include "SceneBeachStage.h"
#include "ModulePlayer2.h"
#include "ModuleFrisbee.h"

#include <stdio.h>

#include "SDL/include/SDL.h"

ModulePlayer::ModulePlayer(bool startEnabled) : Module(startEnabled)
{
	//idleLAnim
	for (int i = 0; i < 8; i++) {
		idleLAnim.PushBack({ 211+(i*53), 338, 53, 57 });
	} 
	idleLAnim.loop = true;
	idleLAnim.speed = 0.075f;

	//idleRAnim
	for (int i = 0; i < 8; i++) {
		idleRAnim.PushBack({ 107 + (i * 53), 0, 53, 57 });
	}
	idleRAnim.loop = true;
	idleRAnim.speed = 0.075f;

	// Move Right
	for (int i = 0; i < 5; i++) {
		rightAnim.PushBack({ 319+(i*53), 399, 53, 57 });
	}
	rightAnim.loop = true;
	rightAnim.speed = 0.075f;

	//Move Left
	for (int i = 0; i < 6; i++) {
		leftAnim.PushBack({ 105 + (i * 53), 58, 53, 57 });
	}
	leftAnim.loop = true;
	leftAnim.speed = 0.075f;
	
	// Move Down Head Right
	for (int i = 0; i < 6; i++) {
		downRAnim.PushBack({ 0 + (i * 53), 114, 53, 57 });
	}
	downRAnim.loop = true;
	downRAnim.speed = 0.075f;

	// Move Down Head Left
	for (int i = 0; i < 6; i++) {
		downLAnim.PushBack({ 425 + (i * 53), 451, 53, 57 });
	} 
	downLAnim.loop = true;
	downLAnim.speed = 0.075f;

	//Move Up Head Left
	for (int i = 0; i < 6; i++) {
		upLAnim.PushBack({ 0 + (i * 53), 395, 53, 57 });
	}
	upLAnim.loop = true;
	upLAnim.speed = 0.075f;
	
	//Move Up Head Right
	for (int i = 0; i < 6; i++) {
		upRAnim.PushBack({ 423 + (i * 53), 57, 53, 57 });
	}
	upRAnim.loop = true;
	upRAnim.speed = 0.075f;
	
	//Idle Disk
	for (int i = 0; i < 3; i++) {
		idleDisk.PushBack({ 158 + (i * 53), 283, 53, 57 });
	}
	idleDisk.loop = true;
	idleDisk.speed = 0.075f;

	//Lanzamiento Disco
	for (int i = 0; i < 5; i++) {
		lanzamiento.PushBack({ 322 + (i * 53), 284, 53, 57 });
	}
	lanzamiento.loop = false;
	lanzamiento.speed = 0.75f;

	//Dust particles
	polvo.PushBack({ 5, 108, 14, 14 });
	polvo.PushBack({ 26, 108, 14, 14 });
	polvo.PushBack({ 47, 108, 15, 14 });
	polvo.PushBack({ 69, 108, 15, 14 });
	polvo.PushBack({ 94, 108, 12, 13 });
	polvo.PushBack({ 113, 108, 12, 13 });
	polvo.PushBack({ 130, 108, 12, 13 });
	polvo.PushBack({ 143, 108, 12, 13 });
	polvo.loop = false;
	polvo.speed = 0.3f;

	pols = false;
	

	//TO DO SWITCH PARA ELEGIR CHARACTERS + ESCENARIOS
	destroyed = false;

}

ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	speed = 2;
	last1 = 1;
	score = 000;
	scoreFont = -1;
	destroyed = false;
	round = 0;

	LOG("Loading player textures");

	bool ret = true;

	texture = App->textures->Load("Assets/Sprites/Characters/Jap.png");
	currentAnimation = &idleRAnim;

	dust_texture = App->textures->Load("Assets/Sprites/particlesAndEffects.png");
	dustAnimation = &polvo;

	position.x = 20;
	position.y = 100;

	collider = App->collisions->AddCollider({ (int)position.x, (int)position.y, 27, 31 }, Collider::Type::PLAYER, this);

	char lookupTable[] = { "0123456789G " };
	scoreFont = App->fonts->Load("Assets/Sprites/UI/Fonts/scoreFont.png", lookupTable, 1);

	char lookupTableDebug[] = { "! ?,_./0123456789?;<??ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
	debugFont = App->fonts->Load("Assets/Sprites/UI/Fonts/debugFont.png", lookupTableDebug, 2);
	isDebugAppear = false;

	FrisbeeTime = 0;

	estadoP1 = STOP;

	return ret;
}

Update_Status ModulePlayer::Update()
{

	switch (estadoP1) {
	case (STOP):
		break;
	case (MOVIMIENTO):
		movimientoPlayer();
		break;

	case (WITHFRISBEE):
		timerP();
		bea = (currentTimeP - initialTimeP) / 1000;
		lanzamientoPlayer();
		break;


	case (LANZAMIENTO):
		if (estadoTP == INICIO)
		{
			initialTimeP = SDL_GetTicks();
			timeLimitP = 0.08 * 1000;
			currentAnimation = &lanzamiento;
			estadoTP = EJECUTANDO;
		}
		else if (estadoTP == EJECUTANDO) {
			/*setsSpriteAppear = true;*/
			timerP();
		}
		else if (estadoTP == FIN)
		{
			//setsSpriteAppear = false;
			//SceneBeachStage::Arbitro(1);
			estadoTP = INICIO;
			estadoP1 = MOVIMIENTO;
			App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
			lanzamiento.Reset();
			currentAnimation = &idleRAnim;
		}
		break;
	}


	collider->SetPos(position.x, position.y);
	currentAnimation->Update();
	dustAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePlayer::PostUpdate()
{
	if (!destroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		App->render->Blit(texture, position.x, position.y, &rect);

		if (pols) {
			SDL_Rect rect2 = dustAnimation->GetCurrentFrame();
			App->render->Blit(dust_texture, position.x-5, position.y+27, &rect2); //ARREGLAR LA POSICION
		}
		pols = false;
	}

	// Draw UI (score) --------------------------------------
	if (App->sceneBeachStage->estadoS != App->sceneBeachStage->INICIO)
	{
		sprintf_s(scoreText, 10, "%2d", score);

		App->fonts->BlitText(117, 17, scoreFont, scoreText);

		//App->fonts->BlitText(20, 150, scoreFont, "0 1 2 3 4 5 6 7 8 9 G");
	}

	//Debug Text rounds P1
	if (App->input->keys[SDL_SCANCODE_F5] == Key_State::KEY_DOWN)
	{
		if (!isDebugAppear)
			isDebugAppear = true;
		else isDebugAppear = false;
	}

	if (isDebugAppear)
	{
		sprintf_s(debugText, 10, "%2d", round);
		App->fonts->BlitText(80, 20, debugFont, debugText);
	}
	

	return Update_Status::UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider)
	{
		//creo q si ya lo tenemos puesto en el disco que si choca no haga nada, no deberia hacer falta ponerlo aqui tmb
		estadoP1 = estadoPlayer::WITHFRISBEE;
		App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::WITHPLAYER;

		//Al recibir disco hace idle con disco en la mano
		currentAnimation = &idleDisk;

		initialTimeP = SDL_GetTicks();
		timeLimitP = 2 * 1000;
		estadoTP = estadoTimerP::EJECUTANDO;

		App->player2->estadoP2 = ModulePlayer2::estadoPlayer2::MOVIMIENTO;
		App->frisbee->limiteConcrete = true;
	}
}

void ModulePlayer::movimientoPlayer(){
		
		//MOVIMIENTO - DASH 
		if ((App->input->keys[SDL_SCANCODE_D] == Key_State::KEY_REPEAT) && position.x < 110)
		{
			position.x += speed;

			if (currentAnimation != &rightAnim && App->input->keys[SDL_SCANCODE_W] != Key_State::KEY_REPEAT && App->input->keys[SDL_SCANCODE_S] != Key_State::KEY_REPEAT)
			{
				rightAnim.Reset();
				currentAnimation = &rightAnim;
			}
			last1 = 1;

			if (App->input->keys[SDL_SCANCODE_V] == Key_State::KEY_DOWN && estadoTP == INICIO) {

				initialTimeP = SDL_GetTicks();
				timeLimitP = 2 * 1000;
				estadoTP = EJECUTANDO;

			}
			else if (App->input->keys[SDL_SCANCODE_V] == Key_State::KEY_REPEAT && estadoTP == EJECUTANDO)
			{
				timerP();
				position.x += 1, 5 * speed;
				currentAnimation = &rightAnim;
				pols = true;
			}
			else if (estadoTP == FIN) {
				estadoTP = INICIO;
			}

			if (currentAnimation != &rightAnim) {
				rightAnim.Reset();
				currentAnimation = &rightAnim;
			}

		}
		if (App->input->keys[SDL_SCANCODE_A] == Key_State::KEY_REPEAT && position.x > 20)
		{
			position.x -= speed;

			if (currentAnimation != &leftAnim && App->input->keys[SDL_SCANCODE_W] != Key_State::KEY_REPEAT && App->input->keys[SDL_SCANCODE_S] != Key_State::KEY_REPEAT)
			{ 
				leftAnim.Reset();
				currentAnimation = &leftAnim;

			}
			last1 = 0; //TO DO: REVISAR NOMBRE Y SU FUNCIÓN DE TODOS LOS LAST1


			if (App->input->keys[SDL_SCANCODE_V] == Key_State::KEY_DOWN && estadoTP == INICIO) {

				initialTimeP = SDL_GetTicks();
				timeLimitP = 2 * 1000;
				estadoTP = EJECUTANDO;

			}
			else if (App->input->keys[SDL_SCANCODE_V] == Key_State::KEY_REPEAT && estadoTP == EJECUTANDO)
			{
				timerP();
				position.x -= 1, 5 * speed;
				currentAnimation = &leftAnim;
				pols = true;
			}
			else if (estadoTP == FIN) {
				estadoTP = INICIO;
			}

			if (currentAnimation != &leftAnim) {
				leftAnim.Reset();
				currentAnimation = &leftAnim;
			}

		}
		if (App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT && position.y > 50)
		{
			position.y -= speed;
			

			if (currentAnimation != &upLAnim && last1 == 0)
			{
				upLAnim.Reset();
				currentAnimation = &upLAnim;
			}
			if (currentAnimation != &upRAnim && last1 == 1)
			{
				upRAnim.Reset();
				currentAnimation = &upRAnim;
			}


			if (App->input->keys[SDL_SCANCODE_V] == Key_State::KEY_DOWN && estadoTP == INICIO) {

				initialTimeP = SDL_GetTicks();
				timeLimitP = 2 * 1000;
				estadoTP = EJECUTANDO;

			}
			else if (App->input->keys[SDL_SCANCODE_V] == Key_State::KEY_REPEAT && estadoTP == EJECUTANDO)
			{
				timerP();
				position.y -= 1, 5 * speed;
				pols = true;
				currentAnimation = &rightAnim;
			}
			else if (estadoTP == FIN) {
				estadoTP = INICIO;
			}

			if (currentAnimation != &rightAnim) {
				rightAnim.Reset();
				currentAnimation = &rightAnim;
			}

		}
		if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT && position.y < 150)
		{
			position.y += speed;

			if (currentAnimation != &downLAnim && last1 == 0)
			{
				downLAnim.Reset();
				currentAnimation = &downLAnim;
			}
			if (currentAnimation != &downRAnim && last1 == 1)
			{
				downLAnim.Reset();
				currentAnimation = &downRAnim;
			}


			if (App->input->keys[SDL_SCANCODE_V] == Key_State::KEY_DOWN && estadoTP == INICIO) {
				pols = true;
				initialTimeP = SDL_GetTicks();
				timeLimitP = 2 * 1000;
				estadoTP = EJECUTANDO;

			}
			else if (App->input->keys[SDL_SCANCODE_V] == Key_State::KEY_REPEAT && estadoTP == EJECUTANDO)
			{
				timerP();
				position.y += 1, 5 * speed;
				pols = true;
				currentAnimation = &rightAnim;
			}
			else if (estadoTP == FIN) {
				estadoTP = INICIO;
			}

			if (currentAnimation != &rightAnim) {
				rightAnim.Reset();
				currentAnimation = &rightAnim;
			}

		}


		if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_IDLE
			&& App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_IDLE
			&& App->input->keys[SDL_SCANCODE_A] == Key_State::KEY_IDLE
			&& App->input->keys[SDL_SCANCODE_D] == Key_State::KEY_IDLE && last1 == 0)
			currentAnimation = &idleLAnim;

		if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_IDLE
			&& App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_IDLE
			&& App->input->keys[SDL_SCANCODE_A] == Key_State::KEY_IDLE
			&& App->input->keys[SDL_SCANCODE_D] == Key_State::KEY_IDLE && last1 == 1)
			currentAnimation = &idleRAnim;

		if (App->input->keys[SDL_SCANCODE_N] == Key_State::KEY_DOWN &&  (App->frisbee->position.x - (position.x +20)) > 1  && (App->frisbee->position.x - (position.x + 20)) < 40 && App->frisbee->lanzamientoF == ModuleFrisbee::tipoLanzamiento::NORMAL) {
		
			if (App->frisbee->position.y >= position.y && App->frisbee->position.y <= (position.y +31)) {
				App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::BLOCK;

				App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::BLOCKPLAYER1;
			}
				
			
		}
	 

}

void ModulePlayer::lanzamientoPlayer() {
	
	pepe = (float)bea + 1;
	if (pepe > 1.5) {
		pepe = 1.5;
	}

	for (int i = 0; i < 1; i++) {
		if (App->input->keys[SDL_SCANCODE_V] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT)
		{
			App->frisbee->xspeed = 4/pepe; 
			App->frisbee->yspeed = -4/pepe;
			App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO; 
			App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::NORMAL;
			App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::DARRIBA;			
			estadoP1 = estadoPlayer::MOVIMIENTO;
			

			break;
		}


		if (App->input->keys[SDL_SCANCODE_V] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT)
		{
			

			App->frisbee->xspeed = 4/pepe;
			App->frisbee->yspeed = 4 / pepe;
			App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
			App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::NORMAL;
			App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::DABAJO;
			estadoP1 = estadoPlayer::MOVIMIENTO;

			break;

		}


		if (App->input->keys[SDL_SCANCODE_V] == Key_State::KEY_DOWN || estadoTP == FIN)
		{

			//Pepe y bea sirven para modificar la velocidad del disco en funcion del tiempo que la haya tenido el player1.

			currentAnimation = &lanzamiento;
			App->frisbee->xspeed = 4 / pepe;
			App->frisbee->yspeed = 0;
			/*App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;*/
			App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::NORMAL;
			App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::HORIZONTAL;
			/*estadoP1 = estadoPlayer::MOVIMIENTO;*/
			estadoP1 = estadoPlayer::LANZAMIENTO;
			estadoTP = estadoTimerP::INICIO;

			break;

		}


		if (App->input->keys[SDL_SCANCODE_B] == Key_State::KEY_DOWN)
		{
			
			App->collisions->RemoveCollider(App->frisbee->collider);

			App->frisbee->xspeed = 3;
			App->frisbee->yspeed = 0;
			App->frisbee->vel_parabola(position.x, 260);
			App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
			App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::PARABOLA;
			App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::HORIZONTAL;
			estadoP1 = estadoPlayer::MOVIMIENTO;

			break;
		}

		//LANZAMIENTO SUPERSHOT
		p1Char = 0;
		if (p1Char == 0) { //japo

			if (App->input->keys[SDL_SCANCODE_N] == Key_State::KEY_DOWN /*&& (App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT || App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT)*/ && App->frisbee->lanzamientoF == ModuleFrisbee::BLOCKPLAYER1)
			{
				App->frisbee->angulo = 0;
				App->frisbee->xspeed = 5;
				App->frisbee->yspeed = -5;
				App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::SUPERSHOT;
				App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
				App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::MAX;
				App->frisbee->tipoSupershot = ModuleFrisbee::tipoSupershot::MITA_SUPERSHOT;
				estadoP1 = estadoPlayer::MOVIMIENTO;
				break;

			}

		}
		else if (p1Char == 1) { //coreano

			if (App->input->keys[SDL_SCANCODE_N] == Key_State::KEY_DOWN /*&& (App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT || App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT)*/ && App->frisbee->lanzamientoF == ModuleFrisbee::BLOCKPLAYER1)
			{
				App->frisbee->xspeed = 5;
				App->frisbee->yspeed = -5;
				App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::SUPERSHOT;
				App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
				App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::MAX;
				App->frisbee->tipoSupershot = ModuleFrisbee::tipoSupershot::YOO_SUPERSHOT;
				estadoP1 = estadoPlayer::MOVIMIENTO;
				break;

			}

		}
		else if (p1Char == 2) { //aleman

			if (App->input->keys[SDL_SCANCODE_N] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT && App->frisbee->lanzamientoF == ModuleFrisbee::BLOCKPLAYER1)
			{
				App->frisbee->limiteWesselSupershot = 120;
				App->frisbee->xspeed = 5;
				App->frisbee->yspeed = -5;
				App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::SUPERSHOT;
				App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
				App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::MAX;
				App->frisbee->tipoSupershot = ModuleFrisbee::tipoSupershot::WESSEL_SUPERSHOT;
				estadoP1 = estadoPlayer::MOVIMIENTO;
				break;

			}

			if (App->input->keys[SDL_SCANCODE_N] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT && App->frisbee->lanzamientoF == ModuleFrisbee::BLOCKPLAYER1)
			{
				App->frisbee->limiteWesselSupershot = 120;
				App->frisbee->xspeed = 5;
				App->frisbee->yspeed = 5;
				App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::SUPERSHOT;
				App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
				App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::MAX;
				App->frisbee->tipoSupershot = ModuleFrisbee::tipoSupershot::WESSEL_SUPERSHOT;
				estadoP1 = estadoPlayer::MOVIMIENTO;
				break;

			}

		}
	
	}
}

void ModulePlayer::timerP() {
	currentTimeP = SDL_GetTicks();

	if (currentTimeP - initialTimeP >= timeLimitP) {
		estadoTP = estadoTimerP::FIN;
	}
}
