#pragma once

#include "BitBoardFamily.hpp"

class BitBoardFamilyArray
{
private:
    std::vector<BitBoardFamily> _family;
    size_t _ply;

public:
    /**
     * @brief   Get the Bit Board Family object
     * @param   ply
     * @return  BitBoardFamily
     */
    BitBoardFamily getBitBoardFamily(void) const;

    /**
     * @brief   Get the current ply
     * @return  the current ply
     */
    size_t getPly(void) const;

    /**
     * @brief   Initializes the board by syncing with the passed piece map
     * @param   piece_map   the piece map that BBFA sync
     */
    void init(const PieceMap &piece_map);

    /**
     * @brief   Moves the piece and appends the board after move on BBFA
     * @param   move
     * @param   piece_map
     * @param   isCastling
     * @param   isPromotion
     * @param   isEnPassant
     * @param   isCapture
     */
    void move_piece(
        Move move,
        const PieceMap &piece_map,
        bool isCastling, bool isPromotion, bool isEnPassant, bool isCapture);

    /**
     * @brief   decriments _ply (not delete the current board)
     */
    void unmove_piece(void);

    int count_piece(Piece piece) const;

    /**
     * @brief   counts the number of the same boards in array ([0 : _ply])
     * @return  the number of the same boards
     */
    int count_same_board(void) const;

    void output(std::ostream &os);
};
