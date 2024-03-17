#pragma once

#include "Record.h"

/**
 * @brief	class of a legal move
 * @details  constructor initializes members by a first legal move.@n
 *           oparator++() returns a next legal move.
 *
 */
class LegalMove : public Record {
 private:
  /**
   * @brief   the origin of current move
   */
  LoopAddress m_form;

  /**
   * @brief   the destination of current move
   */
  LoopAddress m_latt;

  /**
   * @brief   the bitboard for searching next legal move
   */
  BitBoard myMap;

  /**
   * @brief   the StaticBoard to be passed to ModeMove()
   */
  const StaticBoard &m_board;

  std::queue<Move> m_killerMove;

  int mode;

  bool set_first_move(void);

 public:
  /**
   * @brief    checks whether a move is defined
   * @retval   true    a former address and a latter one are both valid.
   */
  bool is_defined(void);

  /**
   * @brief    gets the next legal move
   * @returns  if there is the next legal move, returns it. @n
   *           if not, returns { 64, 64 } (not-defined move)
   */
  LegalMove &operator++(void);

  LegalMove(const StaticBoard &board);
  LegalMove(
    const StaticBoard &board, const std::queue<Move> &killerMove);
};
