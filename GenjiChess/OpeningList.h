// #pragma once

// class OpeningData
// {
// public:

// 	int index = 0;
// 	std::string name = "";
// 	class Address form[40] = { 0 };
// 	class Address latt[40] = { 0 };
// };

// class OpeningList
// {
// private:

// 	class OpeningData opening[200];
// 	int openingCounter = 0;
// 	const std::string FILENAME = "opening.dat";

// 	void fileOutput(void) const;
// 	void add(class OpeningData newOpening);
// 	void randomization(int num[], int size) const;

// public:

// 	int openingIndex = 0;

// 	// inputs the opening list from the set_file
// 	OpeningList(void);

// 	// searches the list and return best move
// 	Record openingSearch(const DynamicBoard& board);

// 	// registers the new opening
// 	// void registraion(void);
// };
