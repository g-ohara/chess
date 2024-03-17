#pragma once
#include "pch.h"
#include "BitBoard.h"
#include "Board.h"

#define COLOR 2
#define NAME 6
#define FILE_NUM 8

enum class Color;

// BitBoard[64]
typedef std::array<BitBoard, 64> pattern_t;

// BitBoard[64][256][256]
// typedef std::array<std::array<std::array<BitBoard, 256>, 256>, 64> line_pattern_t;

// BitLine[8][256]
// BitLine[ライン上のrookの位置][ライン上のpieceの配置]
typedef std::array<std::array<BitLine, 256>, 8> line_array_t;

/**
 * @brief This class calculates where the piece move.
 */
class Marker {
 private:
  static pattern_t king_move;
  static pattern_t knight_move;

  static line_array_t rook_move;
  // line_pattern_t rook_movE = {0};

  static pattern_t normal_pawn_move[COLOR];
  static pattern_t first_pawn_move[COLOR];
  static pattern_t capture_pawn_move[COLOR];
  static pattern_t enpassant_move[COLOR][FILE_NUM + 1];

  static BitBoard kingside[COLOR];
  static BitBoard queenside[COLOR];

  static void king_init(void);
  static void rook_init(void);
  static void knight_init(void);
  static void pawn_init(void);

  static BitBoard castlingMarking(const StaticBoard &board);
  static BitBoard rookMarking(const StaticBoard &board, Address ad);
  static BitBoard bishopMarking(const StaticBoard &board, Address ad);

  static bool MarkingB(BitBoard &trg, Address ad, BitBoard myMap, BitBoard opMap);

 public:
  static BitBoard normalMove(const StaticBoard &board, Address ad);
  static BitBoard captureMove(const StaticBoard &board, Address ad);
  static BitBoard AllMove(const StaticBoard &board, Address ad);
  static BitBoard ModeMove(const StaticBoard &board, Address ad, int mode);

  // checks and removes suicide moves
  // ���E������O�i�I�[�o�[�w�b�h�ɒ��Ӂj
  static BitBoard normalMove_s(const DynamicBoard &board, Address ad);
  static BitBoard captureMove_s(const DynamicBoard &board, Address ad);
  static BitBoard AllMove_s(const DynamicBoard &board, Address ad);
  static BitBoard ModeMove_s(const DynamicBoard &board, Address ad, int mode);

  static bool opMarkCheck(const StaticBoard &board, color_t myColor, Address ad);
  static bool myKingChecked(const StaticBoard &board, color_t myColor);

  /**
   * @brief judges whether the move is valid or not
   * @return bool TRUE if the move is valid
   */
  static bool MoveIsValid(const StaticBoard &board, Move move);

  /**
   * @brief judges whether the move is legal or suicide move
   * @return bool TRUE if the move is legal
   */
  static bool MoveIsLegal(DynamicBoard board, Move move);

  Marker(void);
};
