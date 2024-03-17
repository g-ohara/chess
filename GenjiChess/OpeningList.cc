// #include "pch.hpp"
// #include "Board.hpp"
// #include "Move.hpp"
// #include "OpeningList.hpp"

// template <typename TYPE>
// TYPE stdinput(void);

// using std::cin;
// using std::cout;
// using std::endl;
// using std::getline;
// using std::ifstream;
// using std::ofstream;
// using std::string;

// // output the opening list to the set_file
// void OpeningList::fileOutput(void) const
// {

// 	std::ofstream stream(FILENAME);

// 	if (stream)
// 	{

// 		for (int num = 0; num < 200; num++)
// 		{

// 			if (opening[num].index == 0)
// 			{

// 				// finish
// 				break;
// 			}

// 			// output the index and the name of the opening
// 			stream << opening[num].index << " " << opening[num].name << endl;

// 			for (int flag = 0; flag < 40; flag++)
// 			{

// 				// output the address
// 				// stream << opening[num].form[flag].x() << " " << opening[num].form[flag].y() << " " << opening[num].latt[flag].x() << " " << opening[num].latt[flag].y() << " ";

// 				if (opening[num].form[flag].get_file() == 8)
// 				{

// 					// change a line
// 					stream << endl;
// 					break;
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{

// 		// output the error message
// 		cout << "I cannot find the opening file.";
// 	}
// }

// // add the new opening line
// void OpeningList::add(class OpeningData newOpening)
// {

// 	for (int num = 199; num > 0; num--)
// 	{

// 		if (opening[num - 1].index > 0)
// 		{

// 			if (opening[num - 1].index < newOpening.index)
// 			{

// 				// add the new opening
// 				opening[num].index = newOpening.index;
// 				opening[num].name = newOpening.name;

// 				for (int flag = 0; flag < 40; flag++)
// 				{

// 					opening[num].form[flag] = newOpening.form[flag];
// 					opening[num].latt[flag] = newOpening.latt[flag];
// 				}

// 				break;
// 			}
// 			else
// 			{

// 				// push openings back
// 				opening[num].index = opening[num - 1].index;
// 				opening[num].name = opening[num - 1].name;

// 				for (int flag = 0; flag < 40; flag++)
// 				{

// 					opening[num].form[flag] = opening[num - 1].form[flag];
// 					opening[num].latt[flag] = opening[num - 1].latt[flag];
// 				}
// 			}
// 		}
// 	}

// 	return;
// }

// // make random natural numbers
// void OpeningList::randomization(int num[], int size) const
// {

// 	// declare variables
// 	int loop1 = 0, loop2 = 0, rnd = 0;

// 	// initialize num[]
// 	for (loop1 = 0; loop1 < size; loop1++)
// 	{

// 		num[loop1] = -1;
// 	}

// 	srand((unsigned)time(NULL));

// 	for (loop1 = 0; loop1 < size; loop1++)
// 	{

// 		rnd = rand() % (size - loop1);

// 		for (loop2 = 0; loop2 <= rnd; loop2++)
// 		{

// 			if (num[loop2] != -1)
// 			{

// 				// skip
// 				rnd++;
// 			}
// 		}

// 		num[loop2 - 1] = loop1;
// 	}
// }

// // ��Ճf�[�^�x�[�X�ɂ��v�l
// Record OpeningList::openingSearch(const DynamicBoard &board)
// {

// 	Record re;

// 	int num = 0;
// 	int flag = 0;
// 	int bookMark = 0;
// 	int counter = 0;
// 	int rnd[10] = {0};

// 	randomization(rnd, 10);

// 	for (int loop = 0; loop < 10; loop++)
// 	{

// 		for (num = 0; num < 200; num++)
// 		{

// 			if (opening[num].index == (rnd[loop] + 1) + bookMark)
// 			{

// 				// ��Ղ̌���
// 				break;
// 			}
// 		}

// 		for (flag = 0; flag < 40; flag++)
// 		{

// 			if (num == 200)
// 			{

// 				// �Y�������ՂȂ�
// 				break;
// 			}

// 			counter++;

// 			if (opening[num].form[flag].get_file() == 8)
// 			{

// 				// ��Ֆ��[�ɓ��B
// 				counter--;
// 				flag = -1;
// 				break;
// 			}

// 			if (counter == board.getPly())
// 			{

// 				// ����ɓ��B
// 				break;
// 			}

// 			if (!(opening[num].form[flag].get_file() == board.RecordList::at(counter).form.get_file() &&
// 				  opening[num].form[flag].get_rank() == board.RecordList::at(counter).form.get_rank() &&
// 				  opening[num].latt[flag].get_file() == board.RecordList::at(counter).latt.get_file() &&
// 				  opening[num].latt[flag].get_rank() == board.RecordList::at(counter).latt.get_rank()))
// 			{

// 				// ��Ղ̕s��v
// 				counter--;
// 				break;
// 			}
// 		}

// 		if (flag == -1)
// 		{

// 			// �q�f�[�^�ւ̃W�����v
// 			bookMark = (rnd[loop] + 1 + bookMark) * 10;
// 			loop = -1;

// 			// �����Đ���
// 			randomization(rnd, 10);
// 		}
// 		else
// 		{

// 			if (counter == board.getPly())
// 			{

// 				// ����̕ԋp
// 				re.form = opening[num].form[flag];
// 				re.latt = opening[num].latt[flag];
// 				re.value = 0;

// 				// ����̎�ނ̎擾
// 				// Marking marker;
// 				// marker.marking(board, re.form);
// 				// re.mode = marker.mark(re.latt);

// 				openingIndex = num;

// 				// ���Ռp��
// 				return re;
// 			}
// 		}
// 	}

// 	// ���ՏI��
// 	re.value = TIMEUP;
// 	return re;
// }

// /*
// //register the new opning
// void OpeningList::registraion(void) {

// 	class DynamicBoard bd;
// 	class Move openingMove;
// 	class playerMove move;
// 	class openingData openingMemory;
// 	int flag;
// 	int counter;
// 	int bookMark = 0;
// 	int answer;

// 	while (1) {

// 		//initialize the counter
// 		counter = 0;

// 		//search and output the openings that can be accessed
// 		for (int num = 1; num <= 10; num++) {

// 			for (int loop = 0; loop < 200; loop++) {

// 				if (opening[loop].index == num + bookMark) {

// 					//found the child data
// 					counter = num + bookMark;

// 					//output the name of the opening
// 					cout << endl << num << "." << opening[loop].name;

// 					break;
// 				}
// 			}
// 		}

// 		if (counter == 0) {

// 			//register the index of the new opening
// 			openingMemory.index = bookMark + 1;

// 			//start register
// 			break;
// 		}
// 		else {

// 			while (1) {

// 				//output the message
// 				cout << "\n\nWhich does your new opening follow? (If you start from here,input '0'.)\n";

// 				//input the user's answer
// 				cin >> answer;

// 				for (flag = 0; flag < 200; flag++) {

// 					if (opening[flag].index == bookMark + answer) {

// 						//found the according opening
// 						bookMark += answer;
// 						break;
// 					}
// 				}

// 				if (answer == 0 || flag < 200) {

// 					//valid answer
// 					break;
// 				}

// 				//not valid answer
// 				cout << "It is unsuitable.\n";
// 			}

// 			if (answer == 0) {

// 				//register the index of new opening
// 				openingMemory.index = counter + 1;

// 				//start register
// 				break;
// 			}

// 			for (int loop = 0; loop < 40; loop++) {

// 				if (opening[flag].form[loop].x() == 8) {

// 					//reach the end of the current opening
// 					bookMark *= 10;
// 					break;
// 				}
// 				else {

// 					//substract the information of the openingMove in the opening into 'openingMove'
// 					bd.marker.marking(bd, opening[flag].form[loop]);
// 					openingMove.mode = bd.marker.mark(opening[flag].latt[loop]);
// 					openingMove.form = opening[flag].form[loop];
// 					openingMove.latt = opening[flag].latt[loop];

// 					//record and openingMove along the opening
// 					bd.movement(openingMove);
// 				}
// 			}
// 		}
// 	}

// 	//input the name of the opening
// 	cout << "\nInput the name of the opening.\n";
// 	std::getline(cin, openingMemory.name);
// 	std::getline(cin, openingMemory.name);

// 	//initialize the counter
// 	counter = 0;

// 	while (1) {

// 		counter++;

// 		//outputs the board and record
// 		bd.output();

// 		//input the openingMove
// 		move.input(bd, (bd.index() % 2) ? Color::white : Color::black);

// 		//recoard and openingMove
// 		bd.movement(move);

// 		//memorize the openingMove
// 		openingMemory.form[openingCounter] = move.form;
// 		openingMemory.latt[openingCounter] = move.latt;
// 		openingCounter++;

// 		//outputs the board and record
// 		bd.output();

// 		//input whether register will be continued or not
// 		cout << "\nDo you continue? (1.continue, 2.finish)";
// 		answer = stdinput<int>();

// 		while (answer < 1 || answer>2) {

// 			cout << "\nIt is unsuitable. Put in 1 or 2.\n";
// 			answer = stdinput<int>();
// 		}

// 		if (answer == 1) {

// 			continue;
// 		}
// 		else if (answer == 2) {

// 			//finish register
// 			break;
// 		}
// 	}

// 	//the end of the opening
// 	openingMemory.form[counter] = { 8,8 };
// 	openingMemory.latt[counter] = { 8,8 };

// 	//add the new opening to the list
// 	add(openingMemory);

// 	//output the new list into the set_file
// 	fileOutput();

// 	cout << "The register has successed.";

// 	return;
// }
// */
// // input the opening list from the set_file
// OpeningList::OpeningList(void)
// {

// 	ifstream stream(FILENAME);

// 	if (stream)
// 	{

// 		for (int num = 0; num < 200; num++)
// 		{

// 			// input the index of the opening
// 			stream >> opening[num].index;

// 			// input the name of the opening
// 			std::getline(stream, opening[num].name);

// 			if (opening[num].name[0] == '\0')
// 			{

// 				// finish
// 				break;
// 			}

// 			for (int flag = 0; flag < 40; flag++)
// 			{

// 				// input the address
// 				// stream >> opening[num].form[flag].x() >> opening[num].form[flag].y() >> opening[num].latt[flag].x() >> opening[num].latt[flag].y;

// 				if (opening[num].form[flag].get_file() == 8)
// 				{

// 					// openingMove to the next opening
// 					break;
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{

// 		// output the error message
// 		cout << "I cannot find the opening file.";
// 	}

// 	return;
// }
