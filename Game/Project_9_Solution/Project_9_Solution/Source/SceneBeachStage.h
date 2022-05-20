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

	//The scene sprite sheet loaded into an SDL_Texture
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

	void Arbitro(int arbitro);
	void Round();
	void Score();
	
	
	void Win();
	bool suddenDeath;

	
	int time;
	int initialTime;
	bool startTheGame;
	bool debugwinP1;
	bool debugwinP2;
	

	int winState;

	bool godMode;

	//UPLEFT == 3 punts adalt esquerra
	//MIDRIGHT == 5 punts mig dreta etc...
	enum golScoreUI
	{
		UPLEFT,
		MIDLEFT,
		DOWNLEFT,
		UPRIGHT,
		MIDRIGHT,
		DOWNRIGHT,
		CLEAR
	};
	int estadoGolScore;

	enum estadoScene
	{
		INICIO, //Inicio
		INICIORONDA, //Animaciones de inicio de ronda
		RONDA, //durante el juego
		FINALRONDA, //animaciones/texturas fin de ronda
		FINAL //fin de partida

	};
	int estadoS;

	enum estadoTimerS {
		INICIOT,
		EJECUTANDO,
		FIN,
	};
	int estadoTS;
	int initialTimeS;
	int currentTimeS;
	int timeLimitS;

	//TIMER PER GOLS
	enum estadoTimerGol {
		INICIOGOL,
		EJECUTANDOGOL,
		FINGOL,
	};
	int estadoTGol;
	int initialTimeGol;
	int currentTimeGol;
	int timeLimitGol;


	int arbitroFinalRonda;

	void TimerS();
	void TimerGol();
	
	//Rectangulet esquerra
	SDL_Rect rectanguletL = { 0, 357, 29, 3 };
	//Rectangulet dreta
	SDL_Rect rectanguletR = { 0, 357, 29, 3 };

	SDL_Rect rectTimer88;
	SDL_Rect rectNormes;
	SDL_Rect tresPuntsL;
	SDL_Rect cincPuntsL;
	SDL_Rect tresPuntsR;
	SDL_Rect cincPuntsR;
	SDL_Rect set1Rect;

	//Font Debug vars
	int debugFont;
	char debugText[10] = { "\0" };
	char debugText2[10] = { "\0" };
	bool isDebugAppear;
	
};

#endif