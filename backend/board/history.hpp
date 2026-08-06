#pragma once
#include "move.hpp"
#include "undo_move.hpp"

struct History{
    Move m_move;
    UndoMove m_undo_info;

    History() = default;
    History(Move move, UndoMove&& undo_info);
};
