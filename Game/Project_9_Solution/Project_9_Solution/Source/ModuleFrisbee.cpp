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
#include <math.h>

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

	collider = App->collisions->AddCollider({ (int)position.x, (int)position.y, 16, 16 }, Collider::Type::FRISBEE, this);
	estadoF = ARBITROF;
	lanzamientoF = ARBITRO;
	estadoTF = INICIO;
	blockSuperShot = true;
	contadorBlock = 0;
	contador_Angulo_SuperShot = true;
	contadorYooSuperShot = 0;
	yooDirec = true;
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
			collider = App->collisions->AddCollider({ (int)position.x, (int)position.y, 16,16 }, Collider::Type::FRISBEE, this);
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
		
		//Flash particle left or right depending on the player
		if (position.x < 150)
		{
			App->particles->AddParticle(0, 0, App->particles->leftGoalFlashParticle, App->player->position.x + 29, App->player->position.y, Collider::NONE, 0);
		}
		else if (position.x > 150)
		{
			App->particles->AddParticle(0, 0, App->particles->rightGoalFlashParticle, App->player2->position.x - 5, App->player2->position.y, Collider::NONE, 0);
		}

		estadoTF = INICIO;

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
			collider = App->collisions->AddCollider({ (int)position.x, (int)position.y, 16, 16 }, Collider::Type::FRISBEE, this);
			estadoF = SUELO;
			
		}
	}
	else if (lanzamientoF == ARBITRO) {
		position.x += xspeed;
		position.y += yspeed;
	}

	else if (lanzamientoF == SUPERSHOT) {
		
		if (tipoSupershot == MITA_SUPERSHOT) {

			anguloSupershot();
			position.x += xspeed*0.8f;
			position.y += yspeed * cos(angulo);
			App->particles->AddParticle(0, 0, App->particles->mitaSuperShotParticle, position.x, position.y - 48, Collider::NONE, 5);


		} else if (tipoSupershot == YOO_SUPERSHOT) {

			if (contadorYooSuperShot <= 8 && contador_Angulo_SuperShot) {
				position.x += xspeed;
				contadorYooSuperShot++;

				if (contadorYooSuperShot == 8) {
					contador_Angulo_SuperShot = false;
					if (yooDirec) {
						contadorYooSuperShot = 0;
					}
					else if (!yooDirec) {
						contadorYooSuperShot = 4;
					}
					
					
				}
			}
			else if (!contador_Angulo_SuperShot) {

				if (yooDirec) {
					position.y -= yspeed;
					contadorYooSuperShot++;

					if (contadorYooSuperShot >= 4 || position.y >= 170) {
						contador_Angulo_SuperShot = true;
						contadorYooSuperShot = 0;
						yooDirec = false;
					}

				}
				else if (!yooDirec) {
					position.y += yspeed;
					contadorYooSuperShot--;

					if (contadorYooSuperShot <= 0 || position.y <= 45) {
						contador_Angulo_SuperShot = true;
						yooDirec = true;
					}
				}
			}

		}
		else if (tipoSupershot == WESSEL_SUPERSHOT) {
			if (position.x < limiteWesselSupershot ) {
				position.x += xspeed;
			} else if (position.x >= limiteWesselSupershot){
				position.y += yspeed;
			}
		}
	}



	
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
			//UP
			if (position.y <= 48) {
				//Right
				if (xspeed > 0)
				{
					App->particles->AddParticle(0, 0, App->particles->xocDownright, position.x + 15, position.y, Collider::NONE, 0);
				}
				//Left	
				else if (xspeed < 0)
				{
					App->particles->AddParticle(0, 0, App->particles->xocDownleft, position.x - 25, position.y, Collider::NONE, 0);
				}
				yspeed *= -1;
			}
			//DOWN
			else if (position.y >= 170) {
				//Right
				if (xspeed > 0 && position.y < 173)
				{
					App->particles->AddParticle(0, 0, App->particles->xocUpright, position.x + 15, position.y, Collider::NONE, 0);
				}
				//Left	
				else if (xspeed < 0 && position.y < 173)
				{
					App->particles->AddParticle(0, 0, App->particles->xocUpleft, position.x - 25, position.y, Collider::NONE, 0);
				}
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

	if (pos_final_frisbee >= 260) {
		projectile.speed = (pos_final_frisbee - pos_Player) / (pos_final_frisbee - pos_Player / xspeed) * 0.2f;
			
	}
	else if (pos_final_frisbee <= 35) {
		projectile.speed = (pos_Player - pos_final_frisbee) / ( pos_Player - pos_final_frisbee / xspeed*-1) * 0.2f;
	}

}

void ModuleFrisbee::anguloSupershot() {

	if (angulo <= 180 && contador_Angulo_SuperShot) { 
		angulo -= 0.15;
		if (angulo == 0) {
			contador_Angulo_SuperShot = false;
		}
	}
	else if (angulo >= 0 && !contador_Angulo_SuperShot) {
		angulo += 0.15;
		if (angulo == 180) {
			contador_Angulo_SuperShot = true;
		}
	}



}