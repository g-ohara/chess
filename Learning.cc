#include "Stdafx.h"

void print_header(std::ofstream ofs[3])
{
    for (int i = 0; i < 3; ++i)
    {
        for (name_t name = KING; name <= PAWN; ++name)
            for (Address ad = 0; ad.is_valid(); ++ad)
                ofs[i] << Piece::initial(Piece{WHITE, name, true})
                       << "-" << ad.get_str() << ",";

        ofs[i] << "color,"
               << "cas_king, cas_queen,"
               << "ply,value"
               << std::endl;
    }
}

void print_features(
    std::ofstream &ofs,
    DynamicBoard &board)
{
    // static Thinker thinker;
    // MoveLine line = thinker.thinking(board, 1).at(0);
    // for (int j = 0; j < line.size(); ++j)
    //     board.movement(line[j]);

    Feature feature = Evaluator::get_feature(board); // -> Feature
    for (int j = 0; j < feature.rows(); ++j)         // -> CSV form
        ofs << feature(j) << ",";
}

void print_value(std::ofstream &ofs, std::string val_str)
{
    int val;
    try
    {
        val = stoi(val_str);
    }
    catch (std::exception &ex)
    {
        // std::cout << "Can't read a character \'"
        //           << val_str << "\'" << std::endl;
        val = 23;
    }
    ofs << val << std::endl;
}

void print_data(std::ofstream ofs[3], std::string fen_code, std::string val_str)
{
    DynamicBoard l_board(fen_code); // FEN code -> DynamicBoard
    int count = l_board.getBBF().count_piece();
    int idx;
    if (count > 20)
        idx = 0;
    else if (count > 10)
        idx = 1;
    else
        idx = 2;
    print_features(ofs[idx], l_board);
    print_value(ofs[idx], val_str);
}

void print_progress_bar(int percent, int bar_length)
{
    std::string progress;
    for (int j = 0; j < bar_length; ++j)
        if (j < bar_length * percent / 100)
            progress += "#";
        else
            progress += " ";

    // 半角スペースを10個、左詰めで'#'を上書きする
    // さらにスペースを7個、右詰めでパーセンテージを表示
    std::cout << "["
              << "\e[1;33;47m"
              << std::setfill(' ') << std::setw(10) << std::left
              << progress
              << "\e[0m"
              << "]"
              << std::setfill(' ') << std::setw(7) << std::right
              << percent << "%"
              << std::flush;

    if (percent == 100)
        std::cout << std::endl;
    else
        std::cout << '\r';
}

void output_csv_for_ML(void)
{
    std::cout << "Now Processing Data ...\n";

    std::ifstream ifs("Data/chessData.csv");
    std::ofstream ofs[3];
    ofs[0] = std::ofstream("Data/data-opening.csv");
    ofs[1] = std::ofstream("Data/data-midgame.csv");
    ofs[2] = std::ofstream("Data/data-endgame.csv");
    std::string str_buf;

    getline(ifs, str_buf); // not reads the first line
    print_header(ofs);

    const int iter_num = 500000;
    const int print_freq = iter_num / 100;
    for (int i = 0; i < iter_num; ++i)
    {
        getline(ifs, str_buf);                // std::string
        std::istringstream i_stream(str_buf); // -> std::istringstream

        std::string fen_code; // FEN code
        std::string val_str;  // value
        getline(i_stream, fen_code, ',');
        getline(i_stream, val_str, ',');
        if (val_str.at(0) == '#') // not uses checkmate board
        {
            --i;
            continue;
        }
        print_data(ofs, fen_code, val_str);

        if ((i + 1) % print_freq == 0)
            print_progress_bar((i + 1) * 100 / iter_num, 30);
    }
}