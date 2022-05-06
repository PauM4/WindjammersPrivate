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
	speed = 1;
	arbitro = 1;
	pared = false;
	inicio = false;
	posesion = false;
	destroyed = false;

	currentAnimation2 = &moving;
	LOG("Loading frisbee textures");

	bool ret = true;

	texture = App->textures->Load("Assets/Sprites/Levels/Beach.png");


	position.x = 150;
	position.y = 200;
	projectil = 0;
	FloorTime = 0;


	destroyed = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 16, 16 }, Collider::Type::FRISBEE, this);

	return ret;
}

Update_Status ModuleFrisbee::Update()
{
	//while (position.x != App->player->position.x && position.y != App->player->position.y) {
		/*uint delay = 1500;
		SDL_Delay(delay);*/

	//TIMER SUELO FRISBBE
	if (FloorTime < 120 && currentAnimation2== &stop)
	{
		FloorTime++;
		
	}
	

	if (arbitro == 1) {
		App->player->position.x = 20;
		App->player->position.y = 100;
		App->player2->position.x = 259;
		App->player2->position.y = 100;
		FloorTime = 0;
		App->player->currentAnimation = &App->player->idleRAnim;
		App->player2->currentAnimation = &App->player2->idleLAnim;
		if (position.x != App->player->position.x || position.y != App->player->position.y) {
			position.x -= 3;
			position.y -= 2;
		}
	}
	if (arbitro == 2) {
		App->player->position.x = 20;
		App->player->position.y = 100;
		App->player2->position.x = 259;
		App->player2->position.y = 100;
		FloorTime = 0;
		App->player->currentAnimation = &App->player->idleRAnim;
		App->player2->currentAnimation = &App->player2->idleLAnim;
		if (position.x != App->player2->position.x || position.y != App->player2->position.y) {
			position.x += 3;
			position.y -= 2;
		}
	}

	////MOV FRISBEE HACIA ARRIBA
	//if (mov == 1 && position.x >= 19 && position.x <= 276) {

	//	if (pared == false && position.y >= 50) {

	//		position.x += xspeed;
	//		position.y -= yspeed;

	//	}
	//	else {
	//		pared = true;
	//	}

	//	if (pared == true && position.y < 170) {
	//		position.x += xspeed;
	//		position.y += yspeed;
	//	}
	//	else {
	//		pared = false;
	//	}

	//}

	////MOV FRISBEE HORIZONTAL PROYECTIL - PARA PLAYER2
	//if (mov == 2 && position.x >= 19 && position.x <= 276 && projectil == 1){

	//		position.x += xspeed;
	//	
	//}

	//if (projectil == 2) {

	//	if (PosTemp < position.x || PosTemp > position.x) {
	//		position.x += xspeed;
	//	}
	//	else {
	//		currentAnimation2 = &stop;
	//		mov = 0;
	//		projectil = 0;
	//		posesion = 0;
	//		collider = App->collisions->AddCollider({ position.x, position.y, 16, 16 }, Collider::Type::FRISBEE, this);
	//	}

	//
	//}
	//


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
	//

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

	if (FloorTime == 120) {
		if (position.x < 150 && position.x >20) {
			App->player2->score += 2;
			if (App->sceneBeachStage->suddenDeath) {
				App->sceneBeachStage->Win();
			}
			App->sceneBeachStage->ScoreRound(1);
		}
		if (position.x > 150 && position.x <275) {
			App->player->score += 2;
			if (App->sceneBeachStage->suddenDeath) {
				App->sceneBeachStage->Win();
			}

			App->sceneBeachStage->ScoreRound(2);
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
		arbitro = 0;
		currentAnimation2 = &desaparece;
		FloorTime = 0;

	}
}


void ModuleFrisbee :: movimientoFrisbee() {

	

	if (tipoLanzamiento::NORMAL) {

		if (direccionFrisbeePlayer::DARRIBA) {
			position.x += xspeed;
			position.y += yspeed;

		}
		else if (direccionFrisbeePlayer:: DABAJO) {
			position.x += xspeed;
			position.y += yspeed;
		}
		else if (direccionFrisbeePlayer::HORIZONTAL) {
			position.x += xspeed;
		}

	}
	else if (tipoLanzamiento::PARABOLA) { //TO DO l�mite parabola
		if (direccionFrisbeePlayer::DARRIBA) {
			

		}
		else if (direccionFrisbeePlayer::DABAJO) {

		}
		else if (direccionFrisbeePlayer::HORIZONTAL) {

		}
	}
	else if (tipoLanzamiento::SUPERSHOT) {
		//TODO
	}


	if (mov == 1 && position.x >= 19 && position.x <= 276) {

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

	}


	if (projectil == 2) {

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


	}



	//MOV FRISBEE HACIA ABAJO
	if (mov == 3 && position.x >= 19 && position.x <= 276) {

		if (pared == false && position.y < 170) {

			position.x += xspeed;
			position.y += yspeed;

		}
		else {
			pared = true;
		}

		if (pared == true && position.y >= 50) {
			position.x += xspeed;
			position.y -= yspeed;
		}
		else {
			pared = false;
		}
	}


	//MOV FRISBE PROJECTIL HACIA DELANTE


	if (position.x <= 19 || position.x >= 276) {
		mov = 0;

		//aqui tendremos que llamar la accion del arbitro que envia el disco al player 1

		if (position.x <= 19) {
			if (position.y >= 94 && position.y <= 144) {
				App->player2->score += 5;
				//arbitro = 1;
				if (App->sceneBeachStage->suddenDeath) {
					App->sceneBeachStage->Win();
				}
				App->sceneBeachStage->ScoreRound(1);
				//App->sceneBeachStage->EndRound(1);

			}
			else {
				App->player2->score += 3;
				//arbitro = 1;
				if (App->sceneBeachStage->suddenDeath) {
					App->sceneBeachStage->Win();
				}
				App->sceneBeachStage->ScoreRound(1);
				//App->sceneBeachStage->EndRound(1);
			}
		}
		if (position.x >= 276) {
			if (position.y >= 94 && position.y <= 144) {
				App->player->score += 5;
				//arbitro = 2;
				if (App->sceneBeachStage->suddenDeath) {
					App->sceneBeachStage->Win();
				}

				App->sceneBeachStage->ScoreRound(2);
				//App->sceneBeachStage->EndRound(2);
			}
			else {
				App->player->score += 3;
				//arbitro = 2;
				if (App->sceneBeachStage->suddenDeath) {
					App->sceneBeachStage->Win();
				}
				App->sceneBeachStage->ScoreRound(2);
				//App->sceneBeachStage->EndRound(2);
			}
		}

		position.x = 150;
		position.y = 200;


	}

}