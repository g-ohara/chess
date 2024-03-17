#include "pch.hpp"
#include "RecordList.hpp"

#include "Board.hpp"
#include "Marking.hpp"
#include "Move.hpp"

void RecordList::push_back_record(const Record &record)
{
    try
    {
        size_t pos = _ply - _offset;
        if (pos == _record.size())
            _record.push_back(record);
        else
            _record.at(pos) = record;
    }
    catch (std::exception &ex)
    {
        std::cerr << "Exception in RecordList::push_back_record\n";
        throw;
    }
}

void RecordList::move_piece(Record record)
{
    push_back_record(record);
    ++_ply;
}

void RecordList::unmove_piece(void)
{
    if (--_ply < _offset)
        throw std::range_error(
            "_ply < _offset (in RecordList::unmove_piece)");
}

Record RecordList::get_record(void) const
{
    try
    {
        return _record.at(_ply - _offset - 1);
    }
    catch (const std::exception &)
    {
        std::cerr << "Exception in RecordList::get_record\n";
        throw;
    }
}

void RecordList::init(int ply)
{
    _ply = ply;
    _offset = ply;
    _record.resize(0);
}

void RecordList::output_record(int num) const
{
    // int start = num == 0 ? 0 : std::max(0, _ply - num * 2 + _ply % 2);
    int start = 0;
    std::cout << std::endl;

    for (int i = start; i < _ply - _offset; ++i)
    {
        if (i % 2 == 0)
            std::cout << std::endl
                      << std::right << std::setw(3)
                      << i / 2 + 1 << ". ";
        std::cout << std::left << std::setw(5)
                  << _record.at(i).name << " ";
    }

    std::cout << std::endl
              << std::endl;
}
