#include "Stdafx.h"
#include <fstream>
#include <istream>
// #include "Marking.hpp"

using std::cout;
using std::endl;
DynamicBoard bd;
// OpeningList opList;

Move playersMove(const DynamicBoard &bd, color_t myColor);
// void output_record(const DynamicBoard &bd, const Marking &marker, int currentIndex, int num);

void output_csv_for_ML(void);

// void PLAYERvsCPU(Thinker &thinker, const Marking &marker);

template <typename TYPE>
TYPE stdinput(void);

// // とりあえず適当に定義
// template <>
// int stdinput(void)
// {
// 	int ret;
// 	std::cin >> ret;
// 	return ret;
// }

color_t get_player_color(void)
{
    while (true)
    {
        std::cout << "Your Color (W or B)  : ";

        std::string input;
        std::getline(std::cin, input);

        if (input.empty() || input == "W" || input == "w")
            return WHITE;
        else if (input == "B" || input == "b")
            return BLACK;
    }
}

int get_cpu_level(void)
{
    while (true)
    {
        std::cout << "CPU Level (1 to 100) : ";

        std::string input;
        std::getline(std::cin, input);

        if (input.empty())
        {
            return 10;
        }
        else
        {
            int level;
            try
            {
                level = std::stoi(input);
                if (0 < level && level <= 100)
                    return level;
            }
            catch (const std::invalid_argument &ex)
            {
                continue;
            }
        }
    }
}

/**
 * @brief 	the protocol that CPU plays against itself.
 *
 * @param 	thinker
 * @param 	marker
 */
void CPUvsCPU(Thinker &thinker)
{
    int time = clock(), moveNum = 0;
    for (int turn = 0; turn < 200; ++turn)
    {
        std::vector<MoveLine> move = thinker.thinking(bd, 3);
        switch (move[0][0].value)
        {
        case -INF:
            std::cout << "\nI Resign!\n";
            break;
        default:
            std::cout.clear();
            bd.movement(move[0][0], true);
            ++moveNum;
            bd.output(std::cout);

            if (thinker.inMate(bd))
                break;
            else
                continue;
        }
        break;
    }

    bd.output_record(100);

    if (thinker.inCheckMate(bd))
        std::cout << "Checkmate!" << std::endl;
    else if (thinker.inStaleMate(bd))
        std::cout << "Stalemate!" << std::endl;

    std::cout << "The average of thinking time is "
              << (clock() - time) / moveNum
              << ". Great Game!"
              << std::endl;
    return;
}

/**
 * @brief 	the protocol that human player plays against CPU.
 *
 * @param 	thinker
 * @param 	marker
 */
void PLAYERvsCPU(Thinker &thinker)
{
    color_t playerColor = get_player_color();
    int level = get_cpu_level();

    while (1)
    {
        if (!(playerColor == BLACK && bd.getPly() == 0))
        {
            // outputs the board and the record list
            bd.output(std::cout);
            bd.output_record(10);

            // player's move
            bd.movement(playersMove(bd, playerColor), true);
        }
        std::cout << std::endl;

        // CPU's move
        std::cout << "Now thinking..." << std::endl;

        ComInfo info;
        std::vector<MoveLine> cpuMove;

        cpuMove = thinker.thinking(
            bd, 10, 100 * level, 3, nullptr, &info);

        for (const MoveLine &line : cpuMove)
            line.output(std::cout);

        // beep sound
        std::cout << "\a";

        if ((playerColor == WHITE && cpuMove[0][0].value == INF) ||
            (playerColor == BLACK && cpuMove[0][0].value == -INF))
        {
            // CPU resigns
            std::cout << "\nI resign. Thank you for playing!\n";
            break;
        }
        else
        {
            std::cout
                << "\nI play "
                << cpuMove[0][0].name
                << "." << std::endl
                << "Depth --- " << info.depth << std::endl
                << "Time  --- " << info.time / 1000 << " ms." << std::endl
                << "Value --- " << cpuMove[0][0].value << std::endl;

            if (thinker.inMate(bd))
            {
                if (thinker.inCheckMate(bd))
                    std::cout << "\nCheckmate. I won! Thank you for playing!\n";
                else
                    std::cout << "\nStalemate. Draw! Thank you for playing!\n";
                break;
            }

            bd.movement(cpuMove[0][0], true);
        }
    }
    bd.output(std::cout);
    // output_record(bd, bd.index(), 10);
}

void PLAYERvsPLAYER(Thinker &thinker)
{
    while (1)
    {
        // outputs the board and the record list
        bd.output(std::cout);
        bd.output_record(10);

        // player's move
        bd.movement(playersMove(bd, bd.getPly() % 2), true);
        try
        {
            bd.check_consistency();
        }
        catch (std::exception &ex)
        {
            std::cout << "MOVE\n";
            bd.output(std::cout);
            bd.output_record(10);
            throw;
        }
    }
}

int main(int argc, char *argv[])
{
    Thinker thinker;
    Marker marker;

    std::string opt;
    switch (argc)
    {
    case 1:
        PLAYERvsCPU(thinker);
        break;
    case 2:
        opt = argv[1];
        if (opt == "--cpu-cpu")
        {
            CPUvsCPU(thinker);
            break;
        }
        else if (opt == "--player-player")
        {
            PLAYERvsPLAYER(thinker);
            break;
        }
    default:
        std::cout << "The arguments are invalid." << std::endl;
        return 1;
    }
    // output_csv_for_ML();

    return 0;
}
