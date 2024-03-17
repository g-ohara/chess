#pragma once

#define INF INT_MAX
#define TIMEUP INT_MIN

#include "pch.hpp"
#include "Address.hpp"
#include "Piece.hpp"

// Board.h
class StaticBoard;
class DynamicBoard;

// Marking.h
class Marker;

// CpuMove.h
class KillerMove;
class Thinker;

struct Move
{
    Address form = 0;
    Address latt = 0;
    name_t prmObj = QUEEN;

    // checks the type of move
    bool isCaptureMove(const StaticBoard &board) const;
    bool isCastling(const StaticBoard &board) const;
    bool isEnPassant(const StaticBoard &board) const;
    bool isPromotion(const StaticBoard &board) const;
    bool isCheck(DynamicBoard board, const Marker &marker) const;

    bool operator==(const Move &right) const;
};
