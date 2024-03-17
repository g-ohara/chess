#include "pch.hpp"
#include "Address.hpp"
#include "Board.hpp"

int Address::get_file(void) const
{
	if (is_valid())
		return _index & 7;
	else
		throw std::range_error(
			"This address is invalid (in Address::get_file)");
}

int Address::get_rank(void) const
{
	if (is_valid())
		return _index >> 3;
	else
		throw std::range_error(
			"This address is invalid (in Address::get_rank)");
}

void Address::set_file(int num)
{
	_index = (_index & ~7) + num;
}

void Address::set_rank(int num)
{
	_index = (_index & 7) + (num << 3);
}

bool Address::is_valid(void) const
{
	return _index < 64;
}

std::string Address::get_str(void)
{
	std::string ret = "";
	ret += ('a' + static_cast<char>(get_file()));
	ret += std::to_string(get_rank() + 1);
	return ret;
}

Address Address::front(color_t color) const
{
	if (color == WHITE)
		if (is_valid() && get_rank() < 7)
			return Address{get_file(), get_rank() + 1};
		else
			return Address{64};
	else
		return back(WHITE);
}

Address Address::back(color_t color) const
{
	if (color == WHITE)
		if (is_valid() && get_rank() > 0)
			return Address{get_file(), get_rank() - 1};
		else
			return Address{64};
	else
		return front(WHITE);
}

Address Address::left(color_t color) const
{
	if (color == WHITE)
		if (is_valid() && get_file() > 0)
			return Address{get_file() - 1, get_rank()};
		else
			return Address{64};
	else
		return right(WHITE);
}

Address Address::right(color_t color) const
{
	if (color == WHITE)
		if (is_valid() && get_file() < 7)
			return Address{get_file() + 1, get_rank()};
		else
			return Address{64};
	else
		return left(WHITE);
}

Address::Address(void) : Address{0}
{
}

Address::Address(size_t index) : _index{index}
{
}

Address::Address(int x, int y) : Address{size_t(x + y * 8)}
{
}

bool Address::operator==(const Address &right) const
{
	return this->_index == right._index;
}

bool Address::operator!=(const Address &right) const
{
	return !(*this == right);
}

bool Address::operator<(const Address &right) const
{
	return this->_index < right._index;
}

Address &Address::operator=(const Address &right)
{
	_index = static_cast<size_t>(right);
	return (*this);
}

/**
 * @brief increments address (a1 < a2 < ... < a8 < b1 < ... < h8)
 */
Address &Address::operator++(void)
{
	++_index;
	return *this;
}

const Address Address::operator++(int)
{
	const Address answer = *this;
	++(*this);
	return answer;
}

Address::operator size_t(void) const
{
	return _index;
}

LoopAddress::LoopAddress(void)
{
}

LoopAddress::LoopAddress(BitBoard bitboard)
{
	ini(bitboard);
}

LoopAddress &LoopAddress::operator++(void)
{
	_index = (bb &= (bb - 1)).NTZ();
	return *this;
}

LoopAddress &LoopAddress::ini(BitBoard bitBoard)
{
	bb = bitBoard;
	_index = bb.NTZ();
	return *this;
}
