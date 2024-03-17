#include "pch.h"
#include "Board.h"
#include "Piece.h"
#include "Thinker.h"

#include "Marking.h"

pattern_t Marker::king_move{0};
pattern_t Marker::knight_move = {0};

line_array_t Marker::rook_move = {0};

pattern_t Marker::normal_pawn_move[COLOR] = {0};
pattern_t Marker::first_pawn_move[COLOR] = {0};
pattern_t Marker::capture_pawn_move[COLOR] = {0};
pattern_t Marker::enpassant_move[COLOR][FILE_NUM + 1] = {0};

BitBoard Marker::kingside[COLOR] = {0};
BitBoard Marker::queenside[COLOR] = {0};

BitBoard Marker::normalMove(const StaticBoard &board, Address ad) {
  const color_t myColor = board.getPiece(ad).color;
  const color_t opColor = 1 - myColor;

  BitBoard myMap = board.getBBF().color_map[myColor];
  BitBoard opMap = board.getBBF().color_map[opColor];

  BitBoard ret;

  switch (board.getPiece(ad).name) {
    case KING:
      ret = king_move.at(ad) & ~myMap & ~opMap;
      break;

    case QUEEN:
      ret = (rookMarking(board, ad) | bishopMarking(board, ad)) & ~myMap & ~opMap;
      break;

    case ROOK:
      ret = rookMarking(board, ad) & ~myMap & ~opMap;
      break;

    case BISHOP:
      ret = bishopMarking(board, ad) & ~myMap & ~opMap;
      break;

    case KNIGHT:
      ret = knight_move.at(ad) & ~myMap & ~opMap;
      break;

    case PAWN:
      ret = normal_pawn_move[myColor].at(ad) & ~myMap & ~opMap;
      if (ret.any())
        ret |= first_pawn_move[myColor].at(ad) & ~myMap & ~opMap;
      break;
  }

  return ret;
}

BitBoard Marker::captureMove(const StaticBoard &board, Address ad) {
  const color_t myColor = board.getPiece(ad).color;
  const color_t opColor = 1 - myColor;

  BitBoard opMap = board.getBBF().color_map[opColor];

  BitBoard ret;

  switch (board.getPiece(ad).name) {
    case KING:
      ret = (king_move.at(ad) & opMap) | castlingMarking(board);
      break;

    case QUEEN:
      ret = (rookMarking(board, ad) | bishopMarking(board, ad)) & opMap;
      break;

    case ROOK:
      ret = rookMarking(board, ad) & opMap;
      break;

    case BISHOP:
      ret = bishopMarking(board, ad) & opMap;
      break;

    case KNIGHT:
      ret = knight_move.at(ad) & opMap;
      break;

    case PAWN:
      ret = (capture_pawn_move[myColor].at(ad) & opMap) |
            (enpassant_move[myColor][board.enPassantMark()].at(ad));
      break;
  }

  return ret;
}

BitBoard Marker::AllMove(const StaticBoard &board, Address ad) {
  return normalMove(board, ad) | captureMove(board, ad);
}

BitBoard Marker::ModeMove(const StaticBoard &board, Address ad, int mode) {
  return mode ? normalMove(board, ad) : captureMove(board, ad);
}

void Marker::king_init(void) {
  for (Address ad = {0, 0}; ad.is_valid(); ++ad) {
    if (ad.get_file() > 0)
      king_move.at(ad).set(Address{ad.get_file() - 1, ad.get_rank()});

    if (ad.get_file() > 0 && ad.get_rank() > 0)
      king_move.at(ad).set(Address{ad.get_file() - 1, ad.get_rank() - 1});

    if (ad.get_file() > 0 && ad.get_rank() < 7)
      king_move.at(ad).set(Address{ad.get_file() - 1, ad.get_rank() + 1});

    if (ad.get_file() < 7)
      king_move.at(ad).set(Address{ad.get_file() + 1, ad.get_rank()});

    if (ad.get_file() < 7 && ad.get_rank() > 0)
      king_move.at(ad).set(Address{ad.get_file() + 1, ad.get_rank() - 1});

    if (ad.get_file() < 7 && ad.get_rank() < 7)
      king_move.at(ad).set(Address{ad.get_file() + 1, ad.get_rank() + 1});

    if (ad.get_rank() > 0)
      king_move.at(ad).set(Address{ad.get_file(), ad.get_rank() - 1});

    if (ad.get_rank() < 7)
      king_move.at(ad).set(Address{ad.get_file(), ad.get_rank() + 1});
  }

  kingside[0].set(Address{6, 0});
  kingside[1].set(Address{6, 7});
  queenside[0].set(Address{2, 0});
  queenside[1].set(Address{2, 7});
}

void Marker::rook_init(void) {
  for (int x = 0; x < 8; ++x) {
    for (int iLine = 0; iLine < 256; ++iLine) {
      BitLine line = static_cast<unsigned char>(iLine);
      for (int i = x - 1; i >= 0; --i) {
        rook_move[x][iLine].set(i);
        if (line.test(i))
          break;
      }
      for (int i = x + 1; i < 8; ++i) {
        rook_move[x][iLine].set(i);
        if (line.test(i))
          break;
      }
    }
  }
}
// 	for (Address ad = 0; ad.valid(); ++ad)
// 	{
// 		for (int iRank = 0; iRank < 256; ++iRank)
// 		{
// 			BitLine rank = static_cast<unsigned char>(iRank);
// 			for (int iFile = 0; iFile < 256; ++iFile)
// 			{
// 				BitLine file = static_cast<unsigned char>(iFile);
// 				for (int i = ad.file() - 1; i >= 0; --i)
// 				{
// 					rook_move[ad.file()][ad.rank()][iRank][iFile].set(Address{i, ad.rank()});
// 					if (rank.test(i))
// 						break;
// 				}
// 				for (int i = ad.file() + 1; i < 8; ++i)
// 				{
// 					rook_move[ad.file()][ad.rank()][iRank][iFile].set(Address{i, ad.rank()});
// 					if (rank.test(i))
// 						break;
// 				}
// 				for (int i = ad.rank() - 1; i >= 0; --i)
// 				{
// 					rook_move[ad.file()][ad.rank()][iRank][iFile].set(Address{ad.file(), i});
// 					if (file.test(i))
// 						break;
// 				}
// 				for (int i = ad.rank() + 1; i < 8; ++i)
// 				{
// 					rook_move[ad.file()][ad.rank()][iRank][iFile].set(Address{ad.file(), i});
// 					if (file.test(i))
// 						break;
// 				}
// 			}
// 		}
// 	}
// }

void Marker::knight_init(void) {
  for (Address ad = {0, 0}; ad.is_valid(); ++ad) {
    BitBoard &trg = knight_move.at(ad);
    if (ad.get_file() > 0 && ad.get_rank() < 6)
      trg.set((ad.get_file() - 1) + (ad.get_rank() + 2) * 8);
    if (ad.get_file() > 0 && ad.get_rank() > 1)
      trg.set((ad.get_file() - 1) + (ad.get_rank() - 2) * 8);
    if (ad.get_file() > 1 && ad.get_rank() < 7)
      trg.set((ad.get_file() - 2) + (ad.get_rank() + 1) * 8);
    if (ad.get_file() > 1 && ad.get_rank() > 0)
      trg.set((ad.get_file() - 2) + (ad.get_rank() - 1) * 8);
    if (ad.get_file() < 6 && ad.get_rank() < 7)
      trg.set((ad.get_file() + 2) + (ad.get_rank() + 1) * 8);
    if (ad.get_file() < 6 && ad.get_rank() > 0)
      trg.set((ad.get_file() + 2) + (ad.get_rank() - 1) * 8);
    if (ad.get_file() < 7 && ad.get_rank() < 6)
      trg.set((ad.get_file() + 1) + (ad.get_rank() + 2) * 8);
    if (ad.get_file() < 7 && ad.get_rank() > 1)
      trg.set((ad.get_file() + 1) + (ad.get_rank() - 2) * 8);
  }
}

void Marker::pawn_init(void) {
  for (int color = WHITE; color <= BLACK; ++color) {
    for (int file = 0; file < 8; ++file) {
      Address form{file, color == WHITE ? 1 : 6};
      Address dist;
      if ((dist = form.front(color).front(color)).is_valid())
        first_pawn_move[color].at(form).set(dist);
    }
    for (Address from; from.is_valid(); ++from) {
      Address dist;

      if ((dist = from.front(color)).is_valid())
        normal_pawn_move[color].at(from).set(dist);

      if ((dist = from.front(color).left(color)).is_valid())
        capture_pawn_move[color].at(from).set(dist);

      if ((dist = from.front(color).right(color)).is_valid())
        capture_pawn_move[color].at(from).set(dist);
    }
  }
  for (int x = 0; x < 8; ++x) {
    if (x > 0) {
      enpassant_move[WHITE][x].at(Address{x - 1, 4}).set(Address{x, 5});
      enpassant_move[BLACK][x].at(Address{x - 1, 3}).set(Address{x, 2});
    }
    if (x < 7) {
      enpassant_move[WHITE][x].at(Address{x + 1, 4}).set(Address{x, 5});
      enpassant_move[BLACK][x].at(Address{x + 1, 3}).set(Address{x, 2});
    }
  }
}

bool Marker::opMarkCheck(const StaticBoard &board, color_t myColor, Address ad) {
  const color_t opColor = 1 - myColor;

  BitBoardFamily bb = board.getBBF();
  if ((king_move.at(ad) &
       bb.color_map[opColor] &
       bb.name_map[KING])
      .any()) {
    return true;
  }
  if ((knight_move.at(ad) &
       bb.color_map[opColor] &
       bb.name_map[KNIGHT])
      .any()) {
    return true;
  }
  if ((capture_pawn_move[myColor].at(ad) &
       bb.color_map[opColor] &
       bb.name_map[PAWN])
      .any()) {
    return true;
  }
  if ((bishopMarking(board, ad) &
       bb.color_map[opColor] &
       (bb.name_map[BISHOP] | bb.name_map[QUEEN]))
      .any()) {
    return true;
  }
  if ((rookMarking(board, ad) &
       bb.color_map[opColor] &
       (bb.name_map[ROOK] | bb.name_map[QUEEN]))
      .any()) {
    return true;
  }
  return false;
}

bool Marker::myKingChecked(const StaticBoard &board, color_t color) {
  try {
    return opMarkCheck(
             board,
             color,
             (board.getBBF().piece_map(Piece{color, KING, true})).NTZ());
  } catch (const std::exception &) {
    std::cerr << "Exception in Marking::myKingChecked\n";
    throw;
  }
}

bool Marker::MoveIsValid(const StaticBoard &board, Move move) {
  return AllMove(board, move.form).test(move.latt);
}

bool Marker::MoveIsLegal(DynamicBoard board, Move move) {
  const color_t myColor = board.getPly() % 2;
  try {
    board.movement(move, false);
    return !myKingChecked(board, myColor);
  } catch (std::exception &ex) {
    std::cerr << "Exception in Marking::MoveIsLegal\n";
    throw;
  }
}

BitBoard Marker::castlingMarking(const StaticBoard &board) {
  const color_t myColor = board.getPly() % 2;

  BitBoard ret = 0;
  BitBoardFamily bb = board.getBBF();

  if (bb.castling_index[myColor].kingside &&
      !((BitBoard)(bb.exist_map())).test(Address{5, 7 * myColor}) &&
      !((BitBoard)(bb.exist_map())).test(Address{6, 7 * myColor}) &&
      !opMarkCheck(board, myColor, Address{4, 7 * myColor}) &&
      !opMarkCheck(board, myColor, Address{5, 7 * myColor}) &&
      !opMarkCheck(board, myColor, Address{6, 7 * myColor})) {
    ret |= kingside[myColor];
  }
  if (bb.castling_index[myColor].queenside &&
      !(bb.exist_map()).test(Address{3, 7 * myColor}) &&
      !(bb.exist_map()).test(Address{2, 7 * myColor}) &&
      !(bb.exist_map()).test(Address{1, 7 * myColor}) &&
      !opMarkCheck(board, myColor, Address{4, 7 * myColor}) &&
      !opMarkCheck(board, myColor, Address{3, 7 * myColor}) &&
      !opMarkCheck(board, myColor, Address{2, 7 * myColor})) {
    ret |= queenside[myColor];
  }
  return ret;
}

// BitBoard Marking::rookMarking(const StaticBoard &board, Address ad)
// {
// 	BitBoard exist_map = board.getBitBoard().exist_map();
// 	return rook_move[ad.file()][ad.rank()][exist_map.rank(ad.rank())][exist_map.file(ad.file())];
// }

BitBoard Marker::rookMarking(const StaticBoard &board, Address ad) {
  BitBoard ret = 0;

  BitBoard exist_map = board.getBBF().exist_map();
  ret.set_rank(rook_move.at(ad.get_file()).at(exist_map.rank(ad.get_rank())), ad.get_rank());
  ret.set_file(rook_move.at(ad.get_rank()).at(exist_map.file(ad.get_file())), ad.get_file());
  return ret;
}

BitBoard Marker::bishopMarking(const StaticBoard &board, Address ad) {
  const color_t myColor = board.getPly() % 2;
  const color_t opColor = 1 - myColor;

  const BitBoard myMap = board.getBBF().color_map[myColor];
  const BitBoard opMap = board.getBBF().color_map[opColor];

  BitBoard ret = 0;

  for (int i = 1; ad.get_file() + i < 8 && ad.get_rank() + i < 8; ++i)
    if (MarkingB(ret, {ad.get_file() + i, ad.get_rank() + i}, myMap, opMap))
      break;

  for (int i = 1; ad.get_file() - i >= 0 && ad.get_rank() + i < 8; ++i)
    if (MarkingB(ret, {ad.get_file() - i, ad.get_rank() + i}, myMap, opMap))
      break;

  for (int i = 1; ad.get_file() + i < 8 && ad.get_rank() - i >= 0; ++i)
    if (MarkingB(ret, {ad.get_file() + i, ad.get_rank() - i}, myMap, opMap))
      break;

  for (int i = 1; ad.get_file() - i >= 0 && ad.get_rank() - i >= 0; ++i)
    if (MarkingB(ret, {ad.get_file() - i, ad.get_rank() - i}, myMap, opMap))
      break;

  return ret;
}

bool Marker::MarkingB(
  BitBoard &trg, Address ad, BitBoard myMap, BitBoard opMap) {
  BitBoard trg_map;
  trg_map.set(ad);

  if ((myMap & trg_map).any()) {
    return true;
  } else {
    trg |= trg_map;
    return (opMap & trg_map).any();
  }
}

BitBoard Marker::normalMove_s(const DynamicBoard &board, Address ad) {
  BitBoard ret = normalMove(board, ad);
  for (LoopAddress i(ret); i.is_valid(); ++i)
    if (!MoveIsLegal(board, {ad, i}))
      ret.reset(i);
  return ret;
}

BitBoard Marker::captureMove_s(const DynamicBoard &board, Address ad) {
  BitBoard ret = captureMove(board, ad);
  for (LoopAddress i(ret); i.is_valid(); ++i)
    if (!MoveIsLegal(board, {ad, i}))
      ret.reset(i);
  return ret;
}

BitBoard Marker::AllMove_s(const DynamicBoard &board, Address ad) {
  return normalMove_s(board, ad) | captureMove_s(board, ad);
}

BitBoard Marker::ModeMove_s(const DynamicBoard &board, Address ad, int mode) {
  return mode ? normalMove_s(board, ad) : captureMove_s(board, ad);
}

Marker::Marker(void) {
  king_init();
  rook_init();
  knight_init();
  pawn_init();
}
