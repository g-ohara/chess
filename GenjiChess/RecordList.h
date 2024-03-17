#pragma once
#include "pch.h"
#include "Record.h"

class RecordList {
 private:
  std::vector<Record> _record;
  int _ply;
  int _offset;

  void push_back_record(const Record &record);

 public:
  /**
   * @brief   gets the newest record
   * @return  the newest record
   */
  Record get_record(void) const;

  /**
   * @brief   records a move
   * @param   [in] move   a move to be recorded
   * @details This function receives a move
   *          and inserts or pushes it back into vector.
   */
  void move_piece(Record record);

  /**
   * @brief   decriments _ply
   */
  void unmove_piece(void);

  /**
   * @brief   initializes _ply and _offset by the argument
   *          and resizes _record_list
   * @param   [in] ply the initial ply
   */
  void init(int ply);

  /**
   * @brief   outputs records in _record_list
   * @param   [in] num the number of printed records
   */
  void output_record(int num) const;
};
