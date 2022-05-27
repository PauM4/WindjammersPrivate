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
	projectile.speed = 0.2f;

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

	currentAnimation2 = &desaparece;
	LOG("Loading frisbee textures");

	

	texture = App->textures->Load("Assets/Sprites/Levels/Beach.png");


	position.x = 150;
	position.y = 200;
	projectil = 0;
	FloorTime = 0;


	destroyed = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 16, 16 }, Collider::Type::FRISBEE, this);
	estadoF = ARBITROF;
	lanzamientoF = ARBITRO;
	estadoTF = INICIO;
	blockSuperShot = true;
	contadorBlock = 0;
	return ret;
}

Update_Status ModuleFrisbee::Update()
{

	switch (estadoF) {

	case ARBITROF:
		currentAnimation2 = &desaparece;
		break;

	case STOP:
		currentAnimation2 = &stop;
		break;

	case MOVIMIENTO:
		movimientoFrisbee();
		limitesFrisbee();
		break;	

	case WITHPLAYER:
		currentAnimation2 = &desaparece;
		if (App->player->estadoP1 == ModulePlayer::estadoPlayer::WITHFRISBEE) {
			position.x = App->player->position.x + 28;
			position.y = App->player->position.y;
		}
		else if (App->player2->estadoP2 == ModulePlayer2::estadoPlayer2::WITHFRISBEE) {
			position.x = App->player2->position.x - 17;
			position.y = App->player2->position.y;
		}
		break;
	
	//case BLOCK:

	//	//animaciones del disco volando
	//	if (estadoTF == INICIO) {
	//		initialTimeF = SDL_GetTicks();
	//		
	//		if (contadorBlock == 0) {
	//			if (lanzamientoF == BLOCKPLAYER1) {
	//				position.x = App->player->position.x + 35;
	//			} 
	//			else if (lanzamientoF == BLOCKPLAYER2) {
	//				position.x = App->player2->position.x - 25;
	//			}

	//			timeLimitF = 2 * 1000;
	//			currentAnimation2 = &projectile;
	//			App->collisions->RemoveCollider(App->frisbee->collider);
	//			
	//		}
	//		else if (contadorBlock == 1) {
	//			timeLimitF = 1 * 1000;
	//			currentAnimation2 = &stop;
	//			
	//		}
	//		contadorBlock++;
	//		estadoTF = EJECUTANDO;
	//		
	//	
	//	}
	//	else if (estadoTF == EJECUTANDO) {
	//		timerF();
	//	}
	//	else if (estadoTF == FIN) {

	//		if (contadorBlock == 1){
	//			collider = App->collisions->AddCollider({ position.x, position.y, 16,16 }, Collider::Type::FRISBEE, this);
	//			blockSuperShot = true;
	//			estadoTF = INICIO;
	//		}

	//		if (contadorBlock == 2) {
	//			contadorBlock = 0;
	//			estadoTF = INICIO;
	//			currentAnimation2 = &desaparece;
	//			App->player->estadoP1 = ModulePlayer::estadoPlayer::STOP;
	//			App->player2->estadoP2 = ModulePlayer2::estadoPlayer2::STOP;
	//			App->sceneBeachStage->Score();
	//			
	//		}
	//		
	//	}
	//	break;

	case BLOCK:

		//animaciones del disco volando
		if (estadoTF == INICIO) {
			initialTimeF = SDL_GetTicks();

			if (lanzamientoF == BLOCKPLAYER1) {
				position.x = App->player->position.x + 35;
			}
			else if (lanzamientoF == BLOCKPLAYER2) {
				position.x = App->player2->position.x - 25;
			}

			timeLimitF = 2 * 1000;
			currentAnimation2 = &projectile;
			App->collisions->RemoveCollider(App->frisbee->collider);
			estadoTF = EJECUTANDO;

		}
		else if (estadoTF == EJECUTANDO) {
			timerF();
		}
		else if (estadoTF == FIN) {
			estadoTF = INICIO;
			currentAnimation2 = &stop;
				collider = App->collisions->AddCollider({ position.x, position.y, 16,16 }, Collider::Type::FRISBEE, this);
				blockSuperShot = true;
				
				estadoF = SUELO;
		}
		break;

	case SUELO:
		currentAnimation2 = &stop;
		if (estadoTF == INICIO) {
			initialTimeF = SDL_GetTicks();
			timeLimitF = 2 * 1000;
			estadoTF = EJECUTANDO;

		}
		else if (estadoTF == EJECUTANDO) {
			timerF();
		}
		else if (estadoTF == FIN) {
			estadoTF = INICIO;
			estadoF = ARBITROF;
			App->sceneBeachStage->Score();
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
		//currentAnimation2 = &desaparece;
		//estadoF = estadoFrisbee::STOP;
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
	else if (lanzamientoF == PARABOLA) { //solo haremos que la parabola se pueda lanzar horizontalmente
	
		currentAnimation2 = &projectile;
		if (35 < position.x && 250 > position.x) {
			position.x += xspeed;
		}
		else {
			collider = App->collisions->AddCollider({ position.x, position.y, 16, 16 }, Collider::Type::FRISBEE, this); 
			estadoF = SUELO;

		}
	}
	else if (lanzamientoF == ARBITRO) {
		position.x += xspeed;
		position.y += yspeed;
	}

	else if (lanzamientoF == SUPERSHOT) {
		
		if (tipoSupershot == MITA_SUPERSHOT) {

		} else if (tipoSupershot == YOO_SUPERSHOT) {

		}
		else if (tipoSupershot == WESSEL_SUPERSHOT) {
			if (position.x < limiteWesselSupershot ) {
				position.x += xspeed;
			} else if (position.x >= limiteWesselSupershot){
				position.y += yspeed;
			}
		}
	}



	
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

}

void ModuleFrisbee::limitesFrisbee() {

	if (position.x < 19 || position.x >276) {

		//funsion score
		estadoF = ARBITROF;
		App->player->estadoP1 = ModulePlayer::estadoPlayer::STOP;
		App->player2->estadoP2 = ModulePlayer2::estadoPlayer2::STOP;
		App->sceneBeachStage->Score();
		tipoSupershot = NONE;

	}

	if (tipoSupershot == WESSEL_SUPERSHOT) {
		
		if (position.y <= 50) {
			position.x += xspeed;
			yspeed = 0;
		}
		else if (position.y >= 170) {
			position.x += xspeed;
			yspeed = 0;
		}

	}
	else if (lanzamientoF != ARBITRO && lanzamientoF != SUPERSHOT) {
		if (position.x >= 19 && position.x <= 276) {

			if (position.y <= 50) {
				yspeed *= -1;
			}
			else if (position.y >= 170) {
				yspeed *= -1;
			}
		}

		
	}





}


void ModuleFrisbee::timerF() {
	currentTimeF = SDL_GetTicks();

	if (currentTimeF - initialTimeF >= timeLimitF) {
		estadoTF = estadoTimerF::FIN;
	}
}



void ModuleFrisbee::vel_parabola(int pos_Player, int pos_final_frisbee) {

	if (pos_final_frisbee == 260) {
		projectile.speed = (pos_final_frisbee - pos_Player) / (pos_final_frisbee - pos_Player / xspeed) * 0.2f;
			
	}
	else if (35) { //este 35 va en el player2
		projectile.speed = (pos_final_frisbee - pos_Player) / (pos_final_frisbee - pos_Player / xspeed);
	}

}

void ModuleFrisbee::anguloSupershot() {




}