#ifndef __MODULE_IN_GAME_UI_H__
#define __MODULE_IN_GAME_UI_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class ModuleInGameUI : public Module
{
public:
	//Constructor
	ModuleInGameUI(bool startEnabled);

	//Destructor
	~ModuleInGameUI();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	Update_Status Update() override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	Update_Status PostUpdate() override;

	// Disables the ui
	bool CleanUp();

public:

	SDL_Texture* uiSpriteTexture = nullptr;
	SDL_Texture* timerTexture = nullptr;

	Animation* currentTimerAnim = nullptr;
	Animation timerAnim;

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

	void TimerS();

	//Rectangulet esquerra
	SDL_Rect rectanguletL;
	//Rectangulet dreta
	SDL_Rect rectanguletR;

	SDL_Rect rectNormes;
	SDL_Rect tresPuntsL;
	SDL_Rect cincPuntsL;
	SDL_Rect tresPuntsR;
	SDL_Rect cincPuntsR;
	SDL_Rect set1Rect;
	SDL_Rect set2Rect;
	SDL_Rect setFinalRect;
	SDL_Rect suddenRect;

	SDL_Rect rectTimer;

	SDL_Rect winUIRight;
	SDL_Rect LoseUIRight;
	SDL_Rect winUILeft;
	SDL_Rect LoseUILeft;

	SDL_Rect rectTimer88;

	SDL_Rect p1Rect;
	SDL_Rect p2Rect;
	SDL_Rect japanFlagRect;
	SDL_Rect koreanFlagRect;
	SDL_Rect germanyFlagRect;

	SDL_Rect rounds;

	SDL_Texture* bothCharactersTexture;

	//Font Debug vars
	int debugFont;
	char debugText[10] = { "\0" };
	bool isDebugAppear;
};

#endif