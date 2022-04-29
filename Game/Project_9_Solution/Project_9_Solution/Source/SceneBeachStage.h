#ifndef __SCENE_BEACH_STAGE_H__
#define __SCENE_BEACH_STAGE_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class SceneBeachStage : public Module
{
public:
	//Constructor
	SceneBeachStage(bool startEnabled);

	//Destructor
	~SceneBeachStage();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	Update_Status Update() override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	Update_Status PostUpdate() override;

	// Disables the player
	bool CleanUp();

public:

	// The scene sprite sheet loaded into an SDL_Texture
	//SDL_Texture* beachTexture = nullptr;
	SDL_Texture* bgBeachTexture = nullptr;
	SDL_Texture* timerTexture = nullptr;
	SDL_Texture* uiSpriteTexture = nullptr;
	SDL_Texture* Winn;

	

	Animation timerAnim;
	Animation* currentTimerAnim = nullptr;
	Animation bgBeachAnim;
	Animation* currentBeachAnim = nullptr;

	// Sound effects indices
	uint round1FX;

	//uint winFx = 0;

	void EndRound(int arbitro);
	void ScoreRound(int arbitro);
	void Win();
	bool suddenDeath;
	
	int time;
	int initialTime;
	bool startTheGame;
	bool debugwinP1;
	bool debugwinP2;

	int winState;

	bool godMode;
	
	//Rectangulet esquerra
	SDL_Rect rectanguletL = { 0, 357, 29, 3 };
	//Rectangulet dreta
	SDL_Rect rectanguletR = { 0, 357, 29, 3 };

	bool a = 0;
	
};

#endif