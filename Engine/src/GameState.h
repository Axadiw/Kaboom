#pragma once

#include "GameEngine.h"

/*! 
 *  \brief     Klasa abstrakcyjna opisuj�ca stan gry
 *  \author    Micha� Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class GameState
{
public:
	/// inicjacja zasob�w (uruchamiane przy wchodzeniu do stanu)
	virtual void Init() = 0;

	/// zwalnianie zasob�w (uruchamiane przy wychodzeniu ze stanu)
	virtual void Cleanup() = 0;

	/// chwilowo wstrzymaj dzia�anie stanu
	virtual void Pause() = 0;

	/// wzn�w dzia�anie stanu
	virtual void Resume() = 0;

	/// obs�uga zdarze�
	virtual void HandleEvents(GameEngine* game) = 0;

	/// obs�uga logiki gry
	virtual void Update(GameEngine* game) = 0;

	/// rysowanie
	virtual void Draw(GameEngine* game) = 0;

	/// zmienia stan
	void ChangeState(GameEngine* game, GameState* state) 
	{
		game->ChangeState(state);
	}
};

