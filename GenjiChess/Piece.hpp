#pragma once

#include "pch.hpp"

// enum class Name
// {
//     king,
//     queen,
//     rook,
//     bishop,
//     knight,
//     pawn
// };

#define KING 0
#define QUEEN 1
#define ROOK 2
#define BISHOP 3
#define KNIGHT 4
#define PAWN 5

#define WHITE 0
#define BLACK 1

struct Piece
{
    color_t color;
    name_t name;
    bool exist;

    char initial(void) const;
    static char initial(Piece piece);

    Piece(void);
    Piece(color_t _color, name_t _name, bool _exist);
    Piece(char fen_code);

    bool operator==(const Piece &right) const;
    bool operator!=(const Piece &right) const;
    Piece &operator++(void);
};
