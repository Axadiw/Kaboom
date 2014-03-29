#pragma once

#include "SDL.h"
#include "Singleton.h"
#include "Math\Vector2.h"
#include "includes.h"
#include "PolyMTLib.h"

#include <vector>
using namespace std;

class GameState;

/*! 
 *  \brief     Obiekt g³ówny silnika
 *  \author    Micha³ Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */
class GameEngine: public Singleton<GameEngine>
{
public:

	/// inicjuje okno gry
	void Init(const char* title);

	/// zwalnia zasoby
	void Cleanup();

	/// zmienia aktualny stan
	void ChangeState(GameState* state);

	/// dodaje stan na stos stanów
	void PushState(GameState* state);

	/// zdejmuje ostatnio dodany stan ze stosu
	void PopState();

	/// obs³uga zdarzeñ
	void HandleEvents();

	/// obs³uga logiki gry
	void Update();

	/// rysowanie na ekranie
	void Draw();

	/// czy gra jest uruchomiona
	bool Running() { return m_running; }

	/// zamknij grê
	void Quit() { m_running = false; }

	SDL_Surface* screen;

	/// delta czasu
	float dt;

	/// iloœæ wyœwietlanych klatek na sekundê
	float FPS;

	/// wymiary ekranu
	Vector2 screenDimensions;

	bool fullscreen;

	PolyMTLib* arEngine;
private:
	/// stos stanów gry
	vector<GameState*> states;

	bool m_running;	
};