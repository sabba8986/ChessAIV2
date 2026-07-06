#include "history.hpp"
#include <utility>

History::History(Move move, UndoMove&& undo_info): 
    m_move{move}, m_undo_info{std::move(undo_info)}{}