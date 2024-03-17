#pragma once
#include <bitset>

typedef std::bitset<8> bitline_t;

class BitLine : public bitline_t {
 public:
  operator unsigned char(void);

  BitLine(void) : bitline_t{} {}
  BitLine(unsigned char);
};

typedef std::bitset<64> bitboard_t;

// Address.h
class Address;

class BitBoard : public bitboard_t {
 public:
  /**
   * @brief 	gets Number of Training Zero
   * @return 	int		Number of training zero
   */
  int NTZ(void) const;

  BitLine rank(int rank_index) const;
  BitLine file(int file_index) const;

  BitBoard &set_rank(const BitLine &line, int rank_index);
  BitBoard &set_file(const BitLine &line, int file_index);

  /**
   * @brief 	print itself in csv form
   * @param 	os	output stream
   */
  void print_csv(std::ostream &os) const;

  operator unsigned long long(void);
  BitBoard(void);
  BitBoard(unsigned long long right);

  BitBoard &set(size_t pos, bool val = true);
  BitBoard &reset(size_t pos);
  BitBoard &operator=(unsigned long long &right);
  BitBoard &operator&=(unsigned long long &right);
  BitBoard &operator&=(const BitBoard &right);
  BitBoard &operator|=(unsigned long long &right);
  BitBoard &operator|=(const BitBoard &right);
  BitBoard operator~(void) const;
  BitBoard operator&(const BitBoard &right) const;
  BitBoard operator|(const BitBoard &right) const;
};
