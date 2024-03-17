#pragma once
#include "pch.h"
#include "Move.h"

struct Record : public Move {
  STRING name;
  int value = 0;

  /**
   * @brief       names a move
   * @param       [in]board  the board before moving a piece
   * @warning     Pass the board BEFORE MOVING as the argument 'board'!
   *              If not, this function returns invalid string.
   */
  void nameMove(const class DynamicBoard &board);

  /**
   * @brief       names a move
   * @param       [in]board  the board before moving a piece
   * @param       [in]move   a move to be named
   * @return      a name of a move (ex. e4, Nxc5, O-O-O, ...)
   * @warning     Pass the board BEFORE MOVING as the argument 'board'!
   *              If not, this function returns invalid string.
   */
  static STRING nameMove(const class DynamicBoard &board, Move move);

  Record(void) {}
  Record(Move move);
  Record(Move move, const class DynamicBoard &board);
};
