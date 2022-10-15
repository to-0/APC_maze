
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
			if (!(in >> num)) { //input ended but it doesn't match with specified columns and rows
				if (in.eof()) { //smaller input, than required
					return 1;
				}
			}
			if (num != 0 && num != 1) {
				return 1;
			}
			map[i][j] = num;
		}
	}
	num = 0;
	if (in >> num) { //if we read another integer, the input is bigger than required, this should clean any reamining whitespaces if there is no other number
		std::cerr << "Bigger input than defined.\n";
		return 1;
	}
	if (!in.eof()) { //we are still not at the end of file
		return 1;
	}
	return 0;
}
int verify_steps(std::vector<std::vector<int>>& map, std::vector<int>& steps, int rows, int columns, std::vector<int> &pos) {
	if (pos[0] < 0 || pos[0] >= rows || pos[1] < 0 || pos[1] >= columns) {
		return -1; //out of map
	}
	//started in the wall 
	if (map[pos[0]][pos[1]] == 0) {
		return -1; //invalid
	}
	//move
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
			return 0; //out of map, error
		}
		//check if we hit the wall 
		if (map[pos[0]][pos[1]] == 0) {
			return 0; 
		}
	}
	return 1; //valid path
}

int read_steps(std::vector<std::vector<int>> map,int input_flag, std::string input_path, int rows, int columns, int output_flag, std::string output_path) {
	std::string buffer;
	std::vector<int> steps;
	std::vector<int> position;
	int valid_path{ -1 };
	std::ifstream in;
	std::ofstream out;
	//opening files if flags are set
	if (output_flag == 1) {
		out.open(output_path);
		if (!out.is_open()) {
			std::cerr << "Cannot open\n";
			return 1;
		}
	}
	if (input_flag == 1) {
		in.open(input_path);
		if (!in.is_open()) {
			std::cerr << "Cannot open\n";
			return 1;
		}
	}
	// reading either from stdin or ifstream
	while (std::getline((input_flag==0)?std::cin:in, buffer)) {
		if (buffer == "")
			break;
		std::stringstream ss(buffer);
		//ss << buffer;
		int number{ 0 };
		int count{ 0 };
		while (ss >> number) {
			// read first starting positions
			if (count < 2 && ( (count==0 && number < rows && number >=0) || (count==1 && number < columns && number >=0)) ) {
				position.insert(position.end(), number);
				count++;
			}
			// ife we already have positions we read number, check if the input is valid
			else if (number == 0 || number == 1 || number == 2 || number == 3) {
				steps.insert(steps.end(),number);
			}
			else {
				std::cerr << "Invalid step ";
				return 1; //invalid step
			}
		}
		if (!ss.eof()) {
			ss.clear();
			return 1;
		}
		ss.clear();
		if (count < 2) { // no starting positions were supplied
			return 1;
		}
		//verify steps
		valid_path = verify_steps(map, steps, rows, columns, position);
		//if there was an error
		if (valid_path == -1) {
			return 1; //error
		}
		else {
			if (output_flag == 0) {
				std::cout << valid_path;
				std::cout << '\n';
			}
			else {
				out << valid_path;
				out << "\n";
			}
		}
		steps.clear();
		position.clear();
	}
	return 0;
}


int main(int argc, char* argv[])
{
	std::vector<std::string> argum;

	// file path not included
	if (argc == 1) {
		return 1;
	}
	// push arguments into vector of strings for easier comparing 
	for (int i = 1; i < argc; i++) {
		argum.push_back(argv[i]);
	}
	// flags for arguments
	int flag_verify{ 0 };
	int flag_input{ 0 };
	int flag_output{ 0 };
	std::string input_file{ "" };
	std::string output_file{ "" };
	auto n = argum.size();
	//read arguments
	for (unsigned int i = 1; i < n; i++) {
		if (argum[i] == "--verify") {
			if (flag_verify == 1) {
				return 1; //multiple arguments
			}
			flag_verify = 1;
			continue;
		}
		// we have input file flag and there is something behind this flag (path)
		if (argum[i] == "--input-file" && i + 1 < n) {
			if (i + 1 >= n) {
				return 1;
			}
			if (flag_input == 1) {
				return 1; //multiple argument flag_input
			}
			flag_input = 1;
			input_file = argum[i + 1];
			i++;
			// check if there is some invalid file path
			if (input_file[0] == '-') {
				std::cerr << "Invalid file path to input\n";
				return 1;
			}
			continue;
		}
		// there is no path after input-file flag so the parameter is invalid

		// we have output file flag
		if (argum[i] == "--output-file" && i + 1 < n) {
			if (flag_output) {
				return 1; // multiple arguments
			}
			flag_output = 1;
			if (i + 1 >= n) {
				return 1;
			}
			output_file = argum[i + 1];
			i++;
			//check if there is some invalid file path
			if (output_file[0] == '-') {
				std::cerr << "Invalid file path to output\n";
				return 1;
			}
			continue;
		}
		if (argum[i] != "--output-file" && argum[i] != "--input-file" && argum[i] != "--verify") {
			std::cerr << "Invalid arguments";
			return 1;

		}
	}
	if (flag_verify == 0) {
		std::cerr << "Argument verify doesn't exist\n";
		return 1;
	}
	std::string map_path = argv[1];
	std::ifstream in;
	in.open(map_path);
	if (!in.is_open()) {
		std::cerr << "Cannot open\n";
		return 1;
	}
	std::string trash;
	in >> trash;
	if (trash != "P1") {
		return 1;
	}
	int rows{ 0 };
	int columns{ 0 };
	in >> rows;
	in >> columns;
	std::vector<std::vector<int>> map;
	if (load_map(in, rows, columns, map) == 1) {
		return 1;
	}
	// read input steps
	if (read_steps(map,flag_input, input_file,rows, columns,flag_output,output_file) == 1) {
		return 1; // error
	}
	return 0;
}
