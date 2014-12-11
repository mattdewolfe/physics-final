#include <iostream>
#include <GL\include\glut.h>
#include "../SDL2/include/SDL.h"
#include "../SDL2/include/SDL_opengl.h"
#include "PhysicsManager.h"
#include "Entity.h"
#include "Component.h"
#include "PhysicsComponent.h"
#include "GraphicsComponent.h"
#include "MovingPlatformComponent.h"
#include "InputComponent.h"
#include "VisualText.h"
#include "BallComponent.h"

// Game Loop variables
bool game_running = true;
bool playerWon = false;

std::vector<Entity> entities;
PhysicsManager* physicsManager = nullptr;
InputComponent* inputComponent;
VisualText* text;
Entity* player;
// Screen size
int screenWidth = 640;
int screenHeight = 480;

bool isPaused = false;

// Function prototypes
void InitGL();
void HandleEvents(SDL_Event* curEvent);
void Update(float dt);
void Render(SDL_Window* window);
void PlayerWon();
void Reset();

float FRAMERATE = 1.0f/60.0f;

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window;
	SDL_Event curEvent;
	SDL_GLContext glContext;

	// Set our base SDL attributes.
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create our SDL window.
	window = SDL_CreateWindow(	"Physics Playground",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screenWidth, screenHeight,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (!window)
	{
		std::cout << "Couldn't create window: " << SDL_GetError() << std::endl;
		return 0;
	}

	// Create our openGL context with our window.
	glContext = SDL_GL_CreateContext(window);

	if (!glContext)
	{
		std::cout << "Couldn't create context: " << SDL_GetError() << std::endl;
		return 0;
	}

	// Init openGL
	InitGL();
	
	text = new VisualText(2, 0.0f, 0.0, 0.0);

	physicsManager = new PhysicsManager();
	physicsManager->Init();

	// Define the ball at the top of the screen
	player = new Entity();
	player->SetPosition(0, 15, 0);
	
	inputComponent = new InputComponent();
	inputComponent->SetOwner(player);
	player->AddComponent(inputComponent);

	BallComponent* bc = new BallComponent();
	bc->SetOwner(player);
	bc->Init(PhysicsComponent::RBST_Sphere, 1.0f);
	player->AddComponent(bc);

	GraphicsComponent* gc = new GraphicsComponent(GraphicsComponent::GST_Sphere);
	gc->SetOwner(player);
	gc->Init(0, 0);
	gc->SetColor(COLOR_Red);
	player->AddComponent(gc);

	entities.push_back(*player);

	// Define the platform to move back and forth
	Entity ent2;
	ent2.SetPosition(-10, -15, 0);

	PhysicsComponent* pc = new PhysicsComponent();
	pc->SetOwner(&ent2);
	pc->Init(PhysicsComponent::RBST_Box, 1.0f);
	ent2.AddComponent(pc);
	
	gc = new GraphicsComponent(GraphicsComponent::GST_Cube);
	gc->SetOwner(&ent2);
	gc->Init(2, 1);
	gc->SetColor(COLOR_Red);
	ent2.AddComponent(gc);

	MovingPlatformComponent* mp = new MovingPlatformComponent();
	mp->Init(5, 1);
	mp->SetOwner(&ent2);
	ent2.AddComponent(mp);

	entities.push_back(ent2);

	// Define to obstacles to stand in the players way
	// Define a platform to collide with
	Entity ent3;
	ent3.SetPosition(8, 0, 0);

	pc = new PhysicsComponent();
	pc->SetOwner(&ent3);
	pc->Init(PhysicsComponent::RBST_Box, 0.0f);
	ent3.AddComponent(pc);
	
	gc = new GraphicsComponent(GraphicsComponent::GST_Cube);
	gc->SetOwner(&ent3);
	gc->Init(2, 1);
	gc->SetColor(COLOR_Yellow);
	ent3.AddComponent(gc);

	entities.push_back(ent3);

	Entity ent4;
	ent4.SetPosition(-8, 0, 0);

	pc = new PhysicsComponent();
	pc->SetOwner(&ent4);
	pc->Init(PhysicsComponent::RBST_Box, 0.0f);
	ent4.AddComponent(pc);
	
	gc = new GraphicsComponent(GraphicsComponent::GST_Cube);
	gc->SetOwner(&ent4);
	gc->Init(2, 1);
	gc->SetColor(COLOR_Yellow);
	ent4.AddComponent(gc);

	entities.push_back(ent4);

	Entity ent5;
	ent5.SetPosition(-4, -5, 0);

	pc = new PhysicsComponent();
	pc->SetOwner(&ent5);
	pc->Init(PhysicsComponent::RBST_Box, 0.0f);
	ent5.AddComponent(pc);
	
	gc = new GraphicsComponent(GraphicsComponent::GST_Cube);
	gc->SetOwner(&ent5);
	gc->Init(2, 1);
	gc->SetColor(COLOR_Yellow);
	ent5.AddComponent(gc);

	entities.push_back(ent5);

	Entity ent6;
	ent6.SetPosition(4, -5, 0);

	pc = new PhysicsComponent();
	pc->SetOwner(&ent6);
	pc->Init(PhysicsComponent::RBST_Box, 0.0f);
	ent6.AddComponent(pc);
	
	gc = new GraphicsComponent(GraphicsComponent::GST_Cube);
	gc->SetOwner(&ent6);
	gc->Init(2, 1);
	gc->SetColor(COLOR_Yellow);
	ent6.AddComponent(gc);

	entities.push_back(ent6);
	DWORD prevTime = GetCurrentTime();

	// Main loop
	do
	{
		//add a real frame rate call here
		DWORD currentTime = GetCurrentTime();
		float deltaTime = (float)currentTime - (float)prevTime;
		deltaTime *= 0.001;
		prevTime = currentTime;
		Update(deltaTime);
		
		Render(window);

		while (SDL_PollEvent(&curEvent))
		{
			HandleEvents(&curEvent);
		}

	} while (game_running);

	physicsManager->Shutdown();
	return 0;
}

void PlayerWon()
{
	playerWon = true;
}

void Reset()
{
	playerWon = false;
	// Define the ball at the top of the screen
	inputComponent->Reset();
	player->ClearForces();	
	player->SetPosition(0, 15, 0);
	player->SetBodyPosition();
	
}

void InitGL()
{
	glClearColor(1, 1, 1, 1);
	glViewport(0, 0, screenWidth, screenHeight);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
}

void HandleEvents(SDL_Event* curEvent)
{
	switch (curEvent->type)
	{
	case SDL_QUIT:
		game_running = false;
		break;
	case SDL_KEYDOWN:
		switch (curEvent->key.keysym.sym)
		{
		default:
			break;
		}
		break;
	case SDL_KEYUP:
		switch (curEvent->key.keysym.sym)
		{
		case SDLK_w:
			PhysicsManager::GetInstance()->SetDebug( !PhysicsManager::GetInstance()->IsDebugOn() );
			break;
		case SDLK_a:
			inputComponent->LeftReleased();
			break;
		case SDLK_r:
			Reset();
			break;
		case SDLK_p:
			isPaused = !isPaused;
			break;
		case SDLK_d:
			inputComponent->RightReleased();
			break;
		case SDLK_SPACE:
			inputComponent->SetRelease();
			break;
		case SDLK_ESCAPE:
			game_running = false;
			break;
		}
		break;
	default:
		break;
	}
}

void Update(float dt)
{
	if (isPaused == false)
	{
		physicsManager->Update(dt);
		for(std::vector<Entity>::iterator it = entities.begin(); it != entities.end(); ++it)
		{
			(*it).Update(dt);
			if ((*it).CheckPlayerWon() == true)
			{
				PlayerWon();
			}
		}
	}
}

void Render(SDL_Window *window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, screenWidth / screenHeight, 0.1f, 250);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Should be change to camera class or similar.
	gluLookAt(0, 0, 30, 0, 0, 0, 0, 1, 0);
   
	if (isPaused == false)
	{
		text->WriteBitmapString(-10, 12, "Press space");
		text->WriteBitmapString(-13, 9, " to drop ball!");
		if (playerWon == true)
		{
			text->WriteBitmapString(-4, -10, "NICE SHOT!");
		}
		else
		{
			text->WriteBitmapString(-13, -10, "Press R to reset");
		}
	}
	else
	{
		text->WriteBitmapString(-5, 12, "Paused");
		text->WriteBitmapString(-14, 9, "Press p to resume");
	}
	
	for(std::vector<Entity>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
        glPushMatrix();
		(*it).Render();
        glPopMatrix();
	}

	SDL_GL_SwapWindow(window);
}