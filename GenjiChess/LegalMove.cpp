#include "pch.hpp"
#include "Marking.hpp"

#include "LegalMove.hpp"

LegalMove &LegalMove::operator++(void)
{
    if ((++m_latt).is_valid())
    {
        form = m_form;
        latt = m_latt;
        return *this;
    }
    else
    {
        while ((++m_form).is_valid())
        {
            if (m_latt.ini(Marker::ModeMove(m_board, m_form, mode)).is_valid())
            {
                form = m_form;
                latt = m_latt;
                return *this;
            }
        }

        while ((++mode) < 2)
            if (set_first_move())
                return *this;

        form = {8, 8};
        latt = {8, 8};
        return *this;
    }
}

LegalMove::LegalMove(const StaticBoard &board)
    : m_form{}, m_latt{}, myMap{board.getBBF().color_map[board.getPly() % 2]},
      m_board{board}, m_killerMove{}
{
    for (mode = 0; mode < 2; ++mode)
        if (set_first_move())
            return;
}

LegalMove::LegalMove(
    const StaticBoard &board, const std::queue<Move> &killerMove)
    : m_form{}, m_latt{}, myMap{board.getBBF().color_map[board.getPly() % 2]},
      m_board{board}, m_killerMove{killerMove}
{
    while (!m_killerMove.empty())
    {
        Move move = m_killerMove.front();
        m_killerMove.pop();
        if (Marker::MoveIsValid(board, move))
        {
            form = move.form;
            latt = move.latt;
            return;
        }
    }

    for (mode = 0; mode < 2; ++mode)
        if (set_first_move())
            return;
}

bool LegalMove::set_first_move(void)
{
    for (m_form.ini(myMap); m_form.is_valid(); ++m_form)
    {
        if (m_latt.ini(Marker::ModeMove(m_board, m_form, mode)).is_valid())
        {
            form = m_form;
            latt = m_latt;
            return true;
        }
    }
    return false;
}

bool LegalMove::is_defined(void)
{
    return form.is_valid() && latt.is_valid();
}
