
#include <iostream>
#include <string>
#include <vector>
#include<fstream>
#include <sstream>
int load_map(std::ifstream &in, int rows, int columns, std::vector<std::vector<int>> &map){
	map.resize(rows);
	int num{ 0 };
	for (int i = 0; i < rows; i++) {
		map[i].resize(columns);
		for (int j = 0; j < columns; j++) {
			if (!(in >> num)) {
				if (in.eof()) {
					return 1;
				}
			}
			if (num != 0 && num != 1) {
				return 1;
			}
			map[i][j] = num;
		}
	}
	return 0;
}
int verify_steps(std::vector<std::vector<int>>& map, std::vector<int>& steps, int rows, int columns, std::vector<int> &pos) {
	for (auto step : steps) {
		switch (step) {
		case 0:
			pos[1]--;
			break;
		case 1:
			pos[1]++;
			break;
		case 2:
			pos[0]--;
			break;
		case 3:
			pos[0]++;
			break;
		}
		//check if we are still on map
		if (pos[0] < 0 || pos[0] >= rows || pos[1] < 0 || pos[1] >= columns) {
			return 1; //out of map
		}
		//check if we hit the wall 
		if (map[pos[0]][pos[1]] == 0) {
			return 1; //invalid
		}
	}
	return 0; //valid path
}

int read_steps(int input_flag, std::string input_path, std::vector<int> &steps, std::vector<int> &pos, int rows, int columns) {
	std::string buffer;
	std::stringstream ss;
	// input flag is not set so we read from the stdin
	if (input_flag == 0) {
		while (std::getline(std::cin, buffer)) {
			if (buffer == "")
				break;
			std::cout << buffer;
			std::cout << "\n";
			ss << buffer;
			int number{ 0 };
			int count{ 0 };
			while (ss >> number) {
				if (count < 2 && ( (count==0 && number < rows) || (count==1 && number < columns)) ) {
					pos.insert(pos.end(), number);
					count++;
				}
				else if (number == 0 || number == 1 || number == 2 || number == 3) {
					steps.insert(steps.end(),number);
				}
				else {
					return 1; //invalid step
				}
				if (ss.eof()) {
					ss.clear();
					break;
				}
			}
		}
		return 0;
	}
	//input flag is set to 1
	std::ifstream in;
	in.open(input_path);
	while (std::getline(in, buffer)) {
		if (buffer == "")
			break;
		std::cout << buffer;
		std::cout << "\n";
		ss << buffer;
		int number{ 0 };
		int count{ 0 };
		while (ss >> number) {
			if (count < 2 && ((count == 0 && number < rows) || (count == 1 && number < columns))) {
				pos.insert(pos.end(), number);
				count++;
			}
			else if (number == 1 || number == 2 || number == 3 || number == 4) {
				steps.insert(steps.end(), number);
			}
			else {
				return 1; //invalid step
			}
			if (ss.eof()) {
				ss.clear();
				break;
			}
		}
	}
	return 0;
}


int main(int argc, char* argv[])
{
	std::vector<std::string> argum;

	// file path not included
	if (argc == 1) {
		std::cout << "Som hovno";
		return 1;
	}
	std::cout << argc;
	std::cout << '\n';
	// push arguments into vector of strings for easier comparing 
	for (int i = 1; i < argc; i++) {
		argum.push_back(argv[i]);
		std::cout << argv[i];
		std::cout << "\n";
	}
	int flag_verify{ 0 };
	int flag_input{ 0 };
	int flag_output{ 0 };
	std::string input_file{ "" };
	std::string output_file{ "" };
	int n = argc - 1;
	for (int i = 0; i < n; i++) {
		// todo zmenit na switch a dat default ze zly argument
		if (argum[i] == "--verify") {
			flag_verify = 1;
		}
		// we have input file flag and there is something behind this flag (path)
		if (argum[i] == "--input-file" && i + 1 < n) {
			if (i + 1 >= n) {
				return 1;
			}
			flag_input = 1;
			input_file = argum[i + 1];
			i++;
			// check if there is some invalid file path
			if (input_file[0] == '-') {
				std::cout << "Invalid file path to input\n";
				return 1;
			}
		}
		// there is no path after input-file flag so the parameter is invalid

		// we have output file flag
		if (argum[i] == "--output-file" && i + 1 < n) {
			flag_output = 1;
			if (i + 1 >= n) {
				return 1;
			}
			output_file = argum[i + 1];
			i++;
			//check if there is some invalid file path
			if (output_file[0] == '-') {
				std::cout << "Invalid file path to output\n";
				return 1;
			}
			continue;
		}
	}
	if (flag_verify == 0) {
		std::cout << "Flag verify doesn't exist\n";
		return 1;
	}

	std::string map_path = argv[1];
	std::ifstream in;
	in.open(map_path);
	if (!in.is_open()) {
		std::cout << "Cannot open\n";
		return 1;
	}
	std::string trash;
	in >> trash;
	std::cout << trash;
	int rows{ 0 };
	int columns{ 0 };
	in >> rows;
	in >> columns;
	std::cout << "\n";
	std::cout << rows;
	std::cout << "\n";
	std::cout << columns;
	std::vector<std::vector<int>> map;
	// loading map
	if (load_map(in, rows, columns, map) == 1) {
		std::cout << "Map plan incomplete\n";
		return 1;
	}
	std::cout << "Som az tu\n";
	// writing map
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			std::cout << map[i][j];
			std::cout << " ";
		}
		std::cout << "\n";
	}
	// read input
	std::vector<int> steps;
	std::vector<int> position;
	if (read_steps(flag_input, input_file, steps, position, rows, columns) == 1) {
		return 1;
	}
	std::cout << "Steps:\n";
	for (auto step : steps) {
		std::cout << step;
		std::cout << ", ";
	}
	std::cout << "Positions\n";
	for (auto pos : position) {
		std::cout << pos;
		std::cout << ", ";
	}
	int valid_path = verify_steps(map, steps, rows, columns, position);
	if (flag_output == 0) {
		std::cout << valid_path;
	}
	else {
		std::ofstream out;
		out.open(output_file);
		if (!in.is_open()) {
			std::cout << "Cannot open\n";
			return 1;
		}
		out << valid_path;
		out << "\n";
	}
	return 0;
}
