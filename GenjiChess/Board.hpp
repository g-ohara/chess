#pragma once

#include "pch.hpp"
#include "Address.hpp"
#include "BitBoardFamilyArray.hpp"
#include "Piece.hpp"
#include "PieceMap.hpp"
#include "RecordList.hpp"
#include "Valuation.hpp"

class StaticBoard
{
protected:
    PieceMap _piece_map;
    BitBoardFamilyArray _family_array;

    /**
     * @brief   the set_file in which enPassant is possible
     *          (if impossible anywhere, value is -1)
     */
    int m_enPassantMark = NO_ENPASSANT;

    Evaluator m_val;

public:
    /**
     * @brief   Gets the current bitboard family (BBF) from BBFA
     * @return  the current BBF
     */
    BitBoardFamily getBBF(void) const;

    /**
     * @brief   gets the piece at grid indicated by 'address'
     * @param   address
     * @return  the piece at grid indicated by 'address'
     */
    Piece getPiece(Address address) const;

    /**
     * @brief   gets the current ply
     * @return  the current ply
     */
    size_t getPly(void) const;

    int enPassantMark(void) const;
    castlingMarker castlingMark(color_t myColor) const;

    /**
     * @brief   initializes the piece map and BBFA (default)
     */
    virtual void init(void);

    /**
     * @brief   initializes the piece map and BBFA (FEN)
     * @param   fen_code
     */
    virtual void init(STRING fen_code);

    int count_piece(Piece piece) const;

    /**
     * @brief   counts the number of the same boards in BBFA
     * @return  the number of the same boards
     */
    int count_same_board(void) const;

    /**
     * @brief       checks consistency of the piece map and BBFA
     *              (for debug)
     * @attention   This method is FOR DEBUG!
     */
    void check_consistency(void) const;

    void output(std::ostream &os);

    bool operator==(const StaticBoard &board) const;

    StaticBoard(void);
    StaticBoard(STRING fen_code);
};

class DynamicBoard : public StaticBoard
{
private:
    RecordList _record_list;
    std::vector<Memory> memoryList;

    void memorization(Move move, bool isEnPassant);
    void move_piece_by_record(Record record);

    int _offset;

public:
    /**
     * @brief   moves a piece and memorizes that moving to unmove later,
     *          but does not record name of the move.
     * @param   move
     */
    virtual void movement(Move move, bool naming);

    void unmovement(void);

    /**
     * @brief   initializes the board and the record list
     */
    virtual void init(void) override;
    virtual void init(STRING fen_code) override;

    void initialization(Evaluator val);

    DynamicBoard(void);
    DynamicBoard(std::string fen_code);

    void output_record(int num);
};

class MainBoard_s : public DynamicBoard
{
public:
    void movement(Move move, bool naming = true) override;
};
