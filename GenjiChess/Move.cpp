#include "pch.hpp"
#include "Board.hpp"
#include "Move.hpp"
#include "Thinker.hpp"
#include "Marking.hpp"

bool Move::isCaptureMove(const StaticBoard &board) const
{
	return board.getPiece(latt).exist || isEnPassant(board);
}

bool Move::isCastling(const StaticBoard &board) const
{
	return board.getPiece(form).name == KING &&
		   abs(latt.get_file() - form.get_file()) == 2;
}

bool Move::isPromotion(const StaticBoard &board) const
{
	return board.getPiece(form).name == PAWN &&
		   (latt.get_rank() == 0 || latt.get_rank() == 7);
}

bool Move::isEnPassant(const StaticBoard &board) const
{
	return board.getPiece(form).name == PAWN &&
		   form.get_file() != latt.get_file() && !board.getPiece(latt).exist;
}

bool Move::isCheck(DynamicBoard board, const Marker &marker) const
{
	try
	{
		board.movement(*this, false);
		return marker.myKingChecked(board, board.getPly() % 2);
	}
	catch (const std::exception &)
	{
		std::cerr << "Exception in Move::isCheck\n";
		throw;
	}
}

bool Move::operator==(const Move &right) const
{
	return form == right.form &&
		   latt == right.latt &&
		   prmObj == right.prmObj;
}
