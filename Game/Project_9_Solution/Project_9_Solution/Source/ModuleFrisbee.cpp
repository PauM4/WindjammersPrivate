#include "ModuleFrisbee.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModulePlayer.h"
#include "ModulePlayer2.h"
#include "SDL/include/SDL.h"
#include "SceneBeachStage.h"

ModuleFrisbee::ModuleFrisbee(bool startEnabled) : Module(startEnabled)
{
	// moving animation
	moving.PushBack({ 117, 48, 16, 16 });
	moving.PushBack({ 149, 48, 16, 16 });
	moving.PushBack({ 181, 48, 16, 16 });
	moving.PushBack({ 213, 48, 16, 16 });
	moving.loop = true;
	moving.speed = 0.1f;

	// Projectile motion animation
	moving.PushBack({ 117, 48, 16, 16 });
	projectile.PushBack({ 35, 8, 32, 14 });
	projectile.PushBack({ 53, 7, 25, 31 });
	projectile.PushBack({ 79, 6, 36, 34 });
	projectile.PushBack({ 116, 5, 28, 36 });
	projectile.PushBack({ 145, 4, 21, 38 });
	projectile.PushBack({ 167, 3, 3, 39 });
	projectile.PushBack({ 171, 2, 23, 41 });
	projectile.PushBack({ 195, 2, 33, 42 });
	projectile.PushBack({ 229, 0, 43, 44 });
	projectile.loop = false;
	projectile.pingpong = true;
	projectile.speed = 0.3f;

	//Desaparece: que no hace falta hacer nada
	//Stop
	stop.PushBack({ 117, 48, 16, 16 });
	stop.loop = false;
	
	

}

ModuleFrisbee::~ModuleFrisbee()
{

}

bool ModuleFrisbee::Start()
{
	bool ret = true;
	speed = 1;
	arbitro = 1;
	pared = false;
	inicio = false;
	posesion = false;
	destroyed = false;

	currentAnimation2 = &moving;
	LOG("Loading frisbee textures");

	

	texture = App->textures->Load("Assets/Sprites/Levels/Beach.png");


	position.x = 150;
	position.y = 200;
	projectil = 0;
	FloorTime = 0;


	destroyed = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 16, 16 }, Collider::Type::FRISBEE, this);
	estadoF = STOP; 
	lanzamientoF = ARBITRO;
	estadoTF = INICIO;
	blockSuperShot = true;
	return ret;
}

Update_Status ModuleFrisbee::Update()
{

	switch (estadoF) {

	case STOP:
		currentAnimation2 = &desaparece;
		break;

	case MOVIMIENTO:
		limitesFrisbee();
		movimientoFrisbee();
		break;	

	case WITHPLAYER:
		currentAnimation2 = &stop;
		if (App->player->estadoP1 == ModulePlayer::estadoPlayer::WITHFRISBEE) {
			App->frisbee->position.x = position.x + 28;
			App->frisbee->position.y = position.y;
		}
		else if (App->player2->estadoP2 == ModulePlayer2::estadoPlayer2::WITHFRISBEE) {
			App->frisbee->position.x = position.x - 17;
			App->frisbee->position.y = position.y;
		}
		break;
	
	case BLOCK:

		//animaciones del disco volando
		if (estadoTF == INICIO) {
			initialTimeF = SDL_GetTicks();
			timeLimitF = 2 * 1000;
			estadoTF = EJECUTANDO;
			currentAnimation2 = &projectile;
			App->collisions->RemoveCollider(App->frisbee->collider);
		}
		
		if (estadoTF == FIN) {

			//collider = App->collisions->AddCollider({ position.x, position.y, 16,16 }, Collider::Type::FRISBEE, this);
			blockSuperShot = true;
		}


		break;
	}


	//while (position.x != App->player->position.x && position.y != App->player->position.y) {
		/*uint delay = 1500;
		SDL_Delay(delay);*/

	//TIMER SUELO FRISBBE
	/*if (FloorTime < 120 && currentAnimation2== &stop)
	{
		FloorTime++;
		
	}*/
	

	

	////MOV FRISBE PROJECTIL HACIA DELANTE
	//

	//if (position.x <= 19 || position.x >= 276) {
	//	mov = 0;

	//	//aqui tendremos que llamar la accion del arbitro que envia el disco al player 1

	//	if (position.x <= 19) {
	//		if (position.y >= 94 && position.y <= 144) {
	//			App->player2->score += 5;
	//			//arbitro = 1;
	//			if (App->sceneBeachStage->suddenDeath) {
	//				App->sceneBeachStage->Win();
	//			}
	//			App->sceneBeachStage->ScoreRound(1);
	//			//App->sceneBeachStage->EndRound(1);
	//			
	//		}
	//		else {
	//			App->player2->score += 3;
	//			//arbitro = 1;
	//			if (App->sceneBeachStage->suddenDeath) {
	//				App->sceneBeachStage->Win();
	//			}
	//			App->sceneBeachStage->ScoreRound(1);
	//			//App->sceneBeachStage->EndRound(1);
	//		}
	//	}
	//	if (position.x >= 276) {
	//		if (position.y >= 94 && position.y <= 144) {
	//			App->player->score += 5;
	//			//arbitro = 2;
	//			if (App->sceneBeachStage->suddenDeath) {
	//				App->sceneBeachStage->Win();
	//			}
	//			
	//			App->sceneBeachStage->ScoreRound(2);
	//			//App->sceneBeachStage->EndRound(2);
	//		}
	//		else {
	//			App->player->score += 3;
	//			//arbitro = 2;
	//			if (App->sceneBeachStage->suddenDeath) {
	//				App->sceneBeachStage->Win();
	//			}
	//			App->sceneBeachStage->ScoreRound(2);
	//			//App->sceneBeachStage->EndRound(2);
	//		}
	//	}

	//	position.x = 150;
	//	position.y = 200;


	//}



//PARABOLA

	if (FloorTime == 120) {
		if (position.x < 150 && position.x >20) {
			App->player2->score += 2;
			if (App->sceneBeachStage->suddenDeath) {
				App->sceneBeachStage->Win();
			}
			/*App->sceneBeachStage->ScoreRound(1); - LO  CCOMENTO PARA Q NO DE ERRO*/
		}
		if (position.x > 150 && position.x <275) {
			App->player->score += 2;
			if (App->sceneBeachStage->suddenDeath) {
				App->sceneBeachStage->Win();
			}

			/*App->sceneBeachStage->ScoreRound(2);*/
		}
		position.x = 150;
		position.y = 200;
	}


	currentAnimation2->Update();
	

	collider->SetPos(position.x, position.y);



	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModuleFrisbee::PostUpdate()
{
	SDL_Rect rect2 = currentAnimation2->GetCurrentFrame();
	App->render->Blit(texture, position.x, position.y, &rect2);
	return Update_Status::UPDATE_CONTINUE;
}

void ModuleFrisbee::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && destroyed == false)
	{
		currentAnimation2 = &desaparece;
		estadoF = estadoFrisbee::STOP;
	/*	FloorTime = 0;*/

	}
}


void ModuleFrisbee :: movimientoFrisbee() {
	currentAnimation2 = &moving;

 	if (lanzamientoF == NORMAL) {
		

		if (direccionF == DARRIBA) {
			position.x += xspeed;
			position.y += yspeed;

		}
		else if (direccionF == DABAJO) {
			position.x += xspeed;
			position.y += yspeed;
		}
		else if (direccionF == HORIZONTAL) {
			position.x += xspeed;
		}

	}
	else if (lanzamientoF == PARABOLA) { //TO DO límite parabola
		if (direccionF == DARRIBA) {
			

		}
		else if (direccionF  == DABAJO) {

		}
		else if (HORIZONTAL) {

		}
	}
	else if(lanzamientoF == ARBITRO){
		position.x += xspeed;
		position.y += yspeed;
	}
	else if (lanzamientoF == SUPERSHOT) {
		//TODO
	}


	/*if (mov == 1 && position.x >= 19 && position.x <= 276) {

		if (pared == false && position.y >= 50) {

			position.x += xspeed;
			position.y -= yspeed;

		}
		else {
			pared = true;
		}

		if (pared == true && position.y < 170) {
			position.x += xspeed;
			position.y += yspeed;
		}
		else {
			pared = false;
		}

	}*/


	/*if (projectil == 2) {

		if (PosTemp < position.x || PosTemp > position.x) {
			position.x += xspeed;
		}
		else {
			currentAnimation2 = &stop;
			mov = 0;
			projectil = 0;
			posesion = 0;
			collider = App->collisions->AddCollider({ position.x, position.y, 16, 16 }, Collider::Type::FRISBEE, this);
		}


	}*/



	////MOV FRISBEE HACIA ABAJO
	//if (mov == 3 && position.x >= 19 && position.x <= 276) {

	//	if (pared == false && position.y < 170) {

	//		position.x += xspeed;
	//		position.y += yspeed;

	//	}
	//	else {
	//		pared = true;
	//	}

	//	if (pared == true && position.y >= 50) {
	//		position.x += xspeed;
	//		position.y -= yspeed;
	//	}
	//	else {
	//		pared = false;
	//	}
	//}


	//MOV FRISBE PROJECTIL HACIA DELANTE


	//if (position.x <= 19 || position.x >= 276) {
	//	mov = 0;

		////aqui tendremos que llamar la accion del arbitro que envia el disco al player 1

		//if (position.x <= 19) {
		//	if (position.y >= 94 && position.y <= 144) {
		//		App->player2->score += 5;
		//		//arbitro = 1;
		//		if (App->sceneBeachStage->suddenDeath) {
		//			App->sceneBeachStage->Win();
		//		}
		//		App->sceneBeachStage->ScoreRound(1);
		//		//App->sceneBeachStage->EndRound(1);

		//	}
		//	else {
		//		App->player2->score += 3;
		//		//arbitro = 1;
		//		if (App->sceneBeachStage->suddenDeath) {
		//			App->sceneBeachStage->Win();
		//		}
		//		App->sceneBeachStage->ScoreRound(1);
		//		//App->sceneBeachStage->EndRound(1);
		//	}
		//}
		//if (position.x >= 276) {
		//	if (position.y >= 94 && position.y <= 144) {
		//		App->player->score += 5;
		//		//arbitro = 2;
		//		if (App->sceneBeachStage->suddenDeath) {
		//			App->sceneBeachStage->Win();
		//		}

		//		App->sceneBeachStage->ScoreRound(2);
		//		//App->sceneBeachStage->EndRound(2);
		//	}
		//	else {
		//		App->player->score += 3;
		//		//arbitro = 2;
		//		if (App->sceneBeachStage->suddenDeath) {
		//			App->sceneBeachStage->Win();
		//		}
		//		App->sceneBeachStage->ScoreRound(2);
		//		//App->sceneBeachStage->EndRound(2);
		//	}
		//}

		//position.x = 150;
		//position.y = 200;


	//}

}

void ModuleFrisbee::limitesFrisbee() {

	if (lanzamientoF != ARBITRO) {
		if (position.x >= 19 && position.x <= 276) {

			if (position.y <= 50) {
				yspeed *= -1;
			}
			else if (position.y >= 170) {
				yspeed *= -1;
			}
		}

		else if (position.x < 19 || position.x >276) {

			//funsion score
			estadoF = estadoFrisbee::STOP;
			App->player->estadoP1 = ModulePlayer::estadoPlayer::STOP;
			App->player2->estadoP2 = ModulePlayer2::estadoPlayer2::STOP;
			App->sceneBeachStage->Score();

		}
	}





}


void ModuleFrisbee::timerF() {
	currentTimeF = SDL_GetTicks();

	if (currentTimeF - initialTimeF >= timeLimitF) {
		estadoTF = estadoTimerF::FIN;
	}
}



