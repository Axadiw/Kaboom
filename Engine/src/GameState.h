#pragma once

#include "GameEngine.h"

/*! 
 *  \brief     Klasa abstrakcyjna opisuj¹ca stan gry
 *  \author    Micha³ Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class GameState
{
public:
	/// inicjacja zasobów (uruchamiane przy wchodzeniu do stanu)
	virtual void Init() = 0;

	/// zwalnianie zasobów (uruchamiane przy wychodzeniu ze stanu)
	virtual void Cleanup() = 0;

	/// chwilowo wstrzymaj dzia³anie stanu
	virtual void Pause() = 0;

	/// wznów dzia³anie stanu
	virtual void Resume() = 0;

	/// obs³uga zdarzeñ
	virtual void HandleEvents(GameEngine* game) = 0;

	/// obs³uga logiki gry
	virtual void Update(GameEngine* game) = 0;

	/// rysowanie
	virtual void Draw(GameEngine* game) = 0;

	/// zmienia stan
	void ChangeState(GameEngine* game, GameState* state) 
	{
		game->ChangeState(state);
	}
};

