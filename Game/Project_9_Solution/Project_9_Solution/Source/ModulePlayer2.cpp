#include "ModulePlayer2.h"

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
#include "ModulePlayer.h"
#include "SDL/include/SDL.h"

//Venga developer;)


#include <stdio.h>

ModulePlayer2::ModulePlayer2(bool startEnabled) : Module(startEnabled)
{


	// Idle Left Animation
	idleLAnim.PushBack({ 211, 341, 53, 57 });
	idleLAnim.PushBack({ 264, 341, 53, 57 });
	idleLAnim.PushBack({ 317, 341, 53, 57 });
	idleLAnim.PushBack({ 370, 341, 53, 57 });
	idleLAnim.PushBack({ 423, 341, 53, 57 });
	idleLAnim.PushBack({ 476, 341, 53, 57 });
	idleLAnim.PushBack({ 529, 341, 53, 57 });
	idleLAnim.PushBack({ 582, 341, 53, 57 });
	idleLAnim.loop = true;
	idleLAnim.speed = 0.1f;

	// Idle Right Animation
	idleRAnim.PushBack({ 105, 0, 53, 57 });
	idleRAnim.PushBack({ 158, 0, 53, 57 });
	idleRAnim.PushBack({ 211, 0, 53, 57 });
	idleRAnim.PushBack({ 264, 0, 53, 57 });
	idleRAnim.PushBack({ 317, 0, 53, 57 });
	idleRAnim.PushBack({ 370, 0, 53, 57 });
	idleRAnim.PushBack({ 423, 0, 53, 57 });
	idleRAnim.PushBack({ 476, 0, 53, 57 });
	idleRAnim.loop = true;
	idleRAnim.speed = 0.1f;

	// Move Right
	rightAnim.PushBack({ 454, 148, 45, 32 });
	rightAnim.PushBack({ 426, 148, 28, 33 });
	rightAnim.PushBack({ 385, 148, 41, 36 });
	rightAnim.PushBack({ 339, 148, 46,	29 });
	rightAnim.PushBack({ 310, 148, 29, 36 });
	rightAnim.PushBack({ 269, 148, 41, 35 });
	rightAnim.loop = true;
	rightAnim.speed = 0.1f;

	// Move Down Head Left
	downLAnim.PushBack({ 240, 404, 29, 43 });
	downLAnim.PushBack({ 208, 404, 32, 38 });
	downLAnim.PushBack({ 179, 404, 29, 35 });
	downLAnim.PushBack({ 155, 404, 24, 41 });
	downLAnim.PushBack({ 125, 404, 30, 38 });
	downLAnim.PushBack({ 94, 404, 31, 34 });
	downLAnim.loop = true;
	downLAnim.speed = 0.1f;

	// Move Down Head Right
	downRAnim.PushBack({ 240, 148, 29, 43 });
	downRAnim.PushBack({ 208, 148, 32, 38 });
	downRAnim.PushBack({ 179, 148, 29, 35 });
	downRAnim.PushBack({ 155, 148, 24, 41 });
	downRAnim.PushBack({ 125, 148, 30, 38 });
	downRAnim.PushBack({ 94, 148, 31, 34 });
	downRAnim.loop = true;
	downRAnim.speed = 0.1f;

	//Move Up Head Left
	upLAnim.PushBack({ 150, 447, 30, 40 });
	upLAnim.PushBack({ 120, 447, 30, 51 });
	upLAnim.PushBack({ 90, 447, 30, 50 });
	upLAnim.PushBack({ 60, 447, 30, 40 });
	upLAnim.PushBack({ 30, 447, 30, 53 });
	upLAnim.PushBack({ 0, 447, 30, 50 });
	upLAnim.loop = true;
	upLAnim.speed = 0.1f;

	//Move Up Head Right
	upRAnim.PushBack({ 150, 191, 30, 40 });
	upRAnim.PushBack({ 120, 191, 30, 51 });
	upRAnim.PushBack({ 90, 191, 30, 50 });
	upRAnim.PushBack({ 60, 191, 30, 40 });
	upRAnim.PushBack({ 30, 191, 30, 53 });
	upRAnim.PushBack({ 0, 191, 30, 50 });
	upRAnim.loop = true;
	upRAnim.speed = 0.1f;

	//Move Left
	leftAnim.PushBack({ 455, 404, 45, 32 });
	leftAnim.PushBack({ 427, 404, 28, 33 });
	leftAnim.PushBack({ 386, 404, 40, 36 });
	leftAnim.PushBack({ 340, 404, 45, 29 });
	leftAnim.PushBack({ 311, 404, 28, 36 });
	leftAnim.PushBack({ 270, 404, 40, 35 });
	leftAnim.loop = true;
	leftAnim.speed = 0.1f;

	//Idle Disk
	idleDisk.PushBack({ 0, 512, 47, 48 });
	idleDisk.PushBack({ 48, 512, 47, 48 });
	idleDisk.PushBack({ 96, 512, 47, 48 });
	idleDisk.PushBack({ 144, 512, 47, 48 });
	idleDisk.PushBack({ 192, 512, 47, 48 });
	idleDisk.loop = true;
	idleDisk.speed = 0.1f;
}

ModulePlayer2::~ModulePlayer2()
{

}

bool ModulePlayer2::Start()
{

	speed = 2;
	score2 = 0;

	 last2 = 0;

	 explosionFx = 0;
	 tossFx = 0;
	 lobFx = 0;


	 score = 000;
	 scoreFont = -1;
	 round = 0;

	LOG("Loading player textures");

	bool ret = true;

	texture = App->textures->Load("Assets/Sprites/Characters/JapR.png");
	currentAnimation = &idleLAnim;

	//SFX
	tossFx = App->audio->LoadFx("Assets/Fx/Toss.wav");
	lobFx = App->audio->LoadFx("Assets/Fx/Lob.wav");

	position.x = 259;
	position.y = 100;

	destroyed = false;
	disco = false;

	collider = App->collisions->AddCollider({ (int)position.x, (int)position.y, 27, 31 }, Collider::Type::PLAYER, this);

	char lookupTable[] = { "0123456789G " };
	scoreFont = App->fonts->Load("Assets/Sprites/UI/Fonts/scoreFont.png", lookupTable, 1);

	char lookupTableDebug[] = { "! ?,_./0123456789?;<??ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
	debugFont = App->fonts->Load("Assets/Sprites/UI/Fonts/debugFont.png", lookupTableDebug, 2);
	isDebugAppear = false;

	FrisbeeTime = 0;

	estadoP2 = STOP;

	return ret;
}

Update_Status ModulePlayer2::Update()
{
	
	switch (estadoP2) {
	case (STOP):
		break;
	case (MOVIMIENTO):
		movimientoPlayer2();
		break;

	case (WITHFRISBEE):
		timerP2();
		lanzamientoPlayer2();
		break;

	}


	//if (App->sceneBeachStage->startTheGame) {

	//	if (FrisbeeTime < 120 && App->frisbee->posesion == 2)
	//	{
	//		FrisbeeTime++;
	//	}

		////LANZAMIENTO DE DISCO PARÁBOLA
		//for (int i = 0; i < 1; i++) {
		//	if (App->input->keys[SDL_SCANCODE_P] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT && disco)
		//	{

		//		
		//		break;
		//	}


		//	if (App->input->keys[SDL_SCANCODE_P] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT && disco)
		//	{
		//		
		//		break;

		//	}

		//	if (App->input->keys[SDL_SCANCODE_P] == Key_State::KEY_DOWN && disco && App->frisbee->posesion == 2)
		//	{
		//		App->frisbee->mov = 2;
		//		disco = false;
		//		App->frisbee->posesion = 0;
		//		App->frisbee->currentAnimation2 = &App->frisbee->projectile;
		//		App->audio->PlayFx(tossFx);
		//		FrisbeeTime = 0;
		//		App->frisbee->projectil = 2;
		//		App->frisbee->PosTemp = 30;
		//		App->collisions->RemoveCollider(App->frisbee->collider);

		//		break;
		//	}


	
	collider->SetPos(position.x, position.y);		
	currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePlayer2::PostUpdate()
{
	if (!destroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		App->render->Blit(texture, position.x, position.y, &rect);
	}

	// Draw UI (score) --------------------------------------
	if (App->sceneBeachStage->estadoS != App->sceneBeachStage->INICIO)
	{
		sprintf_s(scoreText, 10, "%2d", score);

		App->fonts->BlitText(165, 17, scoreFont, scoreText);
	}

	//App->fonts->BlitText(20, 150, scoreFont, "0 1 2 3 4 5 6 7 8 9 G");

	//Debug Text rounds P2
	if (App->input->keys[SDL_SCANCODE_F5] == Key_State::KEY_DOWN)
	{
		if (!isDebugAppear)
			isDebugAppear = true;
		else isDebugAppear = false;
	}

	if (isDebugAppear)
	{
		sprintf_s(debugText, 10, "%2d", round);
		App->fonts->BlitText(205, 20, debugFont, debugText);
	}

	return Update_Status::UPDATE_CONTINUE;
}

void ModulePlayer2::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && destroyed == false)
	{//le pongo 0,0 pq no se exactamente q es esto y como he cambiado la funcion como tal tengo q meterle estos parametros i o si
		
		App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::WITHPLAYER;
		currentAnimation = &idleDisk;
		estadoP2 = estadoPlayer2::WITHFRISBEE;

		initialTimeP2 = SDL_GetTicks();
		timeLimitP2 = 2 * 1000;
		estadoTP2 = estadoTimerP2::EJECUTANDO;

		App->player->estadoP1 = ModulePlayer::estadoPlayer::MOVIMIENTO;
	}

	
}

void ModulePlayer2::movimientoPlayer2() {
	//MOVIMIENTO
	if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_REPEAT && position.x > 159 && App->frisbee->posesion != 2)
	{
		position.x -= speed;

		if (currentAnimation != &leftAnim && App->input->keys[SDL_SCANCODE_UP] != Key_State::KEY_REPEAT && App->input->keys[SDL_SCANCODE_DOWN] != Key_State::KEY_REPEAT)
		{
			leftAnim.Reset();
			currentAnimation = &leftAnim;
		}
		last2 = 0;
	}

	if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_REPEAT && position.x < 258 && App->frisbee->posesion != 2)
	{
		position.x += speed;
		if (currentAnimation != &rightAnim && App->input->keys[SDL_SCANCODE_UP] != Key_State::KEY_REPEAT && App->input->keys[SDL_SCANCODE_DOWN] != Key_State::KEY_REPEAT)
		{
			rightAnim.Reset();
			currentAnimation = &rightAnim;
		}
		last2 = 1;
	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT && position.y < 150 && App->frisbee->posesion != 2)
	{
		position.y += speed;
		if (currentAnimation != &downLAnim && last2 == 0)
		{
			downLAnim.Reset();
			currentAnimation = &downLAnim;
		}
		if (currentAnimation != &downRAnim && last2 == 1)
		{
			downLAnim.Reset();
			currentAnimation = &downRAnim;
		}
	}

	if (App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT && position.y > 50 && App->frisbee->posesion != 2)
	{
		position.y -= speed;
		if (currentAnimation != &upLAnim && last2 == 0)
		{
			upLAnim.Reset();
			currentAnimation = &upLAnim;
		}
		if (currentAnimation != &upRAnim && last2 == 1)
		{
			upRAnim.Reset();
			currentAnimation = &upRAnim;
		}
	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_IDLE && last2 == 0 && App->frisbee->posesion != 2)
		currentAnimation = &idleLAnim;

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_IDLE && last2 == 1 && App->frisbee->posesion != 2)
		currentAnimation = &idleRAnim;



	if (App->input->keys[SDL_SCANCODE_I] == Key_State::KEY_DOWN && ((position.x) - App->frisbee->position.x ) > 1 && ((position.x) - App->frisbee->position.x) < 40 && App->frisbee->lanzamientoF == ModuleFrisbee::tipoLanzamiento::NORMAL) {

		if (App->frisbee->position.y >= position.y && App->frisbee->position.y <= (position.y + 31)) {
	
			App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::BLOCK;
			App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::BLOCKPLAYER2;

		}



	}


}

void ModulePlayer2::lanzamientoPlayer2() {
	for (int i = 0; i < 1; i++) {
		if (App->input->keys[SDL_SCANCODE_O] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT)
		{

			App->frisbee->xspeed = -4;
			App->frisbee->yspeed = -4;
			App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
			App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::NORMAL;
			App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::DARRIBA;
			estadoP2 = estadoPlayer2::MOVIMIENTO;
			break;
		}


		if (App->input->keys[SDL_SCANCODE_O] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT)
		{
			App->frisbee->xspeed = -4;
			App->frisbee->yspeed = 4;
			App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
			App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::NORMAL;
			App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::DABAJO;
			estadoP2 = estadoPlayer2::MOVIMIENTO;
			break;

		}

		if (App->input->keys[SDL_SCANCODE_O] == Key_State::KEY_DOWN || estadoTP2 == FIN)
		{
			App->frisbee->xspeed = -4;
			App->frisbee->yspeed = 0;
			App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
			App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::NORMAL;
			App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::HORIZONTAL;
			estadoP2 = estadoPlayer2::MOVIMIENTO;
			estadoTP2 = estadoTimerP2::INICIO;

			break;
		}
		//}

		//LANZAMIENTO DE DISCO PARÁBOLA

		if (App->input->keys[SDL_SCANCODE_P] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT)
		{
			App->frisbee->xspeed = -4;
			App->frisbee->yspeed = 4;
			App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
			App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::PARABOLA;
			App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::DARRIBA;
			estadoP2 = estadoPlayer2::MOVIMIENTO;
			break;

		}


		if (App->input->keys[SDL_SCANCODE_P] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT)
		{
			App->frisbee->xspeed = -4;
			App->frisbee->yspeed = -4;
			App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
			App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::PARABOLA;
			App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::DABAJO;
			estadoP2 = estadoPlayer2::MOVIMIENTO;
			break;

		}

		if (App->input->keys[SDL_SCANCODE_P] == Key_State::KEY_DOWN)
		{
			App->frisbee->xspeed = -4;
			App->frisbee->yspeed = 0;
			App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
			App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::PARABOLA;
			App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::HORIZONTAL;
			estadoP2 = estadoPlayer2::MOVIMIENTO;
			break;
		}
	}
}

void ModulePlayer2::timerP2() {
	currentTimeP2 = SDL_GetTicks();

	if (currentTimeP2 - initialTimeP2 >= timeLimitP2) {
		estadoTP2 = estadoTimerP2::FIN;
	}
} 