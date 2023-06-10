#pragma once
#include "pch.hpp"
#include "Piece.hpp"

struct castlingMarker;
class BitBoardFamily;

constexpr int existance_value[6] = {0, 900, 500, 300, 300, 90};
constexpr int development_value[6] = {0, -5, 0, 10, 15, 10};
constexpr int centered_pawn_value = 80;

// not-able-to-castle-in-one-side, not-able-to-castle-in-both-side, already-castled
constexpr int castling_const[4] = {-50, -120, 0, 50};

constexpr int vec_size = 64 * 6 + 1 + 2 + 1;
typedef Eigen::Matrix<int, vec_size, 1> Feature;

class Evaluator
{
private:
	int piece_value[6][64] = {0};
	int castling_value[2][2][2] = {0};

	//-------------------------------------

	Feature _coef;

	//-------------------------------------

public:
	//-------------------------------------

	void set_coef(const std::vector<int> &arr);
	static Feature get_feature(const StaticBoard &board);

	//-------------------------------------

	int get_piece_value(Piece piece, Address address) const;
	int get_castling_value(castlingMarker marker) const;
	int board_value(const StaticBoard &board);

	Evaluator();
	Evaluator(std::string file_name);
	~Evaluator();
};
