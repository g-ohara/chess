#include "Address.h"
#include "Move.h"

#include "BitBoardFamily.h"

bool operator==(const BitBoardFamily &left, const BitBoardFamily &right) {
  for (int color = WHITE; color <= BLACK; ++color)
    if (left.color_map[color] != right.color_map[color])
      return false;

  for (int name = KING; name <= PAWN; ++name)
    if (left.name_map[name] != right.name_map[name])
      return false;

  return true;
}

bool BitBoardFamily::operator!=(const BitBoardFamily &right) {
  return !(*this == right);
}

BitBoard BitBoardFamily::exist_map(void) {
  return color_map[WHITE] | color_map[BLACK];
}

void BitBoardFamily::move_piece(
  Move move, PieceMap piece_map,
  bool isCastling, bool isPromotion, bool isEnPassant, bool isCapture) {
  int ply = piece_map.getPly();

  int myColor = ply % 2;
  int opColor = 1 - myColor;
  int myName = piece_map.piece(move.form).name;
  int opName = piece_map.piece(move.latt).name;

  BitBoardFamily &bbf = *this;

  // deletes the captured piece from the color map
  bbf.color_map[opColor].reset(move.latt);

  // moves the moved piece on the color map
  bbf.color_map[myColor].reset(move.form);
  bbf.color_map[myColor].set(move.latt);

  // deletes the captured piece from the name map
  bbf.name_map[opName].reset(move.latt);

  // deletes the moved piece from the name map
  bbf.name_map[myName].reset(move.form);

  if (isPromotion) {
    bbf.name_map[move.prmObj].set(move.latt);
    bbf.start_flag = ply;
  } else {
    bbf.name_map[myName].set(move.latt);

    if (isCastling) {
      switch (move.latt.get_file()) {
        case 2:
          bbf.color_map[myColor].set(move.latt + 1);
          bbf.color_map[myColor].reset(move.latt - 2);
          bbf.name_map[ROOK].set(move.latt + 1);
          bbf.name_map[ROOK].reset(move.latt - 2);
          break;
        case 6:
          bbf.color_map[myColor].set(move.latt - 1);
          bbf.color_map[myColor].reset(move.latt + 1);
          bbf.name_map[ROOK].set(move.latt - 1);
          bbf.name_map[ROOK].reset(move.latt + 1);
          break;
      }
      bbf.start_flag = ply;
    } else if (isEnPassant) {
      bbf.color_map[opColor].reset(
        Address{move.latt.get_file(), move.form.get_rank()});
      bbf.name_map[PAWN].reset(
        Address{move.latt.get_file(), move.form.get_rank()});
      bbf.start_flag = ply;
    } else if (isCapture || piece_map.piece(move.form).name == PAWN) {
      bbf.start_flag = ply;
    }
  }

  for (int color = WHITE; color <= BLACK; ++color)
    bbf.castling_index[color] = piece_map.castling_mark(color);
}

BitBoard BitBoardFamily::piece_map(Piece piece) const {
  return color_map[piece.color] & name_map[piece.name];
}

int BitBoardFamily::count_piece(void) const {
  return (color_map[WHITE] | color_map[BLACK]).count();
}

int BitBoardFamily::count_piece(Piece piece) const {
  return (color_map[piece.color] & name_map[piece.name]).count();
}

void BitBoardFamily::print_csv(std::ostream &os) const {
  for (int color = WHITE; color <= BLACK; ++color)
    for (int name = QUEEN; name <= PAWN; ++name) {
      Piece piece{(color_t)color, name, true};
      piece_map(piece).print_csv(os);
    }
}

const std::string horizontal_line = "  ---------------------------------";

void BitBoardFamily::output(std::ostream &os) {
  BitBoard bb = name_map[PAWN];
  std::cout << std::endl;
  std::cout << horizontal_line << std::endl;
  for (int y = 7; y >= 0; --y) {
    std::cout << y + 1 << " |";
    for (int x = 0; x < 8; ++x) {
      std::cout << " " << bb.test(Address{x, y}) << " |";
    }
    std::cout << std::endl
              << horizontal_line
              << std::endl;
  }

  std::cout << " ";
  for (int x = 0; x < 8; ++x) {
    std::cout << "   " << static_cast<char>('a' + x);
  }
  std::cout << std::endl;
}
