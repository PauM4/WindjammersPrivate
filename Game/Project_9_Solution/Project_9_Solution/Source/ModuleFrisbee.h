#ifndef __MODULE_FRISBEE_H__
#define __MODULE_FRISBEE_H__ 

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

class ModuleFrisbee : public Module
{
public:
	// Constructor
	ModuleFrisbee(bool startEnabled);

	// Destructor
	~ModuleFrisbee();

	// Called when the module is activated
	// Loads the necessary textures for the player
	bool Start() override;

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	Update_Status Update() override;

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	Update_Status PostUpdate() override;

	// Collision callback, called when the player intersects with another collider
	void OnCollision(Collider* c1, Collider* c2) override;

public:
	// Position of the player in the map
	iPoint position;

	// The speed in which we move the player (pixels per frame)
	float speed;
	float xspeed;
	float yspeed;
	int mov;
	int arbitro;

	bool pared;
	bool inicio;
	int posesion; //FALSE: PLAYER 1, TRUE: PLAYER 2
	int PosTemp;
	int projectil;
	int FloorTime;
	int contadorBlock;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation2 = nullptr;

	// A set of animations
	Animation moving;
	Animation projectile;
	Animation desaparece;
	Animation stop;

	// The player's collider
	Collider* collider = nullptr;

	// A flag to detect when the player has been destroyed
	bool destroyed;

	

	enum estadoFrisbee
	{
		ARBITROF,
		STOP, //en el suelo
		MOVIMIENTO, //en el aire
		PRUEBA,
		WITHPLAYER, //con el player + cambiar posicion
		BLOCK,
		SUELO

	};
	int estadoF;

	enum tipoLanzamiento {
		NORMAL,
		PARABOLA,
		ARBITRO,
		SUPERSHOT, 
		BLOCKPLAYER1,
		BLOCKPLAYER2
	};
	int lanzamientoF;
	
	enum direccionFrisbeePlayer {
		DARRIBA,
		HORIZONTAL,
		DABAJO,
		MAX

	};
	int direccionF;

	void movimientoFrisbee();
	void limitesFrisbee();

	enum estadoTimerF {
		INICIO,
		EJECUTANDO,
		FIN,
	};
	int estadoTF;

	void timerF();
	int initialTimeF;
	int currentTimeF;
	int timeLimitF;

	bool blockSuperShot;

};

#endif //!__MODULE_FRISBEE_H__ 