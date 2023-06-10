#pragma once
#include <vector>
#include <queue>

#include "Marking.hpp"
#include "Move.hpp"
#include "Valuation.hpp"

class MoveLine : public std::vector<Record>
{
public:
	MoveLine &add(MoveLine &line)
	{
		if (line.size() != size() - 1)
			return *this;
		std::copy(line.begin(), line.end(), begin() + 1);
		return *this;
	}
	MoveLine(void) : std::vector<Record>{} {}
	MoveLine(size_t size) : std::vector<Record>{size} {}
	void output(std::ostream &os) const;
};

struct ComInfo
{
	bool opening = false;
	int depth = 0;
	int time = 0;
};

struct FinishFlag
{
	bool value = false;
	// std::mutex mutex;
};

class Thinker
{
private:
	class DynamicBoard m_board;

	int timer = 0;
	int m_timeLimit = 0;
	int maxDepth = 0;
	FinishFlag *finish = nullptr;

	std::vector<Move> excluded_move;

	bool timeUp(void) const;

	// searches a game tree and makes the best move
	const MoveLine gameTreeSearch(
		int current_value = 0,
		int currentDepth = 0,
		int alpha = -INF,
		int beta = INF);

	Evaluator m_val;

	int update_board_value(int current_value, Move move);

public:
	void set_value_const(Evaluator val) { m_val = val; }

	Thinker(void) : m_val{"Data/coef-endgame.csv"} {}

	/**
	 * @brief 	manages the CPU's thinking
	 *
	 * @param 	bd
	 * @param 	maxDepth
	 * @param 	timeLimit
	 * @param 	line_num
	 * @param 	finish_flag
	 * @param 	info
	 * @return 	* std::vector<MoveLine>
	 * @bug		unit of 'info->time' is 'μsec.' when executed in Linux,
	 * 			but 'msec.' when executed in Windows.
	 */
	std::vector<MoveLine> thinking(
		const DynamicBoard &bd,
		int maxDepth,
		int timeLimit = INF,
		int line_num = 1,
		FinishFlag *finish_flag = nullptr,
		ComInfo *info = nullptr);

	// Record thinking(
	// 	const class DynamicBoard &bd,
	// 	class OpeningList &openingList,
	// 	int maxDepth,
	// 	int timeLimit = INF,
	// 	ComInfo *info = nullptr);

	/**
	 * @brief 	judges if the board is in mate (checkmate or stalemate) or not
	 *
	 * @param 	board	the board to be judged
	 * @retval 	true	mate
	 * @retval 	false	not mate
	 */
	bool inMate(const DynamicBoard &board);

	/**
	 * @brief 	judges if the board is in checkmate or not
	 *
	 * @param 	board	the board to be judged
	 * @retval 	true	checkmate
	 * @retval 	false	not checkmate
	 */
	bool inCheckMate(const DynamicBoard &board);

	/**
	 * @brief 	judges if the board is in stalemate or not
	 *
	 * @param 	board	the board to be judged
	 * @retval 	true	stalemate
	 * @retval 	false	not stalemate
	 */
	bool inStaleMate(const DynamicBoard &board);
};
