#include "pch.h"
#include "Board.h"
#include "Marking.h"

#include "Record.h"

Record::Record(Move move) {
  form = move.form;
  latt = move.latt;
  prmObj = move.prmObj;
  name = "";
  value = 0;
}

void Record::nameMove(const DynamicBoard &board) {
  name = nameMove(board, *this);
}

// names the move
STRING Record::nameMove(const DynamicBoard &board, Move move) {
  if (!move.form.is_valid() || !move.latt.is_valid())
    throw std::range_error(
      "The move is invalid (in Record::nameMove)\n");

  STRING str = _T("");
  try {
    if (move.isCastling(board)) {
      str = (move.latt.get_file() == 2 ? _T("O-O-O") : _T("O-O"));
    } else {
      if (board.getPiece(move.form).name != PAWN) {
        // initial of the piece
        str = Piece::initial({WHITE, board.getPiece(move.form).name, true});

        for (Address ad = 0; ad.is_valid(); ++ad) {
          if (ad != move.form &&
              board.getPiece(ad) == board.getPiece(move.form)) {
            // if there is the same piece
            // that can move to the same address...
            if (Marker::captureMove(board, ad).test(move.latt)) {
              if (ad.get_file() == move.form.get_file())
                str += (TCHAR)(move.form.get_rank() + (int)_T('1'));
              else
                str += (TCHAR)(move.form.get_file() + (int)_T('a'));
            }
          }
        }
      }
      if (move.isCaptureMove(board)) {
        if (board.getPiece(move.form).name == PAWN)
          str += (TCHAR)(move.form.get_file() + (int)_T('a'));

        str += 'x';
      }

      str += (TCHAR)(move.latt.get_file() + (int)_T('a'));
      str += (TCHAR)(move.latt.get_rank() + (int)_T('1'));

      if (move.isPromotion(board)) {
        str += _T("=");
        str += Piece::initial(Piece{WHITE, move.prmObj, true});
      }
    }

    // if (move.isCheck(board, marker))
    //     str += _T("+");
  } catch (const std::exception &) {
    std::cerr << "Exception in RecordList::nameMove\n";
    throw;
  }
  return str;
}

Record::Record(Move move, const DynamicBoard &board)
  : Record{move} {
  nameMove(board);
}
