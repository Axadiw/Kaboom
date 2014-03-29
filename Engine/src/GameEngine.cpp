
#include <stdio.h>

#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "GameEngine.h"
#include "GameState.h"
#include "includes.h"

void GameEngine::Init(const char* title)
{
	_putenv("SDL_VIDEO_WINDOW_POS=center");
	_putenv("SDL_VIDEO_CENTERED=1");

	int flags = 0;
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		AX_PRINT_ERROR("SDL_Init error");
	}

	if ( fullscreen ) 
	{
		flags = SDL_FULLSCREEN;
	}

	SDL_WM_SetCaption(title, NULL);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	//Antyaliasing
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	SDL_SetVideoMode(static_cast<int>(screenDimensions.x), static_cast<int>(screenDimensions.y), 0, SDL_OPENGL | SDL_HWSURFACE | SDL_DOUBLEBUF | flags);	

	m_running = true;

	glEnable( GL_TEXTURE_2D );
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	glViewport( 0, 0, static_cast<int>(screenDimensions.x), static_cast<int>(screenDimensions.y));

	glClear( GL_COLOR_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	glOrtho(-SCREEN_WIDTH, SCREEN_WIDTH, -SCREEN_HEIGHT, SCREEN_HEIGHT, -1.0f, 1.0f);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	SDL_ShowCursor(0);

	if(IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG) < 0)
	{
		AX_PRINT_ERROR("IMG_init error");
	}
}

void GameEngine::Cleanup()
{
	// zdejmij wszystkie stany ze stosu
	while ( !states.empty() ) 
	{
		states.back()->Cleanup();
		states.pop_back();
	}

	// switch back to windowed mode so other 
	// programs won't get accidentally resized
	if ( fullscreen ) 
	{
		SDL_SetVideoMode(static_cast<int>(screenDimensions.x), static_cast<int>(screenDimensions.y), 0, SDL_OPENGL | SDL_FULLSCREEN);
	}

	// wy³¹cz SDL
	SDL_Quit();
	IMG_Quit();
}

void GameEngine::ChangeState(GameState* state) 
{
	// zwolnij zasoby aktywnego stanu
	if ( !states.empty() ) 
	{
		states.back()->Cleanup();
		states.pop_back();
	}

	// dodaj nowy stan i zainicjuj zasoby
	states.push_back(state);
	states.back()->Init();
}

void GameEngine::PushState(GameState* state)
{
	// zastowuj aktualny stan
	if ( !states.empty() ) 
	{
		states.back()->Pause();
	}

	// dodaj nowy stan i zainicjuj zasoby
	states.push_back(state);
	states.back()->Init();
}

void GameEngine::PopState()
{
	// zwolnij zasoby aktywnego stanu
	if ( !states.empty() ) 
	{
		states.back()->Cleanup();
		states.pop_back();
	}

	// je¿eli to mo¿liwe wznów dzia³anie ostatnio aktywnego stanu
	if ( !states.empty() ) 
	{
		states.back()->Resume();
	}
}


void GameEngine::HandleEvents() 
{
	states.back()->HandleEvents(this);
}

void GameEngine::Update() 
{
	states.back()->Update(this);
}

void GameEngine::Draw() 
{
	glClear( GL_COLOR_BUFFER_BIT );
	states.back()->Draw(this);

	// podmiana buforów OpenGL (podwójne buforowanie)
	SDL_GL_SwapBuffers();
}
