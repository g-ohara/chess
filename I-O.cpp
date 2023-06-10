#include "Stdafx.h"

using std::cin;
using std::cout;
using std::endl;
using std::getline;

template <typename TYPE>
TYPE stdinput(void);

Address address_input(void)
{
	while (true)
	{
		STRING str;
		std::cin >> str;

		if (str.size() == 2)
		{
			Address ret;
			ret.set_file(str[0] - 'a');
			ret.set_rank(str[1] - '1');
			if (ret.is_valid())
				return ret;
		}
		std::cout << "It is invalid input. Try again." << std::endl;
	}
}

// input the player's move
Move playersMove(const DynamicBoard &bd, color_t myColor)
{
	Move answer;
	int usersInput = 0;

	do
	{
		while (true)
		{
			cout << "From: ";
			answer.form = address_input();

			Piece trg = bd.getPiece(answer.form);
			if (trg.exist && trg.color == myColor)
				break;
			else
				cout << "There is not your piece there." << endl;
		}

		BitBoard mark = Marker::AllMove_s(bd, answer.form);

		cout << "  To: ";
		answer.latt = address_input();

		if (!mark.test(answer.latt))
		{
			cout << "It is illegal move." << endl;
			usersInput = 0;
		}
		else
		{
			cout << endl
				 << "You play "
				 << Record::nameMove(bd, answer)
				 << ". Is this OK? (1.OK,2.Cancel)"
				 << endl;

			usersInput = stdinput<int>();

			while ((usersInput < 1) || (usersInput > 2))
			{
				cout << "It is unsuitable. Put in 1 or 2." << endl;
				usersInput = stdinput<int>();
			};
		}
	} while (usersInput != 1);

	if (answer.isPromotion(bd))
	{
		cout << "Promotion! Which piece will you promote your pawn to? (1.Queen,2.Rook,3.Bishop,4.Knight)" << endl;
		usersInput = stdinput<int>();

		while (usersInput < 1 || usersInput > 4)
		{
			cout << "It is unsuitable. Select from 1 to 4" << endl;
			usersInput = stdinput<int>();
		};
	}
	else
	{
		usersInput = KING;
	}

	answer.prmObj = usersInput;

	return answer;
}

// standard input
template <typename TYPE>
TYPE stdinput(void)
{
	TYPE a;
	cin >> a;

	while (cin.fail())
	{
		cout << "\nIt is unsuitable. Follow the instruction.\n";

		cin.clear();
		cin.ignore(1024, '\n');

		cin >> a;
	}
	return a;
}
