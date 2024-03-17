#pragma once
#include "BitBoard.h"
#include "PieceMap.h"

struct BitBoardFamily {
  BitBoard color_map[2];
  BitBoard name_map[6];
  castlingMarkerArray castling_index;
  int start_flag = 0;
  bool operator!=(const BitBoardFamily &right);
  BitBoard exist_map(void);
  BitBoard piece_map(Piece piece) const;

  void move_piece(
    Move move, PieceMap piece_map,
    bool isCastling, bool isPromotion, bool isEnPassant, bool isCapture);

  int count_piece(void) const;
  int count_piece(Piece piece) const;
  void output(std::ostream &os);

  /**
   * @brief   prints bitboard for all pieces in csv form
   * @param   os  the output stream
   */
  void print_csv(std::ostream &os) const;
};

bool operator==(const BitBoardFamily &left, const BitBoardFamily &right);
