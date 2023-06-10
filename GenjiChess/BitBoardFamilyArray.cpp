#include "pch.hpp"
#include "Move.hpp"

#include "BitBoardFamilyArray.hpp"

BitBoardFamily BitBoardFamilyArray::getBitBoardFamily(void) const
{
    BitBoardFamily ret;
    try
    {
        ret = _family.at(getPly());
    }
    catch (const std::exception &e)
    {
        std::cerr << "Invalid Access to BitBoardFamilyArray!\n"
                  << e.what() << '\n';
    }
    return ret;
}

size_t BitBoardFamilyArray::getPly(void) const
{
    return _ply;
}

int BitBoardFamilyArray::count_same_board(void) const
{
    return static_cast<int>(
        std::count(
            _family.begin() + _family[getPly()].start_flag,
            _family.begin() + getPly(),
            _family[getPly()]));
}

void BitBoardFamilyArray::move_piece(
    Move move,
    const PieceMap &piece_map,
    bool isCastling, bool isPromotion, bool isEnPassant, bool isCapture)
{
    while (_family.size() <= _ply + 1)
        _family.push_back(BitBoardFamily{0});

    BitBoardFamily &bbf = _family.at(getPly() + 1);
    bbf = _family.at(getPly());
    bbf.move_piece(
        move, piece_map, isCastling, isPromotion, isEnPassant, isCapture);
    ++_ply;
}

void BitBoardFamilyArray::unmove_piece(void)
{
    if (_ply == 0)
        throw std::range_error(
            "The method 'unmove_piece' is called"
            "when the board is in initial state (_ply = 0)!");
    --_ply;
}

void BitBoardFamilyArray::init(const PieceMap &piece_map)
{
    BitBoardFamily new_bbf;
    _ply = 0;
    new_bbf.castling_index[WHITE] = piece_map.castling_mark(WHITE);
    new_bbf.castling_index[BLACK] = piece_map.castling_mark(BLACK);

    for (Address ad = {0, 0}; ad.is_valid(); ++ad)
    {
        Piece piece = piece_map.piece(ad);
        if (piece.exist)
        {
            new_bbf.color_map[piece.color].set(ad);
            new_bbf.name_map[piece.name].set(ad);
        }
    }
    _family.resize(0);
    _family.push_back(new_bbf);
}

int BitBoardFamilyArray::count_piece(Piece piece) const
{
    return getBitBoardFamily().count_piece(piece);
}

void BitBoardFamilyArray::output(std::ostream &os)
{
    getBitBoardFamily().output(os);
}