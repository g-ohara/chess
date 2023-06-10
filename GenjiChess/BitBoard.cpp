#include "pch.hpp"
#include "Address.hpp"
#include "BitBoard.hpp"

BitLine::operator unsigned char(void)
{
	return static_cast<unsigned char>(to_ulong());
}

BitLine::BitLine(unsigned char right)
{
	bitline_t::operator=(right);
}

int BitBoard::NTZ(void) const
{
	return static_cast<int>(BitBoard((*this & (~*this + 1LL)) - 1LL).count());
}

BitLine BitBoard::rank(int rank_index) const
{
	return static_cast<unsigned char>((to_ullong() >> 8 * rank_index) % 256);
}

BitLine BitBoard::file(int file_index) const
{
	BitLine ret = 0;
	for (int rank = 0; rank < 8; ++rank)
		ret |= (to_ullong() >> (7 * rank + file_index)) & (1LL << rank);
	return ret;
}

BitBoard &BitBoard::set_rank(const BitLine &line, int rank_index)
{
	operator|=(line.to_ullong() << rank_index * 8);
	return *this;
}

BitBoard &BitBoard::set_file(const BitLine &line, int file_index)
{
	for (int rank = 0; rank < 8; ++rank)
		operator|=((line.to_ullong() & (1LL << rank)) << (7 * rank + file_index));
	return *this;
}

void BitBoard::print_csv(std::ostream &os) const
{
	for (Address ad = 0; ad.is_valid(); ++ad)
		os << std::to_string(static_cast<int>(test(ad))) << ",";
}

BitBoard::operator unsigned long long(void)
{
	return to_ullong();
}

BitBoard &BitBoard::set(size_t pos, bool val)
{
	bitboard_t::set(pos, val);
	return *this;
}

BitBoard &BitBoard::reset(size_t pos)
{
	bitboard_t::reset(pos);
	return *this;
}

BitBoard::BitBoard(void) : BitBoard{0}
{
}

BitBoard::BitBoard(unsigned long long right)
{
	bitboard_t::operator=(right);
}

BitBoard BitBoard::operator~(void) const
{
	BitBoard ret = *this;
	ret.flip();
	return ret;
}

BitBoard &BitBoard::operator=(unsigned long long &right)
{
	bitboard_t::operator=(right);
	return *this;
}

BitBoard &BitBoard::operator&=(unsigned long long &right)
{
	bitboard_t::operator&=(right);
	return *this;
}

BitBoard &BitBoard::operator&=(const BitBoard &right)
{
	bitboard_t::operator&=(right);
	return *this;
}

BitBoard &BitBoard::operator|=(unsigned long long &right)
{
	bitboard_t::operator|=(right);
	return *this;
}

BitBoard &BitBoard::operator|=(const BitBoard &right)
{
	bitboard_t::operator|=(right);
	return *this;
}

BitBoard BitBoard::operator&(const BitBoard &right) const
{
	BitBoard ret = *this;
	ret &= right;
	return ret;
}

BitBoard BitBoard::operator|(const BitBoard &right) const
{
	BitBoard ret = *this;
	ret |= right;
	return ret;
}
