#include "Application.h"

#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePlayer2.h"
#include "SceneIntro.h"
#include "SceneNeogeo.h"
#include "SceneDataeast.h"
#include "SceneTitle.h"
#include "SceneCharacterSelect.h"
#include "SceneStageSelect.h"
#include "SceneCharacterPresent.h"
#include "SceneBeachStage.h"
#include "SceneLawnStage.h"
#include "ModuleInGameUI.h"

#include "ModuleParticles.h"
#include "ModuleCollisions.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include "ModuleRender.h"
#include "ModuleFrisbee.h"

Application::Application()
{
	// The order in which the modules are added is very important.
	// It will define the order in which Pre/Update/Post will be called
	// Render should always be last, as our last action should be updating the screen

	modules[0] = window = new ModuleWindow(true);
	modules[1] = input = new ModuleInput(true);
	modules[2] = textures = new ModuleTextures(true);
	modules[3] = audio = new ModuleAudio(true);

	modules[4] = sceneIntro = new SceneIntro(false);
	modules[5] = sceneNeogeo = new SceneNeogeo(false);
	modules[6] = sceneDataeast = new SceneDataeast(false);
	modules[7] = sceneTitle = new SceneTitle(false);
	modules[8] = sceneCharacterSelect = new SceneCharacterSelect(true);
	modules[9] = sceneStageSelect = new SceneStageSelect(false);
	modules[10] = sceneCharacterPresent = new SceneCharacterPresent(false);
	modules[11] = sceneBeachStage = new SceneBeachStage(false);
	modules[12] = sceneLawnStage = new SceneLawnStage(false);

	modules[13] = player = new ModulePlayer(false);	//Player starts disabled
	modules[14] = player2 = new ModulePlayer2(false);	//Player starts disabled
	modules[15] = frisbee = new ModuleFrisbee(false);
	modules[16] = particles = new ModuleParticles(true);

	// Aquest modul ha d'estar sempre per sobre dels players i els mapes
	//modules[15] = ingameUI = new ModuleIngameUI(true);
	modules[17] = ingameUI = new ModuleInGameUI(false);

	modules[18] = collisions = new ModuleCollisions(true);
	modules[19] = fade = new ModuleFadeToBlack(true);
	modules[20] = fonts = new ModuleFonts(true);
	modules[21] = render = new ModuleRender(true);
}

Application::~Application()
{
	for (int i = 0; i < NUM_MODULES; ++i)
	{
		//Important: when deleting a pointer, set it to nullptr afterwards
		//It allows us for null check in other parts of the code
		delete modules[i];
		modules[i] = nullptr;
	}
}

bool Application::Init()
{
	bool ret = true;

	// All modules (active and disabled) will be initialized
	for (int i = 0; i < NUM_MODULES && ret; ++i)
		ret = modules[i]->Init();

	// Only active modules will be 'started'
	for (int i = 0; i < NUM_MODULES && ret; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->Start() : true;

	return ret;
}

Update_Status Application::Update()
{
	Update_Status ret = Update_Status::UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == Update_Status::UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->PreUpdate() : Update_Status::UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == Update_Status::UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->Update() : Update_Status::UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == Update_Status::UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->PostUpdate() : Update_Status::UPDATE_CONTINUE;

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (int i = NUM_MODULES - 1; i >= 0 && ret; --i)
		ret = modules[i]->IsEnabled() ? modules[i]->CleanUp() : true;

	return ret;
}