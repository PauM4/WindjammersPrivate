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
#include "SceneCharacterSelect.h"

//Venga developer;)


#include <stdio.h>

ModulePlayer2::ModulePlayer2(bool startEnabled) : Module(startEnabled)
{


	
}

ModulePlayer2::~ModulePlayer2()
{

}

bool ModulePlayer2::Start()
{

	score2 = 0;
	last2 = 0;
	score = 000;
	scoreFont = -1;
	round = 0;
	estadoP2 = STOP;
	bool ret = true;

	LOG("Loading player textures");

	stopLoadFX = false;

	tossFX = App->audio->LoadFx("Assets/Fx/Toss.wav");
	hiromiChargeFX = App->audio->LoadFx("Assets/Fx/HiromiCharge.wav");
	hiromiPowerSoundFX = App->audio->LoadFx("Assets/Fx/HiromiPowerSound.wav");

	yooChargeFX = App->audio->LoadFx("Assets/Fx/B.YooCharge.wav");
	yooPowerSoundFX = App->audio->LoadFx("Assets/FX/B.YooSuperSound.wav");

	wesselChargeFX = App->audio->LoadFx("Assets/Fx/KlaussCharge.wav");
	wesselPowerSoundFX = App->audio->LoadFx("Assets/Fx/KlaussSuperSound.wav");

	dashFX = App->audio->LoadFx("Assets/Fx/Dash.wav");

	switch (App->sceneCharacterSelect->p2Char) {
	case(CharList::Mita):
		texture = App->textures->Load("Assets/Sprites/Characters/Jap2.png");
		speed = 2;

		posicionInicialX = 230;
		posicionInicialY = 100;

		ajusteColliderX = 16;
		ajusteColliderY = 10;


		lanzamientoXSpeed = -4;
		lanzamientoYSpeed = -4;
		parabolaXSpeed = -3;
		parabolaYSpeed = 0;

		collider = App->collisions->AddCollider({ (int)position.x + ajusteColliderX, (int)position.y + ajusteColliderY, 27, 31 }, Collider::Type::PLAYER, this);

		//idleLAnim
		for (int i = 0; i < 8; i++) {
			idleLAnim.PushBack({ 211 + (i * 53), 338, 53, 57 });
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
			rightAnim.PushBack({ 319 + (i * 53), 399, 53, 57 });
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
		idleDisk.PushBack({ 530,627,53,57 });
		idleDisk.PushBack({ 477,627,53,57 });
		idleDisk.PushBack({ 530,627,53,57 });
		idleDisk.PushBack({ 424,627,53,57 });

		idleDisk.loop = true;
		idleDisk.speed = 0.075f;

		//Lanzamiento Disco
		for (int i = 5; i >= 0; i--) {
			lanzamiento.PushBack({ 105 + (i * 53), 625, 53, 57 });
		}
		lanzamiento.loop = false;
		lanzamiento.speed = 0.3f;

		//Dash derecho
		for (int i = 0; i < 3; i++) {
			dashRight.PushBack({ 316 + (i * 53), 112, 53, 57 });
		}
		dashRight.loop = false;
		dashRight.speed = 0.45f;

		//Dash izquierdo
		for (int i = 2; i >= 0; i--) {
			dashLeft.PushBack({ 265 + (i * 53), 450, 53, 57 });
		}
		dashLeft.loop = false;
		dashLeft.speed = 0.45f;

		//Dash arriba
		for (int i = 0; i < 3; i++) {
			dashUp.PushBack({ 477 + (i * 53), 115, 53, 57 });
		}
		dashUp.loop = false;
		dashUp.speed = 0.45f;

		//Dash abajo
		for (int i = 0; i < 3; i++) {
			dashDown.PushBack({ 55 + (i * 53), 170, 53, 57 });
		}
		dashDown.loop = false;
		dashDown.speed = 0.45f;

		//Dash diagonalUpRight

		diagonalUpRight.PushBack({ 635, 112, 53, 57 });
		diagonalUpRight.PushBack({ 635 + 53, 112, 53, 57 });
		diagonalUpRight.PushBack({ 0, 170, 53, 57 });

		diagonalUpRight.loop = false;
		diagonalUpRight.speed = 0.45f;

		//Dash diagonalDownRight
		for (int i = 0; i < 3; i++) {
			diagonalDownRight.PushBack({ 210 + (i * 53), 170, 53, 57 });
		}
		diagonalDownRight.loop = false;
		diagonalDownRight.speed = 0.45f;

		//Dash diagonalDownLeft
		for (int i = 2; i >= 0; i--) {
			diagonalDownLeft.PushBack({ 372 + (i * 53), 510, 53, 57 });
		}
		diagonalDownLeft.loop = false;
		diagonalDownLeft.speed = 0.45f;


		//Dash diagonalUpLeft
		diagonalUpLeft.PushBack({ 0, 455, 53, 57 });
		diagonalUpLeft.PushBack({ 53, 170, 53, 57 });
		diagonalUpLeft.PushBack({ 689, 510, 53, 57 });
		diagonalUpLeft.loop = false;
		diagonalUpLeft.speed = 0.45f;

		//Win
		for (int i = 0; i < 3; i++) {
			win.PushBack({ 0 + (i * 53), 229, 53, 57 });
		}
		win.loop = true;
		win.speed = 0.05f;

		//Lose
		for (int i = 0; i < 5; i++) {
			lose.PushBack({ 159 + (i * 53), 229, 53, 57 });
		}
		lose.loop = true;
		lose.speed = 0.05f;
		break;

	case(CharList::Yoo):
		texture = App->textures->Load("Assets/Sprites/Characters/Kor2.png");
		speed = 2;

		posicionInicialX = 230;
		posicionInicialY = 100;

		ajusteColliderX = 16;
		ajusteColliderY = 10;


		lanzamientoXSpeed = -4;
		lanzamientoYSpeed = -4;
		parabolaXSpeed = -3;
		parabolaYSpeed = 0;

		collider = App->collisions->AddCollider({ (int)position.x + ajusteColliderX, (int)position.y + ajusteColliderY, 27, 31 }, Collider::Type::PLAYER, this);

		//idleLAnim
		for (int i = 0; i < 8; i++) {
			idleLAnim.PushBack({ 462 + (i * 66), 462, 66, 66 });
		}
		idleLAnim.loop = true;
		idleLAnim.speed = 0.075f;

		//idleRAnim
		for (int i = 0; i < 8; i++) {
			idleRAnim.PushBack({ 198 + (i * 66), 0, 66, 66 });
		}
		idleRAnim.loop = true;
		idleRAnim.speed = 0.075f;

		// Move Right
		for (int i = 0; i < 6; i++) {
			rightAnim.PushBack({ 792 + (i * 66), 0, 66, 66 });
		}
		rightAnim.loop = true;
		rightAnim.speed = 0.075f;

		//Move Left
		for (int i = 0; i < 6; i++) {
			leftAnim.PushBack({ 66 + (i * 66), 462, 66, 66 });
		}
		leftAnim.loop = true;
		leftAnim.speed = 0.075f;

		// Move Down Head Right
		for (int i = 0; i < 6; i++) {
			downRAnim.PushBack({ 792 + (i * 66), 66, 66, 66 });
		}
		downRAnim.loop = true;
		downRAnim.speed = 0.075f;

		// Move Down Head Left
		for (int i = 0; i < 6; i++) {
			downLAnim.PushBack({ 66 + (i * 66), 528, 66, 66 });
		}
		downLAnim.loop = true;
		downLAnim.speed = 0.075f;

		//Move Up Head Left
		for (int i = 0; i < 6; i++) {
			upLAnim.PushBack({ 462 + (i * 66), 528, 66, 66 });
		}
		upLAnim.loop = true;
		upLAnim.speed = 0.075f;

		//Move Up Head Right
		for (int i = 0; i < 6; i++) {
			upRAnim.PushBack({ 330 + (i * 66), 66, 66, 66 });
		}
		upRAnim.loop = true;
		upRAnim.speed = 0.075f;

		//Idle Disk
		for (int i = 0; i < 8; i++) {
			idleDisk.PushBack({ 660 + (i * 66), 792, 66, 66 });
		}
		idleDisk.loop = true;
		idleDisk.speed = 0.075f;

		//Lanzamiento Disco
		for (int i = 5; i >= 0; i--) {
			lanzamiento.PushBack({ 396 + (i * 66), 858, 66, 66 });
		}
		lanzamiento.loop = false;
		lanzamiento.speed = 0.3f;

		//Dash derecho
		dashRight.PushBack({ 1122, 66, 66, 66 });
		for (int i = 0; i < 3; i++) {
			dashRight.PushBack({ 0 + (i * 66), 132, 66, 66 });
		}
		dashRight.loop = false;
		dashRight.speed = 0.45f;

		//Dash izquierdo
		dashRight.PushBack({ 0, 528, 66, 66 });
		for (int i = 3; i >= 0; i--) {
			dashLeft.PushBack({ 1122 + (i * 66), 594, 66, 66 });
		}
		dashLeft.loop = false;
		dashLeft.speed = 0.45f;

		//Dash arriba
		for (int i = 0; i < 3; i++) {
			dashUp.PushBack({ 198 + (i * 66), 132, 66, 66 });
		}
		dashUp.loop = false;
		dashUp.speed = 0.45f;

		//Dash abajo
		dashDown.PushBack({ 726, 132, 66, 66 });
		dashDown.PushBack({ 792, 132, 66, 66 });
		dashDown.PushBack({ 1122, 132, 66, 66 });
		dashDown.PushBack({ 132, 198, 66, 66 });
		dashDown.loop = false;
		dashDown.speed = 0.45f;

		//Win
		for (int i = 0; i < 3; i++) {
			win.PushBack({ 132 + (i * 66), 264, 66, 66 });
		}
		win.loop = true;
		win.speed = 0.05f;

		//Lose
		for (int i = 0; i < 6; i++) {
			lose.PushBack({ 330 + (i * 66), 264, 66, 66 });
		}
		lose.loop = true;
		lose.speed = 0.05f;

		//Dash diagonalUpRight
		for (int i = 0; i < 4; i++) {
			diagonalUpRight.PushBack({ 462 + (i * 66), 66, 66, 66 });
		}
		diagonalUpRight.loop = false;
		diagonalUpRight.speed = 0.45f;

		//Dash diagonalDownRight
		for (int i = 0; i < 4; i++) {
			diagonalDownRight.PushBack({ 198 + (i * 66), 132, 66, 66 });
		}
		diagonalDownRight.loop = false;
		diagonalDownRight.speed = 0.45f;

		//Dash diagonalDownLeft
		for (int i = 3; i >= 0; i--) {
			diagonalDownLeft.PushBack({ 726 + (i * 66), 660, 66, 66 });
		}
		diagonalDownLeft.loop = false;
		diagonalDownLeft.speed = 0.45f;

		//Dash diagonalUpLeft
		for (int i = 3; i >= 0; i--) {
			diagonalUpLeft.PushBack({ 462 + (i * 66), 594, 66, 66 });
		}
		diagonalUpLeft.loop = false;
		diagonalUpLeft.speed = 0.45f;

		break;

	case(CharList::Wessel):
		texture = App->textures->Load("Assets/Sprites/Characters/Ger2.png");
		speed = 2;

		posicionInicialX = 230;
		posicionInicialY = 100;

		ajusteColliderX = 16;
		ajusteColliderY = 10;


		lanzamientoXSpeed = -4;
		lanzamientoYSpeed = -4;
		parabolaXSpeed = -3;
		parabolaYSpeed = 0;

		collider = App->collisions->AddCollider({ (int)position.x + ajusteColliderX, (int)position.y + ajusteColliderY, 27, 31 }, Collider::Type::PLAYER, this);
		
		//idleLAnim
		for (int i = 0; i < 3; i++) {
			idleLAnim.PushBack({ 825 + (i * 75), 390, 75, 65 });
		}
		idleLAnim.loop = true;
		idleLAnim.speed = 0.075f;

		//idleRAnim
		for (int i = 0; i < 3; i++) {
			idleRAnim.PushBack({ 225 + (i * 75), 0, 75, 65 });
		}
		idleRAnim.loop = true;
		idleRAnim.speed = 0.075f;

		// Move Right
		for (int i = 0; i < 6; i++) {
			rightAnim.PushBack({ 450 + (i * 75), 0, 75, 65 });
		}
		rightAnim.loop = true;
		rightAnim.speed = 0.075f;

		//Move Left
		for (int i = 0; i < 6; i++) {
			leftAnim.PushBack({ 375 + (i * 75), 390, 75, 65 });
		}
		leftAnim.loop = true;
		leftAnim.speed = 0.075f;

		// Move Down Head Right
		for (int i = 0; i < 6; i++) {
			downRAnim.PushBack({ 525 + (i * 75), 65, 75, 65 });
		}
		downRAnim.loop = true;
		downRAnim.speed = 0.075f;

		// Move Down Head Left
		for (int i = 0; i < 6; i++) {
			downLAnim.PushBack({ 525 + (i * 75), 65, 75, 65 });
		}
		downLAnim.loop = true;
		downLAnim.speed = 0.075f;

		//Move Up Head Left
		for (int i = 0; i < 6; i++) {
			upLAnim.PushBack({ 75 + (i * 75), 65, 75, 65 });
		}
		upLAnim.loop = true;
		upLAnim.speed = 0.075f;

		//Move Up Head Right
		for (int i = 0; i < 6; i++) {
			upRAnim.PushBack({ 75 + (i * 75), 65, 75, 65 });
		}
		upRAnim.loop = true;
		upRAnim.speed = 0.075f;

		//Idle Disk
		idleDisk.PushBack({ 75, 650, 75, 65 });
		idleDisk.PushBack({ 0, 650, 75, 65 });
		idleDisk.PushBack({ 1200, 715, 75, 65 });
		idleDisk.PushBack({ 1125, 715, 75, 65 });
		idleDisk.loop = true;
		idleDisk.speed = 0.075f;

		//Lanzamiento Disco
		for (int i = 5; i >= 0; i--) {
			lanzamiento.PushBack({ 75 + (i * 75), 715, 75, 65 });
		}
		lanzamiento.loop = false;
		lanzamiento.speed = 0.3f;

		//Dash derecho
		for (int i = 0; i < 4; i++) {
			dashRight.PushBack({ 975 + (i * 75), 65, 75, 65 });
		}
		dashRight.loop = false;
		dashRight.speed = 0.45f;

		//Dash izquierdo
		for (int i = 3; i >= 0; i--) {
			dashLeft.PushBack({ 0 + (i * 75), 455, 75, 65 });
		}
		dashLeft.loop = false;
		dashLeft.speed = 0.45f;

		//Dash arriba
		for (int i = 0; i < 4; i++) {
			dashUp.PushBack({ 0 + (i * 75), 130, 75, 65 });
		}
		dashUp.loop = false;
		dashUp.speed = 0.45f;

		//Dash abajo
		for (int i = 0; i < 4; i++) {
			dashDown.PushBack({ 675 + (i * 75), 130, 75, 65 });
		}
		dashDown.loop = false;
		dashDown.speed = 0.45f;

		//Win
		for (int i = 0; i < 7; i++) {
			win.PushBack({ 450 + (i * 75), 195, 75, 65 });
		}
		win.loop = true;
		win.speed = 0.05f;

		//Lose
		for (int i = 0; i < 4; i++) {
			lose.PushBack({ 975 + (i * 75), 195, 75, 65 });
		}
		lose.PushBack({ 0, 260, 75, 65 });
		lose.PushBack({ 75, 260, 75, 65 });
		lose.loop = true;
		lose.speed = 0.05f;

		//Dash diagonalUpRight
		for (int i = 0; i < 4; i++) {
			diagonalUpRight.PushBack({ 375 + (i * 75), 130, 75, 65 });
		}
		diagonalUpRight.loop = false;
		diagonalUpRight.speed = 0.45f;

		//Dash diagonalDownRight
		for (int i = 0; i < 4; i++) {
			diagonalDownRight.PushBack({ 975 + (i * 75), 130, 75, 65 });
		}
		diagonalDownRight.loop = false;
		diagonalDownRight.speed = 0.45f;

		//Dash diagonalDownLeft
		for (int i = 3; i >= 0; i--) {
			diagonalDownLeft.PushBack({ 0 + (i * 75), 520, 75, 65 });
		}
		diagonalDownLeft.loop = false;
		diagonalDownLeft.speed = 0.45f;

		//Dash diagonalUpLeft
		for (int i = 3; i >= 0; i--) {
			diagonalUpLeft.PushBack({ 600 + (i * 75), 520, 75, 65 });
		}
		diagonalUpLeft.loop = false;
		diagonalUpLeft.speed = 0.45f;

		break;
	}

	currentAnimation = &idleLAnim;

	position.x = posicionInicialX;
	position.y = posicionInicialY;

	char lookupTable[] = { "0123456789G " };
	scoreFont = App->fonts->Load("Assets/Sprites/UI/Fonts/scoreFont.png", lookupTable, 1);

	char lookupTableDebug[] = { "! ?,_./0123456789?;<??ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
	debugFont = App->fonts->Load("Assets/Sprites/UI/Fonts/debugFont.png", lookupTableDebug, 2);
	isDebugAppear = false;


	return ret;
}

Update_Status ModulePlayer2::Update()
{
	
	switch (estadoP2) {
	case (STOP):
		break;
	case (MOVIMIENTO):
		limitePlayer2();
		movimientoPlayer2();
		break;

	case (WITHFRISBEE):
		timerP2();
		bea = (currentTimeP2 - initialTimeP2) / 1000;
		lanzamientoPlayer2();
		break;

	case (LANZAMIENTO):
		if (estadoTP2 == INICIO)
		{
			initialTimeP2 = SDL_GetTicks();
			timeLimitP2 = 0.2 * 1000;
			currentAnimation = &lanzamiento;
			estadoTP2 = EJECUTANDO;
		}
		else if (estadoTP2 == EJECUTANDO) {
			timerP2();
		}
		else if (estadoTP2 == FIN)
		{
			App->audio->PlayFx(tossFX);
			estadoTP2 = INICIO;
			estadoP2 = MOVIMIENTO;
			App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
			lanzamiento.Reset();
			currentAnimation = &idleRAnim;
			last2 = 0;
		}
		break;

	case (LANZAMIENTO_SUPER):
		if (estadoTP2 == INICIO)
		{
			initialTimeP2 = SDL_GetTicks();
			timeLimitP2 = 0.8 * 1000;
			lanzamiento.loop = true;
			currentAnimation = &lanzamiento; //lanzamiento supershot animation
			estadoTP2 = EJECUTANDO;
		}
		else if (estadoTP2 == EJECUTANDO) {
			timerP2();
		}
		else if (estadoTP2 == FIN)
		{
			estadoTP2 = INICIO;
			estadoP2 = MOVIMIENTO;
			App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::MOVIMIENTO;
			lanzamiento.Reset();
			currentAnimation = &idleRAnim;
			last2 = 0;
			lanzamiento.loop = false;
		}
		break;
	}
	


	collider->SetPos(position.x + ajusteColliderX, position.y + ajusteColliderY);
	currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePlayer2::PostUpdate()
{
	
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->render->Blit(texture, position.x, position.y, &rect);
	

	//// Draw UI (score) --------------------------------------
	//if (App->sceneBeachStage->estadoS != App->sceneBeachStage->INICIO)
	//{
	//	sprintf_s(scoreText, 10, "%2d", score);

	//	App->fonts->BlitText(165, 17, scoreFont, scoreText);
	//}

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

	if (App->frisbee->position.x > 150 && App->frisbee->estadoF == App->frisbee->BLOCK && !stopLoadFX)
	{
		switch (App->sceneCharacterSelect->p2Char)
		{
		case Mita:
			App->particles->AddParticle(0, 0, App->particles->mitaLoadShotParticle, position.x, position.y, Collider::NONE, 1);
			App->audio->PlayFx(hiromiChargeFX);
			App->audio->PlayFx(hiromiPowerSoundFX);
			break;
		case Yoo:
			App->particles->AddParticle(0, 0, App->particles->yooLoadShotParticle, position.x, position.y, Collider::NONE, 1);
			App->audio->PlayFx(yooChargeFX);
			App->audio->PlayFx(yooPowerSoundFX);
			break;
		case Wessel:
			App->particles->AddParticle(0, 0, App->particles->wesselLoadShotParticle, position.x, position.y, Collider::NONE, 1);
			App->audio->PlayFx(wesselChargeFX);
			App->audio->PlayFx(wesselPowerSoundFX);
			break;
		}

		stopLoadFX = true;
	}
	else if (App->frisbee->estadoF == App->frisbee->MOVIMIENTO)
	{
		stopLoadFX = false;
	}


	return Update_Status::UPDATE_CONTINUE;
}

void ModulePlayer2::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && c2 == App->frisbee->collider)
	{//le pongo 0,0 pq no se exactamente q es esto y como he cambiado la funcion como tal tengo q meterle estos parametros i o si
		
		estadoP2 = estadoPlayer2::WITHFRISBEE;
		App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::WITHPLAYER;
		currentAnimation = &idleDisk;

		initialTimeP2 = SDL_GetTicks();
		timeLimitP2 = 2 * 1000;
		estadoTP2 = estadoTimerP2::EJECUTANDO;

		App->player->estadoP1 = ModulePlayer::estadoPlayer::MOVIMIENTO;
		App->frisbee->limiteConcrete = true;
	}			

	
}

void ModulePlayer2::movimientoPlayer2() {
	//MOVIMIENTO

	if ((App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_REPEAT) && position.x < 258)
	{
		position.x += speed;

		if (currentAnimation != &rightAnim && App->input->keys[SDL_SCANCODE_UP] != Key_State::KEY_REPEAT && App->input->keys[SDL_SCANCODE_DOWN] != Key_State::KEY_REPEAT)
		{
			rightAnim.Reset();
			currentAnimation = &rightAnim;
		}
		last2 = 1;

		if (App->input->keys[SDL_SCANCODE_O] == Key_State::KEY_DOWN && estadoTP2 == INICIO) {

			initialTimeP2 = SDL_GetTicks();
			timeLimitP2 = 2 * 1000;
			estadoTP2 = EJECUTANDO;

		}
		else if (App->input->keys[SDL_SCANCODE_O] == Key_State::KEY_REPEAT && estadoTP2 == EJECUTANDO)
		{
			App->audio->PlayFx(dashFX);
			App->particles->AddParticle(0, 0, App->particles->dustParticle, position.x, position.y + 35, Collider::NONE, 0);
			if (App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT) {
				timerP2();
				position.y -= 0.75 * speed;
				position.x += 0.75 * speed;
				currentAnimation = &diagonalUpRight;
			}
			else if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT) {
				timerP2();
				position.y += 0.75 * speed;
				position.x += 0.75 * speed;
				currentAnimation = &diagonalDownRight;
			}
			else {
				timerP2();
				position.x += 1, 5 * speed;
				currentAnimation = &dashRight;

			}


		}
		else if (estadoTP2 == FIN) {
			estadoTP2 = INICIO;
		}
		else {

		}


	
	}

	else if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_REPEAT && position.x > 140)
	{
		position.x -= speed;

		if (currentAnimation != &leftAnim && App->input->keys[SDL_SCANCODE_UP] != Key_State::KEY_REPEAT && App->input->keys[SDL_SCANCODE_DOWN] != Key_State::KEY_REPEAT)
		{
			leftAnim.Reset();
			currentAnimation = &leftAnim;
		}
		last2 = 0;

		if (App->input->keys[SDL_SCANCODE_O] == Key_State::KEY_DOWN && estadoTP2 == INICIO) {

			initialTimeP2 = SDL_GetTicks();
			timeLimitP2 = 2 * 1000;
			estadoTP2 = EJECUTANDO;

		}
		else if (App->input->keys[SDL_SCANCODE_O] == Key_State::KEY_REPEAT && estadoTP2 == EJECUTANDO)
		{
			App->audio->PlayFx(dashFX);
			App->particles->AddParticle(0, 0, App->particles->dustParticle, position.x + 35, position.y + 35, Collider::NONE, 0);
			if (App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT) {
				timerP2();
				position.y -= 0.75 * speed;
				position.x -= 0.75 * speed;
				currentAnimation = &diagonalUpLeft;
			}
			else if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT) {
				timerP2();
				position.y += 0.75 * speed;
				position.x -= 0.75 * speed;
				currentAnimation = &diagonalDownLeft;
			}
			else {
				timerP2();
				position.x -= 1, 5 * speed;
				currentAnimation = &dashLeft;
			}



		}
		else if (estadoTP2 == FIN) {
			estadoTP2 = INICIO;
		}

	}


	else if (App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT && position.y > 50)
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

		if (App->input->keys[SDL_SCANCODE_O] == Key_State::KEY_DOWN && estadoTP2 == INICIO) {

			initialTimeP2 = SDL_GetTicks();
			timeLimitP2 = 2 * 1000;
			estadoTP2 = EJECUTANDO;

		}
		else if (App->input->keys[SDL_SCANCODE_O] == Key_State::KEY_REPEAT && estadoTP2 == EJECUTANDO)
		{
			App->audio->PlayFx(dashFX);
			App->particles->AddParticle(0, 0, App->particles->dustParticle, position.x + 15, position.y + 25, Collider::NONE, 0);
			if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_REPEAT) {
				timerP2();
				position.y -= 0.75 * speed;
				position.x += 0.75 * speed;
				currentAnimation = &diagonalUpRight;
			}
			else if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_REPEAT) {
				timerP2();
				position.y -= 0.75 * speed;
				position.x -= 0.75 * speed;
				currentAnimation = &diagonalUpLeft;
			}
			else {
				timerP2();
				position.y -= 1, 5 * speed;
				currentAnimation = &dashUp;
			}


		}
		else if (estadoTP2 == FIN) {
			estadoTP2 = INICIO;
		}

	}

	else if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT && position.y < 138)
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

		if (App->input->keys[SDL_SCANCODE_O] == Key_State::KEY_DOWN && estadoTP2 == INICIO) {
			pols = true;
			initialTimeP2 = SDL_GetTicks();
			timeLimitP2 = 2 * 1000;
			estadoTP2 = EJECUTANDO;

		}
		else if (App->input->keys[SDL_SCANCODE_O] == Key_State::KEY_REPEAT && estadoTP2 == EJECUTANDO)
		{
			App->audio->PlayFx(dashFX);
			App->particles->AddParticle(0, 0, App->particles->dustParticle, position.x + 15, position.y, Collider::NONE, 0);
			if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_REPEAT) {
				timerP2();
				position.y += 0.75 * speed;
				position.x += 0.75 * speed;
				currentAnimation = &diagonalDownRight;
			}
			else if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_REPEAT) {
				timerP2();
				position.y += 0.75 * speed;
				position.x -= 0.75 * speed;
				currentAnimation = &diagonalDownLeft;
			}
			else {
				timerP2();
				position.y += 1, 5 * speed;
				currentAnimation = &dashDown;
			}

		}
		else if (estadoTP2 == FIN) {
			estadoTP2 = INICIO;
		}

	}

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_IDLE && last2 == 0)
		currentAnimation = &idleLAnim;

	if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_IDLE && last2 == 1)
		currentAnimation = &idleRAnim;



	if (App->input->keys[SDL_SCANCODE_I] == Key_State::KEY_DOWN && ((position.x) - App->frisbee->position.x ) > 1 && ((position.x) - App->frisbee->position.x) < 40 && App->frisbee->lanzamientoF == ModuleFrisbee::tipoLanzamiento::NORMAL) {

		if (App->frisbee->position.y >= position.y && App->frisbee->position.y <= (position.y + 31)) {
	
			App->frisbee->estadoF = ModuleFrisbee::estadoFrisbee::BLOCK;
			App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::BLOCKPLAYER2;

		}



	}


}

void ModulePlayer2::lanzamientoPlayer2() {

	pepe = (float)bea + 1;
	if (pepe > 1.5) {
		pepe = 1.5;
	}

	for (int i = 0; i < 1; i++) {
		if (App->input->keys[SDL_SCANCODE_O] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT)
		{

			App->frisbee->xspeed = lanzamientoXSpeed /pepe;
			App->frisbee->yspeed = lanzamientoYSpeed / pepe;
			App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::NORMAL;
			App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::DARRIBA;
			estadoP2 = estadoPlayer2::LANZAMIENTO;
			estadoTP2 = estadoTimerP2::INICIO;
			break;
		}


		if (App->input->keys[SDL_SCANCODE_O] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT)
		{
			App->frisbee->xspeed = lanzamientoXSpeed / pepe;
			App->frisbee->yspeed = -lanzamientoYSpeed / pepe;
			App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::NORMAL;
			App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::DABAJO;
			estadoP2 = estadoPlayer2::LANZAMIENTO;
			estadoTP2 = estadoTimerP2::INICIO;
			break;

		}

		if (App->input->keys[SDL_SCANCODE_O] == Key_State::KEY_DOWN || estadoTP2 == FIN)
		{
			App->frisbee->xspeed = -4 / pepe;
			App->frisbee->yspeed = lanzamientoYSpeed*0;
			App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::NORMAL;
			App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::HORIZONTAL;
			estadoP2 = estadoPlayer2::LANZAMIENTO;
			estadoTP2 = estadoTimerP2::INICIO;

			break;
		}
		//}

		//LANZAMIENTO DE DISCO PAR�BOLA

		if (App->input->keys[SDL_SCANCODE_P] == Key_State::KEY_DOWN)
		{
			App->collisions->RemoveCollider(App->frisbee->collider);

			App->frisbee->xspeed = parabolaXSpeed;
			App->frisbee->yspeed = parabolaYSpeed;
			App->frisbee->vel_parabola(position.x, 23);
			App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::PARABOLA;
			App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::HORIZONTAL;
			estadoP2 = estadoPlayer2::LANZAMIENTO;
			estadoTP2 = estadoTimerP2::INICIO;
			break;

		}


		//LANZAMIENTO SUPERSHOT
		if (App->sceneCharacterSelect->p2Char == CharList::Mita) { //japo

			if (App->input->keys[SDL_SCANCODE_I] == Key_State::KEY_DOWN /*&& (App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT || App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT)*/ && App->frisbee->lanzamientoF == ModuleFrisbee::BLOCKPLAYER2)
			{
				App->frisbee->angulo = 0;
				App->frisbee->xspeed = -5;
				App->frisbee->yspeed = -5;
				App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::SUPERSHOT;
				App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::MAX;
				App->frisbee->tipoSupershot = ModuleFrisbee::tipoSupershot::MITA_SUPERSHOT;
				estadoP2 = estadoPlayer2::LANZAMIENTO_SUPER;
				estadoTP2 = estadoTimerP2::INICIO;
				break;

			}

		}
		else if (App->sceneCharacterSelect->p2Char == CharList::Yoo) { //coreano

			if (App->input->keys[SDL_SCANCODE_I] == Key_State::KEY_DOWN /*&& (App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT || App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT)*/ && App->frisbee->lanzamientoF == ModuleFrisbee::BLOCKPLAYER2)
			{
				App->frisbee->xspeed = -5;
				App->frisbee->yspeed = -5;
				App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::SUPERSHOT;
				App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::MAX;
				App->frisbee->tipoSupershot = ModuleFrisbee::tipoSupershot::YOO_SUPERSHOT;
				estadoP2 = estadoPlayer2::LANZAMIENTO_SUPER;
				estadoTP2 = estadoTimerP2::INICIO;
				break;

			}

		}
		else if (App->sceneCharacterSelect->p2Char == CharList::Wessel) { //aleman

			if (App->input->keys[SDL_SCANCODE_I] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT && App->frisbee->lanzamientoF == ModuleFrisbee::BLOCKPLAYER2)
			{
				App->frisbee->limiteWesselSupershot = 130;
				App->frisbee->xspeed = -5;
				App->frisbee->yspeed = -5;
				App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::SUPERSHOT;
				App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::MAX;
				App->frisbee->tipoSupershot = ModuleFrisbee::tipoSupershot::WESSEL_SUPERSHOT;
				estadoP2 = estadoPlayer2::LANZAMIENTO_SUPER;
				estadoTP2 = estadoTimerP2::INICIO;
				break;

			}

			if (App->input->keys[SDL_SCANCODE_I] == Key_State::KEY_DOWN && App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT && App->frisbee->lanzamientoF == ModuleFrisbee::BLOCKPLAYER2)
			{
				App->frisbee->limiteWesselSupershot = 130;
				App->frisbee->xspeed = -5;
				App->frisbee->yspeed = 5;
				App->frisbee->lanzamientoF = ModuleFrisbee::tipoLanzamiento::SUPERSHOT;
				App->frisbee->direccionF = ModuleFrisbee::direccionFrisbeePlayer::MAX;
				App->frisbee->tipoSupershot = ModuleFrisbee::tipoSupershot::WESSEL_SUPERSHOT;
				estadoP2 = estadoPlayer2::LANZAMIENTO_SUPER;
				estadoTP2 = estadoTimerP2::INICIO;
				break;

			}

		}






	}
}

void ModulePlayer2::timerP2() {
	currentTimeP2 = SDL_GetTicks();

	if (currentTimeP2 - initialTimeP2 >= timeLimitP2) {
		estadoTP2 = estadoTimerP2::FIN;
	}
} 

void ModulePlayer2::limitePlayer2() {

	if (position.x > App->sceneBeachStage->limiteDerecha) {
		if (position.y < App->sceneBeachStage->limiteSuperior) {
			position.y = App->sceneBeachStage->limiteSuperior+2;
			position.x = App->sceneBeachStage->limiteDerecha-2;
		}
		else if (position.y > App->sceneBeachStage->limiteInferior) {
			position.y = App->sceneBeachStage->limiteInferior-2;
			position.x = App->sceneBeachStage->limiteDerecha-2;
		}
		else {
			position.x = App->sceneBeachStage->limiteDerecha-2;
		}
	} 
	else if (position.x < App->sceneBeachStage->limiteCentralDer) {
		if (position.y < App->sceneBeachStage->limiteSuperior) {
			position.y = App->sceneBeachStage->limiteSuperior+2;
			position.x = App->sceneBeachStage->limiteCentralDer+2;

		}
		else if (position.y > App->sceneBeachStage->limiteInferior) {
			position.y = App->sceneBeachStage->limiteInferior-2;
			position.x = App->sceneBeachStage->limiteCentralDer+2;
		}
		else {
			position.x = App->sceneBeachStage->limiteCentralDer+2;
		}
	}


	if (position.y > App->sceneBeachStage->limiteInferior) {
		position.y = App->sceneBeachStage->limiteInferior-2;
	} 
	else if (position.y < App->sceneBeachStage->limiteSuperior) {
		position.y = App->sceneBeachStage->limiteSuperior+2;
	}


	
}

/*position.x = 230;
position.y = 9*/