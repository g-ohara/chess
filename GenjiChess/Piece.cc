#include "pch.h"
#include "Piece.h"

char Piece::initial(void) const {
  return initial(*this);
}

const char initial_c[6] = {'K', 'Q', 'R', 'B', 'N', 'P'};
char Piece::initial(Piece piece) {
  if (piece.exist) {
    char c = initial_c[piece.name];
    if (piece.color == BLACK)
      c -= 'A' - 'a';
    return c;
  } else {
    return ' ';
  }
}

Piece::Piece(void) : Piece{WHITE, KING, true} {
}

Piece::Piece(color_t _color, name_t _name, bool _exist)
  : color{_color}, name{_name}, exist{_exist} {
}

Piece::Piece(char fen_code) {
  color = WHITE;
  exist = true;

  if ('a' <= fen_code && fen_code <= 'z') {
    color = BLACK;
    fen_code -= ('a' - 'A');
  }

  switch (fen_code) {
    case 'K':
      name = KING;
      break;
    case 'Q':
      name = QUEEN;
      break;
    case 'R':
      name = ROOK;
      break;
    case 'B':
      name = BISHOP;
      break;
    case 'N':
      name = KNIGHT;
      break;
    case 'P':
      name = PAWN;
      break;
    default:
      name = KING;
      exist = false;
      break;
  }
}

bool Piece::operator==(const Piece &right) const {
  return color == right.color && name == right.name && exist == right.exist;
}

bool Piece::operator!=(const Piece &right) const {
  return !(*this == right);
}

Piece &Piece::operator++(void) {
  if (name == PAWN) {
    if (color == WHITE)
      color = BLACK;
    else
      exist = false;
  }
  name = (name + 1) % 6;
  return *this;
}
