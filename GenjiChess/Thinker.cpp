#include "pch.hpp"
#include "Board.hpp"
#include "Address.hpp"
#include "Marking.hpp"
#include "LegalMove.hpp"
#include "OpeningList.hpp"
#include "RecordList.hpp"

#include "Thinker.hpp"

void MoveLine::output(std::ostream &os) const
{
    for (const Record &rec : *this)
        os << rec.name << " ";
    os << std::endl;
}

// checks whether thinking time is over
bool Thinker::timeUp(void) const
{
    if (finish != nullptr)
    {
        // std::lock_guard<std::mutex> lock(finish->mutex);
        if (finish->value)
            return true;
    }
    return m_timeLimit != INF && maxDepth > 0 && clock() - timer > m_timeLimit * CLOCKS_PER_SEC / 1000;
}

/**
 * @brief   manages the search programs without the opening list
 *
 * @return  std::vector<MoveLine>
 *          lines of move which follows the best several moves
 */
std::vector<MoveLine> Thinker::thinking(
    const DynamicBoard &board,
    int maxMaxDepth,
    int timeLimit,
    int line_num,
    FinishFlag *finish_flag,
    ComInfo *info)
{
    // the provisionally best move
    std::vector<MoveLine> bestMoves;
    m_board = board;
    finish = finish_flag;
    excluded_move.resize(0);

    // starts the timer
    m_timeLimit = timeLimit;
    timer = clock();

    if (timeLimit == INF)
    {
        maxDepth = maxMaxDepth;
        for (int i = 0; i < line_num; ++i)
        {
            bestMoves.push_back(gameTreeSearch(m_val.board_value(m_board)));
            excluded_move.push_back(bestMoves[i][0]);
        }

        if (info != nullptr)
        {
            info->opening = false;
            info->depth = maxDepth;
        }
    }
    else
    {
        for (maxDepth = 0; maxDepth <= maxMaxDepth; ++maxDepth)
        {
            // the tried move
            std::vector<MoveLine> move;
            excluded_move.resize(0);

            std::cout << "Thinking in depth " << maxDepth
                      << " ..." << std::endl;

            for (int i = 0; i < line_num; ++i)
            {
                move.push_back(gameTreeSearch(m_val.board_value(m_board)));
                excluded_move.push_back(move[i][0]);
                if (move[i][0].value == TIMEUP)
                    break;
            }

            // executes game-tree search and if it has reached the time limit...
            if (move[move.size() - 1][0].value == TIMEUP)
            {
                std::cout << "Time out\n";
                break;
            }
            else
            {
                std::cout << "Finished\n";
                bestMoves = move;
            }
        }
        std::cout << std::endl;

        if (info != nullptr)
        {
            info->opening = false;
            info->depth = maxDepth - 1;
            info->time = clock() - timer;
        }
    }

    return bestMoves;
}

// // manages the search programs with the opening list
// Record Thinker::thinking(
//     const class DynamicBoard &board, class OpeningList &openingList,
//     int maxMaxDepth, int timeLimit, ComInfo *info)
// {
//     // the provisionally best move
//     Record bestMove;

//     // starts the timer
//     m_timeLimit = timeLimit;
//     timer = clock();

//     // searches the opening database and if no moves can be found in the database...
//     if ((bestMove = openingList.openingSearch(m_board)).value == TIMEUP)
//     {
//         // starts the game-tree search
//         return thinking(board, maxMaxDepth, timeLimit, 1, nullptr, info)[0][0];
//     }
//     else
//     {
//         if (info != nullptr)
//             info->opening = true;
//         return bestMove;
//     }
// }

template <class Class>
bool my_find(const std::vector<Class> &arr, Class trg)
{
    return std::find(arr.begin(), arr.end(), trg) != arr.end();
}

// the game-tree search
const MoveLine Thinker::gameTreeSearch(
    int current_value,
    int currentDepth,
    int alpha,
    int beta)
{
    MoveLine bestMove(maxDepth - currentDepth + 1);
    bestMove[0].value = alpha;

    color_t myColor = m_board.getPly() % 2;
    color_t opColor = 1 - myColor;

    // searches legal moves
    for (LegalMove loop(m_board); loop.is_defined(); ++loop)
    {
        if (!(
                currentDepth == 0 &&
                my_find(excluded_move, static_cast<Move>(loop))))
        {
            // if king has been captured...
            if (m_board.getPiece(loop.latt) == Piece{opColor, KING, true})
            {
                // the previous move is illegal
                return (bestMove[0].value = INF, bestMove);
            }
            else
            {
                // int new_value = update_board_value(current_value, loop);

                // moves the piece
                m_board.movement(loop, false);
                MoveLine line(maxDepth - currentDepth);

                // if same boards appeared for 3 times or more...
                if (m_board.count_same_board() >= 3)
                {
                    // game is draw
                    loop.value = 0;
                }
                else if (currentDepth >= maxDepth)
                {
                    // loop.value = new_value;
                    loop.value = m_val.board_value(m_board);
                    if (myColor == BLACK)
                        loop.value *= -1;
                }
                else
                {
                    // searches deeper
                    line = gameTreeSearch(
                        m_val.board_value(m_board), // new_value,
                        currentDepth + 1,
                        -beta,
                        -bestMove[0].value);
                    loop.value = -line[0].value;
                }

                // unmoves the piece
                m_board.unmovement();

                // stops thinking if the thinking time has reached the limit
                if (timeUp())
                {
                    return (bestMove.at(0).value = TIMEUP, bestMove);
                }
                // beta cut
                if (loop.value >= beta)
                {
                    // avoids slicing
                    bestMove.at(0) = loop;
                    return bestMove;
                }

                // memorizes the provisionally best move
                if (loop.value > bestMove.at(0).value)
                {
                    bestMove.at(0) = Record(loop);
                    bestMove.add(line);
                }
            }
        }
    }

    // if there is no legal move...this board is in checkmate or stalemate
    if (bestMove.at(0).value == -INF && !Marker::myKingChecked(m_board, myColor))
        bestMove.at(0).value = 0;

    bestMove.at(0).nameMove(m_board);
    return bestMove;
}

int Thinker::update_board_value(int current_value, Move move)
{
    int ret = current_value;
    ret -= m_val.get_piece_value(m_board.getPiece(move.latt), move.latt);
    ret -= m_val.get_piece_value(m_board.getPiece(move.form), move.form);
    ret += m_val.get_piece_value(m_board.getPiece(move.form), move.latt);
    return ret;
}

bool Thinker::inMate(const DynamicBoard &board)
{
    return thinking(board, 1)[0][0].value == -INF;
}

bool Thinker::inCheckMate(const DynamicBoard &board)
{
    return inMate(board) && Marker::myKingChecked(board, board.getPly() % 2);
}

bool Thinker::inStaleMate(const DynamicBoard &board)
{
    return inMate(board) && !inCheckMate(board);
}
