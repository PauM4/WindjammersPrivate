#include "ModuleParticles.h"

#include "Application.h"

#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles(bool startEnabled) : Module(startEnabled)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;
}

ModuleParticles::~ModuleParticles()
{

}

bool ModuleParticles::Start()
{
	LOG("Loading particles");
	texture = App->textures->Load("Assets/Sprites/particlesAndEffects.png");

	//particlesAndEffectsTxt = App->textures->Load("Assets/Sprites/particlesAndEffects.png");

	//Left Goal Flash
	leftGoalFlashParticle.anim.PushBack({ 100, 15, 17, 30 });
	leftGoalFlashParticle.anim.PushBack({ 121, 12, 22, 38 });
	leftGoalFlashParticle.anim.PushBack({ 147, 9, 27, 43 });
	leftGoalFlashParticle.anim.PushBack({ 178, 9, 31, 47 });
	leftGoalFlashParticle.anim.PushBack({ 213, 6, 32, 48 });
	leftGoalFlashParticle.anim.loop = false;
	leftGoalFlashParticle.lifetime = 20;
	leftGoalFlashParticle.anim.speed = 0.2f;

	//Right Goal Flash
	rightGoalFlashParticle.anim.PushBack({ 384, 15, 17, 30 });
	rightGoalFlashParticle.anim.PushBack({ 358, 12, 22, 38 });
	rightGoalFlashParticle.anim.PushBack({ 327, 9, 27, 43 });
	rightGoalFlashParticle.anim.PushBack({ 292, 8, 31, 47 });
	rightGoalFlashParticle.anim.PushBack({ 256, 6, 32, 48 });
	rightGoalFlashParticle.anim.loop = false;
	rightGoalFlashParticle.lifetime = 20;
	rightGoalFlashParticle.anim.speed = 0.2f;

	//Dust particle
	dustParticle.anim.PushBack({ 5, 108, 14, 14 });
	dustParticle.anim.PushBack({ 26, 108, 14, 14 });
	dustParticle.anim.PushBack({ 47, 109, 14, 14 });
	dustParticle.anim.PushBack({ 70, 108, 14, 14 });
	dustParticle.anim.PushBack({ 93, 108, 14, 14 });
	dustParticle.anim.PushBack({ 112, 108, 14, 14 });
	dustParticle.anim.PushBack({ 129, 108, 14, 14 });
	dustParticle.anim.PushBack({ 142, 108, 14, 14 });
	dustParticle.anim.loop = false;
	//dustParticle.anim.pingpong = true;
	dustParticle.lifetime = 30;
	dustParticle.anim.speed = 0.4f;

	//Frisbee Player 1 Basic Mov
	frisbee.anim.PushBack({ 117, 48, 16, 16 });
	frisbee.anim.PushBack({ 149, 48, 16, 16 });
	frisbee.anim.PushBack({ 181, 48, 16, 16 });
	frisbee.anim.PushBack({ 213, 48, 16, 16 });
	frisbee.lifetime = 180;
	frisbee.anim.speed = 0.2f;

	//Frisbee Player 1 Projectile motion animation
	frisbeeProjectile.anim.PushBack({ 35, 8, 17, 29 });
	frisbeeProjectile.anim.PushBack({ 53, 7, 25, 31 });
	frisbeeProjectile.anim.PushBack({ 79, 6, 36, 34 });
	frisbeeProjectile.anim.PushBack({ 116, 5, 28, 36 });
	frisbeeProjectile.anim.PushBack({ 145, 4, 21, 38 });
	frisbeeProjectile.anim.PushBack({ 167, 3, 3, 39 });
	frisbeeProjectile.anim.PushBack({ 171, 2, 23, 41 });
	frisbeeProjectile.anim.PushBack({ 195, 2, 33, 42 });
	frisbeeProjectile.anim.PushBack({ 229, 0, 43, 44 });
	frisbeeProjectile.anim.loop = false;
	frisbeeProjectile.lifetime = 180;
	frisbeeProjectile.anim.speed = 0.1f;

	//Particle when frisbee colliding with wall
	xocUpleft.anim.PushBack({ 4, 215, 32, 19 });
	xocUpleft.anim.PushBack({ 37, 215, 32, 22});
	xocUpleft.anim.PushBack({ 71, 215, 31, 23 });
	xocUpleft.anim.PushBack({ 104, 215, 29, 24 });
	xocUpleft.anim.PushBack({ 135, 215, 28, 25 });
	xocUpleft.anim.loop = false;
	xocUpleft.lifetime = 13;
	xocUpleft.anim.speed = 0.25f;

	xocDownleft.anim.PushBack({ 4, 254, 32, 19 });
	xocDownleft.anim.PushBack({ 37, 254, 32, 22 });
	xocDownleft.anim.PushBack({ 71, 254, 31, 23 });
	xocDownleft.anim.PushBack({ 104, 254, 29, 24 });
	xocDownleft.anim.PushBack({ 135, 254, 28, 25 });
	xocDownleft.anim.loop = false;
	xocDownleft.lifetime = 13;
	xocDownleft.anim.speed = 0.25f;

	xocUpright.anim.PushBack({ 135, 283, 28, 25 });
	xocUpright.anim.PushBack({ 104, 283, 29, 24 });
	xocUpright.anim.PushBack({ 71, 283, 31, 23 });
	xocUpright.anim.PushBack({ 37, 283, 32, 22 });
	xocUpright.anim.PushBack({ 4, 283, 32, 19 });
	xocUpright.anim.loop = false;
	xocUpright.lifetime = 13;
	xocUpright.anim.speed = 0.25f;

	xocDownright.anim.PushBack({ 135, 315, 28, 25 });
	xocDownright.anim.PushBack({ 104, 315, 29, 24 });
	xocDownright.anim.PushBack({ 71, 315, 31, 23 });
	xocDownright.anim.PushBack({ 37, 315, 32, 22 });
	xocDownright.anim.PushBack({ 4, 315, 32, 19 });
	xocDownright.anim.loop = false;
	xocDownright.lifetime = 13;
	xocDownright.anim.speed = 0.25f;


	return true;
}

Update_Status ModuleParticles::PreUpdate()
{
	// Remove all particles scheduled for deletion
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr && particles[i]->pendingToDelete)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");

	// Delete all remaining active particles on application exit 
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return true;
}

void ModuleParticles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (particles[i] != nullptr && particles[i]->collider == c1)
		{
			particles[i]->pendingToDelete = true;
			particles[i]->collider->pendingToDelete = true;
			break;
		}
	}
}

Update_Status ModuleParticles::Update()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle == nullptr)	continue;

		// Call particle Update. If it has reached its lifetime, destroy it
		if (particle->Update() == false)
		{
			particles[i]->SetToDelete();
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModuleParticles::PostUpdate()
{
	//Iterating all particle array and drawing any active particles
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle != nullptr && particle->isAlive)
		{
			App->render->Blit(texture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

Particle* ModuleParticles::AddParticle(int xspeed, int yspeed, const Particle& particle, int x, int y, Collider::Type colliderType, uint delay)
{
	Particle* newParticle = nullptr;

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		//Finding an empty slot for a new particle
		if (particles[i] == nullptr)
		{
			newParticle = new Particle(particle);
			newParticle->frameCount = -(int)delay;			// We start the frameCount as the negative delay
			newParticle->position.x = x;						// so when frameCount reaches 0 the particle will be activated
			newParticle->position.y = y;
			newParticle->speed.x = xspeed;
			newParticle->speed.y = yspeed;

			//Adding the particle's collider
			if (colliderType != Collider::Type::NONE)
				newParticle->collider = App->collisions->AddCollider(newParticle->anim.GetCurrentFrame(), colliderType, this);

			particles[i] = newParticle;
			break;
		}
	}

	return newParticle;
}