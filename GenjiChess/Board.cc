#include "pch.h"
#include "Address.h"
#include "Move.h"
#include "Valuation.h"

#include "Board.h"

Piece StaticBoard::getPiece(Address ad) const {
  return _piece_map.piece(ad);
}

size_t StaticBoard::getPly(void) const {
  return _piece_map.getPly();
}

int StaticBoard::enPassantMark(void) const {
  return m_enPassantMark;
}

castlingMarker StaticBoard::castlingMark(color_t myColor) const {
  return getBBF().castling_index[myColor];
}

BitBoardFamily StaticBoard::getBBF(void) const {
  return _family_array.getBitBoardFamily();
}

void StaticBoard::init(void) {
  init("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void StaticBoard::init(STRING fen_code) {
  _piece_map.init(fen_code);
  _family_array.init(_piece_map);
}

void StaticBoard::check_consistency(void) const {
  BitBoardFamily l_bb = getBBF();

  // tests the ply
  if (_family_array.getPly() != _piece_map.getPly())
    throw std::runtime_error(
      "The values of ply are not consistent.");

  // tests color maps
  for (int myColor = WHITE; myColor <= BLACK; ++myColor)
    for (Address ad = {0, 0}; ad.is_valid(); ++ad) {
      Piece trg = getPiece(ad);
      bool bb_test = l_bb.color_map[myColor].test(ad);
      bool map_test = (trg.color == myColor && trg.exist);
      if (bb_test != map_test) {
        std::cerr << "Color: " << myColor << std::endl
                  << "File : " << ad.get_file() << std::endl
                  << "Rank : " << ad.get_rank() << std::endl
                  << "PMap : " << map_test << std::endl
                  << "BBFA : " << bb_test << std::endl;
        throw std::runtime_error(
          "The color maps are not consistent.");
      }
    }

  // tests name maps
  for (int myName = 0; myName < 6; ++myName)
    for (Address ad = {0, 0}; ad.is_valid(); ++ad) {
      Piece trg = getPiece(ad);
      bool bb_test = l_bb.name_map[myName].test(ad);
      bool map_test = (trg.name == myName && trg.exist);
      if (bb_test != map_test) {
        std::cerr << "Name : " << myName << std::endl
                  << "File : " << ad.get_file() << std::endl
                  << "Rank : " << ad.get_rank() << std::endl
                  << "PMap : " << map_test << std::endl
                  << "BBFA : " << bb_test << std::endl;
        throw std::runtime_error(
          "The name maps are not consistent.");
      }
    }
}

StaticBoard::StaticBoard(void) {
  init();
}

StaticBoard::StaticBoard(STRING fen_code) {
  init(fen_code);
}

int StaticBoard::count_piece(Piece piece) const {
  return getBBF().count_piece(piece);
}

int StaticBoard::count_same_board(void) const {
  return _family_array.count_same_board();
}

bool StaticBoard::operator==(const StaticBoard &board) const {
  // checks equality of bitboards
  if (getBBF() != board.getBBF())
    return false;

  // checks equality of piece tables
  for (Address i = 0; i.is_valid(); ++i)
    if (getPiece(i) != board.getPiece(i))
      return false;

  return true;
}

void StaticBoard::output(std::ostream &os) {
  _piece_map.output(os);
  // _family_array.output(os);
}

void DynamicBoard::move_piece_by_record(Record record) {
  try {
    memorization(record, record.isPromotion(*this));

    bool isCastling = record.isCastling(*this);
    bool isPromotion = record.isPromotion(*this);
    bool isEnPassant = record.isEnPassant(*this);
    bool isCapture = record.isCaptureMove(*this);

    _record_list.move_piece(record);

    _family_array.move_piece(
      record,
      _piece_map,
      isCastling,
      isPromotion,
      isEnPassant,
      isCapture);

    _piece_map.move_piece(
      record,
      isCastling,
      isPromotion,
      isEnPassant);

    m_enPassantMark = (abs(record.latt.get_rank() - record.form.get_rank()) == 2 && getPiece(record.latt).name == PAWN) ? record.latt.get_file() : NO_ENPASSANT;
  } catch (const std::exception &e) {
    std::cerr << "Exception in DynamicBoard::movement\n";
    throw;
  }
}

void DynamicBoard::movement(Move move, bool naming) {
  if (naming)
    move_piece_by_record(Record{move, *this});
  else
    move_piece_by_record(Record{move});
}

void MainBoard_s::movement(Move move, bool naming) {
  color_t myColor = getPiece(move.form).color;
  color_t targetColor = getPiece(move.latt).color;

  if (!(myColor == (color_t)(getPly() % 2)) || myColor == targetColor)
    return;

  DynamicBoard::movement(move, naming);
}

void DynamicBoard::unmovement(void) {
  try {
    int prev_ply = getPly() - 1;
    _piece_map.unmove_piece(
      _record_list.get_record(),
      memoryList.at(prev_ply - _offset));
    _family_array.unmove_piece();
    _record_list.unmove_piece();
    m_enPassantMark = memoryList.at(prev_ply - _offset).enPassantMark;
  } catch (const std::exception &e) {
    std::cerr << "Exception in DynamicBoard::unmovement\n";
    throw;
  }
}

// memorizes the move
void DynamicBoard::memorization(Move move, bool isEnPassant) {
  Memory info;
  info.piece = getPiece(move.latt);
  info.enPassantMark = enPassantMark();
  info.isCastling = false;
  info.isEnPassant = isEnPassant;

  if (move.isCastling(*this)) {
    info.promotionObject = getPiece({move.latt.get_file() == 2 ? 3 : 5, move.latt.get_rank()}).name;
    info.isCastling = true;
  } else if (move.isEnPassant(*this)) {
    info.isEnPassant = true;
  }
  while (true) {
    if (getPly() >= memoryList.size()) {
      // add the vector size
      memoryList.push_back(info);
    } else {
      memoryList[getPly()] = info;
      break;
    }
  }
}

void DynamicBoard::init(void) {
  StaticBoard::init();
  _record_list.init(getPly());
}

void DynamicBoard::init(STRING fen_code) {
  StaticBoard::init(fen_code);
  _record_list.init(getPly());
}

DynamicBoard::DynamicBoard(void) {
  init();
  _offset = getPly();
}

DynamicBoard::DynamicBoard(std::string fen_code) {
  init(fen_code);
  _offset = getPly();
}

void DynamicBoard::output_record(int num) {
  _record_list.output_record(num);
}
