#include "pch.h"
#include "Address.h"
#include "BitBoard.h"
#include "Piece.h"
#include "RecordList.h"
#include "Valuation.h"

#include "PieceMap.h"

bool castlingMarker::operator==(const castlingMarker &right) {
  if (done)
    return right.done;
  else
    return kingside == right.kingside && queenside == right.queenside && !right.done;
}

bool castlingMarker::operator!=(const castlingMarker &right) {
  return !(*this == right);
}

STRING castlingMarkerArray::FEN_code(void) {
  STRING ret;
  if (at(WHITE).kingside)
    ret += 'K';
  if (at(WHITE).queenside)
    ret += 'Q';
  if (at(BLACK).kingside)
    ret += 'k';
  if (at(BLACK).queenside)
    ret += 'q';
  return ret;
}

void PieceMap::updateCastlingMark(Move move, bool isCastling) {
  if (isCastling) {
    _castling_mark[getPly() % 2].done = true;
    _castling_mark[getPly() % 2].queenside = false;
    _castling_mark[getPly() % 2].kingside = false;
  } else {
    if (move.form.get_rank() == 0 || move.form.get_rank() == 7) {
      color_t myColor = move.form.get_rank() == 0 ? WHITE : BLACK;
      switch (move.form.get_file()) {
        case 0:
          // queenside is impossible
          _castling_mark[myColor].queenside = false;
          break;
        case 4:
          // both sides are impossible
          _castling_mark[myColor].queenside = false;
          _castling_mark[myColor].kingside = false;
          break;
        case 7:
          // kingside is impossible
          _castling_mark[myColor].kingside = false;
          break;
      }
    }

    if (move.latt.get_rank() == 0 || move.latt.get_rank() == 7) {
      color_t myColor = move.latt.get_rank() == 0 ? WHITE : BLACK;
      switch (move.latt.get_file()) {
        case 0:
          // queenside is impossible
          _castling_mark[myColor].queenside = false;
          break;
        case 7:
          // kingside is impossible
          _castling_mark[myColor].kingside = false;
          break;
      }
    }
  }
}

Piece PieceMap::piece(Address ad) const {
  return _piece_map.at(ad);
}

PieceMap &PieceMap::init(std::string fen_code) {
  std::array<std::string, 6> section;
  std::istringstream i_stream(fen_code);
  for (std::string &sec : section)
    getline(i_stream, sec, ' ');

  // 0. Piece Placement Data
  std::fill(_piece_map.begin(), _piece_map.end(), Piece{WHITE, KING, false});
  int flag = 0;
  for (int rank = 7; rank >= 0; --rank) {
    for (int file = 0; file < 8; ++file) {
      char c = section.at(0).at(flag);
      if ('1' <= c && c <= '8')
        file += (c - '0') - 1;
      else {
        Address a = Address{file, rank};
        _piece_map.at(a) = Piece{c};
        if (!_piece_map.at(a).exist) {
          std::cerr << "Can't read a character \'"
                    << c << "\'" << std::endl;
          throw std::invalid_argument(
            "Invalid FEN Code! (Piece Placement Data)");
        }
      }
      ++flag;
    }
    if (rank > 0 && section.at(0).at(flag) != '/') {
      throw std::invalid_argument(
        "Invalid FEN Code! (Piece Placement Data, newline)");
    }
    ++flag;
  }

  // 1. Active Color
  switch (section.at(1).at(0)) {
    case 'w':
      _ply = 0;
      break;
    case 'b':
      _ply = 1;
      break;
    default:
      throw std::invalid_argument("Invalid FEN Code! (Active Color)");
  }

  // 2. Castling Availability
  flag = 0;
  for (int color = WHITE; color <= BLACK; ++color) {
    _castling_mark[color].kingside = false;
    _castling_mark[color].queenside = false;
  }

  for (char c : section.at(2)) {
    switch (c) {
      case 'K':
        _castling_mark[WHITE].kingside = true;
        break;
      case 'Q':
        _castling_mark[WHITE].queenside = true;
        break;
      case 'k':
        _castling_mark[BLACK].kingside = true;
        break;
      case 'q':
        _castling_mark[BLACK].queenside = true;
        break;
      case '-':
        break;
      default:
        throw std::invalid_argument(
          "Invalid FEN Code! (Castling Availabilty)");
    }
  }

  // 3. EnPassant Target Square
  // 4.
  // 5. fullmove number
  //    (starts from 1 and incremented when black moves)
  _ply += 2 * (stoi(section.at(5)) - 1);
  return *this;
}

size_t PieceMap::getPly(void) const {
  return _ply;
}

castlingMarker PieceMap::castling_mark(color_t color) const {
  return _castling_mark[color];
}

void PieceMap::move_piece(
  Move move, bool isCastling, bool isPromotion, bool isEnPassant) {
  updateCastlingMark(move, isCastling);

  // int x1 = move.form.get_file();
  int y1 = move.form.get_rank();
  int x2 = move.latt.get_file();
  int y2 = move.latt.get_rank();

  // copies the piece from its current position to distination
  _piece_map.at(move.latt) = _piece_map.at(move.form);

  // deletes the piece in its current position
  _piece_map.at(move.form).exist = false;

  if (isCastling) {
    switch (x2) {
      case 2:
        _piece_map.at(Address{3, y2}) = _piece_map.at(Address{0, y2});
        _piece_map.at(Address{0, y2}).exist = false;
        break;
      case 6:
        _piece_map.at(Address{5, y2}) = _piece_map.at(Address{7, y2});
        _piece_map.at(Address{7, y2}).exist = false;
        break;
    }
  } else if (isPromotion) {
    _piece_map.at(move.latt).name = move.prmObj;
  } else if (isEnPassant) {
    _piece_map.at(Address{x2, y1}).exist = false;
  }

  ++_ply;
}

void PieceMap::unmove_piece(Move move, Memory memory) {
  if (_ply == 0)
    throw std::range_error(
      "The method 'unmove_piece' is called"
      "when the board is in initial state (_ply = 0)!");

  --_ply;

  color_t myColor = getPly() % 2;

  // int x1 = move.form.get_file();
  int y1 = move.form.get_rank();
  int x2 = move.latt.get_file();
  int y2 = move.latt.get_rank();

  _piece_map.at(move.form).color = myColor;
  _piece_map.at(move.form).exist = true;
  _piece_map.at(move.latt) = memory.piece;

  if (memory.isCastling) {
    switch (x2) {
      case 2:
        // queenside
        _piece_map.at(Address{3, y2}).name = memory.promotionObject;
        _piece_map.at(Address{0, y2}) = Piece{myColor, ROOK, true};
        _piece_map.at(Address{3, y2}).exist = false;

        break;
      case 6:
        // kingside
        _piece_map.at(Address{5, y2}).name = memory.promotionObject;
        _piece_map.at(Address{7, y2}) = Piece{myColor, ROOK, true};
        _piece_map.at(Address{5, y2}).exist = false;
        break;
    }
  } else if (memory.isEnPassant) {
    _piece_map.at(Address{x2, y1}).exist = true;
  }
}

const std::string horizontal_line = "  ---------------------------------";

void PieceMap::output(std::ostream &os) {
  std::cout << std::endl
            << "PLY      : " << getPly() << std::endl
            << "CASTLING : " << _castling_mark.FEN_code() << std::endl
            << horizontal_line << std::endl;
  for (int y = 7; y >= 0; --y) {
    std::cout << y + 1 << " |";
    for (int x = 0; x < 8; ++x) {
      Piece piece = _piece_map.at(Address{x, y});
      std::cout << " " << piece.initial() << " |";
    }
    std::cout << std::endl
              << horizontal_line
              << std::endl;
  }

  std::cout << " ";
  for (int x = 0; x < 8; ++x)
    std::cout << "   " << static_cast<char>('a' + x);
  std::cout << std::endl;
}
