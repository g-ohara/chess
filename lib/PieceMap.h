#pragma once

#include "pch.h"
#include "Address.h"
#include "Piece.h"

class Move;

#define NO_ENPASSANT 8

struct Memory {
  Piece piece;
  int enPassantMark = NO_ENPASSANT;
  name_t promotionObject = QUEEN;

  bool isCastling;
  bool isEnPassant;
};

struct castlingMarker {
  bool kingside = true;
  bool queenside = true;
  bool done = false;
  bool operator==(const castlingMarker &right);
  bool operator!=(const castlingMarker &right);
};

class castlingMarkerArray : public std::array<castlingMarker, 2> {
 public:
  STRING FEN_code(void);
};

class PieceMap {
 private:
  /**
   * @brief   1-dim array of pieces
   *          accesses value by class Address
   */
  std::array<Piece, 64> _piece_map;

  size_t _ply;
  castlingMarkerArray _castling_mark;

  void updateCastlingMark(Move move, bool isCastling);

 public:
  Piece piece(Address ad) const;

  PieceMap &init(STRING fen_code);

  size_t getPly(void) const;
  castlingMarker castling_mark(color_t color) const;

  void move_piece(
    Move move, bool isCastling, bool isPromotion, bool isEnPassant);
  void unmove_piece(Move move, struct Memory memory);
  void output(std::ostream &os);
};
