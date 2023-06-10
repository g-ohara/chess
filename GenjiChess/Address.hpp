#pragma once
#include "pch.hpp"
#include "BitBoard.hpp"

/**
 * @brief	This class indicates the address on the board with integer (0 - 63)
 *          _index = file * 8 + rank
 */

class Address
{
protected:
    /**
     * @brief indicater to a grid on the board
     */
    size_t _index;

public:
    /**
     * @brief   gets the file index
     * @return  file index
     */
    int get_file(void) const;

    /**
     * @brief   gets the rank index
     * @return  rank index
     */
    int get_rank(void) const;

    /**
     * @brief   sets the file index
     * @param	[in] num
     *          file index ('file a' -> 0, 'file h' -> 7)
     */
    void set_file(int num);

    /**
     * @brief   sets the rank index
     * @param	[in] num
     *          rank index ('rank 1' -> 0, 'rank 8' -> 7)
     */
    void set_rank(int num);

    /**
     * @brief	checks whether the address is valid or not
     * @retval	true	valid (0 <= m_index < 64)
     * @retval	false	invalid (otherwise)
     */
    bool is_valid(void) const;

    Address front(color_t color) const;
    Address back(color_t color) const;
    Address left(color_t color) const;
    Address right(color_t color) const;

    std::string get_str(void);

    Address(void);
    Address(size_t index);
    Address(int file, int rank);

    /**
     * @brief 	constructs a new operator size_t object
     * @note    allows implicit cast
     */
    operator size_t(void) const;

    bool operator==(const Address &right) const;
    bool operator!=(const Address &right) const;
    bool operator<(const Address &right) const;

    Address &operator=(const Address &right);
    Address &operator++(void);
    const Address operator++(int);
};

class LoopAddress : public Address
{
private:
    class BitBoard bb;

public:
    LoopAddress(void);
    LoopAddress(class BitBoard bitboard);
    LoopAddress &operator++(void);

    LoopAddress &ini(BitBoard bitBoard);
};
