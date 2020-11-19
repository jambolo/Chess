#if !defined(CHESS_RESPONSEGENERATOR_H)
#define CHESS_RESPONSEGENERATOR_H

#pragma once

#include <vector>

namespace GamePlayer { class GameState; }

class ResponseGenerator
{
public:
    std::vector<GamePlayer::GameState *> operator ()(GamePlayer::GameState const & state, int depth);
};

#endif // !defined(CHESS_RESPONSEGENERATOR_H)
