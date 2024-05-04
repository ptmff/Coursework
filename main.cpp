#include <iostream>
#include "termcolor/termcolor.hpp"
#include <vector>
#include <algorithm>
#include <conio.h>
#include <fstream>

using namespace std;

string command = "";
string input = "";

std::vector<std::string> split(std::string str, char delimiter) {
	std::string tmp{""};
	std::vector<std::string> vec;
	for(int i{0}; i < str.length(); ++i) {
		if(str[i] == delimiter) { vec.push_back(tmp); tmp = ""; continue; }
		tmp += str[i];
	}
	vec.push_back(tmp);
	return vec;
}

class Error {
public:
	Error(string message): message{message}{}
	string get_message() const {return message;}
private:
	string message;
};

class Sensor {
protected:
	// true: sensor is activated
	// false: sensor is not activated
	bool state;

public:
	Sensor(){
		state = true;
	}
	
	void set_state(bool state){
		this->state = state;
	}

	virtual bool get_state(){
		return state;
	}
};

class Drive {
private:
	// true: driver is activated;
	// false: driver is not activated
	bool state;
public:
	Drive(){
		state = true;
	}

	void activate(){
		state = true;
	}

	void deactivate(){
		state = false;
	}

	bool get_state(){
		if (!state) throw Error{"Driver is inactive\n\n"};
		return state;
	}

	bool get_state_noerr(){
		return state;
	}
};

class Line_sensor : public Sensor {
private:
	Drive* line;

public:
	Line_sensor(Drive* line) : Sensor() {
		this->line = line;
	}

	bool get_state(){
		if(!line->get_state_noerr()) throw Error{"Line is inactive\n\n"};
		if(!state) throw Error{"Sensor is inactive\n\n"};
		return state;
	};
};

class Container {
protected:
	int amount;
	int space;
	Sensor* sensor;
	Drive* drive;

public:
	virtual void put(int num){
		if (!sensor->get_state()) throw Error{"Container's sensor is inactive\n\n"};
		if (!drive->get_state_noerr()) throw Error{"Container's driver is inactive\n\n"};
		amount += num;
		space -= num;
		if (space < 0) throw Error{"Container hasn't got enough space to put stated amount in it\n\n"};
	}

	void take(){
		if (!sensor->get_state()) throw Error{"Container's sensor is inactive\n\n"};
		if (!drive->get_state_noerr()) throw Error{"Container's driver is inactive\n\n"};
		amount -= 1;
		space += 1;
		if (amount < 0) throw Error{"Container hasn't got enough items to give out\n\n"};
	}

	Container(Sensor* sensor, Drive* drive){
		this->sensor = sensor;
		this->drive = drive;
		amount = 0;
		space = 100;
	}

	void set_space(int space){
		this->space = space;
	}
};

class InOut_container : public Container {
private:
	Drive* drive_in;

public:
	InOut_container(Sensor* sensor, Drive* drive, Drive* drive_in) : Container(sensor, drive){
		this->drive_in = drive_in;
	}

	void put(int num){
		if (!sensor->get_state()) throw Error{"Container's sensor is inactive\n\n"};
		if (!drive_in->get_state_noerr()) throw Error{"Container's driver is inactive\n\n"};
		amount += num;
		space -= num;
		if (space < 0) throw Error{"Container hasn't got enough space to put stated amount in it\n\n"};
	}
};

class Model {
private:
	// modes: a(auto) / m(manual)
	char mode;

public:
	Model(char mode){
		this->mode = mode;
	}

	Sensor sensors[6]{Sensor(), Sensor(), Sensor(), Sensor(), Sensor(), Sensor()};
	Drive drivers[10]{Drive(), Drive(), Drive(), Drive(), Drive(), Drive(), Drive(), Drive(), Drive(), Drive()};
	Container containers[5]{Container(&sensors[0], &drivers[0]), Container(&sensors[1], &drivers[4]), Container(&sensors[2], &drivers[5]), Container(&sensors[3], &drivers[7]), Container(&sensors[4], &drivers[9])};
	InOut_container inout_containers[1]{InOut_container(&sensors[5], &drivers[3], &drivers[2])};
	Drive lines[2]{Drive(), Drive()};
	Line_sensor line_sensors[8]{Line_sensor(&lines[0]), Line_sensor(&lines[0]), Line_sensor(&lines[1]), Line_sensor(&lines[1]), Line_sensor(&lines[1]), Line_sensor(&lines[1]), Line_sensor(&lines[1]), Line_sensor(&lines[1])};

	void change_mode(char mode){
		this->mode = mode;
	}

	char get_mode() {
		return mode;
	}

	void change_state(char devise, int index){

	}

	void change_amount(int index, char action, int amount){

	}
};

class Controller {
public:
	Model* machine;

	void configure(ifstream &in){
		bool data;
		in >> data;
		if (data) machine->lines[0].activate();
		else machine->lines[0].deactivate();
		in >> data;
		if (data) machine->lines[1].activate();
		else machine->lines[1].deactivate();
		in >> data;
		machine->sensors[0].set_state(data);
		in >> data;
		if (data) machine->drivers[0].activate();
		else machine->drivers[0].deactivate();
		in >> data;
		machine->line_sensors[0].set_state(data);
		in >> data;
		if (data) machine->drivers[1].activate();
		else machine->drivers[1].deactivate();
		in >> data;
		machine->line_sensors[1].set_state(data);
		in >> data;
		if (data) machine->drivers[2].activate();
		else machine->drivers[2].deactivate();
		in >> data;
		machine->sensors[1].set_state(data);
		in >> data;
		if (data) machine->drivers[4].activate();
		else machine->drivers[4].deactivate();
		in >> data;
		machine->line_sensors[2].set_state(data);
		in >> data;
		machine->sensors[5].set_state(data);
		in >> data;
		if (data) machine->drivers[3].activate();
		else machine->drivers[3].deactivate();
		in >> data;
		machine->line_sensors[3].set_state(data);
		in >> data;
		machine->sensors[2].set_state(data);
		in >> data;
		if (data) machine->drivers[5].activate();
		else machine->drivers[5].deactivate();
		in >> data;
		machine->line_sensors[4].set_state(data);
		in >> data;
		if (data) machine->drivers[6].activate();
		else machine->drivers[6].deactivate();
		in >> data;
		machine->line_sensors[5].set_state(data);
		in >> data;
		machine->sensors[3].set_state(data);
		in >> data;
		if (data) machine->drivers[7].activate();
		else machine->drivers[7].deactivate();
		in >> data;
		machine->line_sensors[6].set_state(data);
		in >> data;
		if (data) machine->drivers[8].activate();
		else machine->drivers[8].deactivate();
		in >> data;
		machine->line_sensors[7].set_state(data);
		in >> data;
		machine->sensors[4].set_state(data);
		in >> data;
		if (data) machine->drivers[9].activate();
		else machine->drivers[9].deactivate();
	}

	void start(){
		try {
			if (machine->get_mode() == 'a') {
				cout << "                     |-------|" << "\n" << "                     |       |" << "\n" << " "<<termcolor::green<<"________"<<termcolor::white<<"         __  \\_____/  __" << "\n" << ""<<termcolor::green<<"|        |"<<termcolor::white<<"       |  |    |    |  |" << "\n" << ""<<termcolor::green<<"|        |"<<termcolor::white<<"       |  |    |    |  |" << "\n" << termcolor::green <<"|   1    |" << termcolor::white<< "       |  |    |    |  |" << "\n" << termcolor::green<<"|________|"<<termcolor::white<<"       |__|    |    |__|" << "\n" << termcolor::green<<" \\______ "<<termcolor::white<<"         ::    ###    ::        ___" << "\n" << "                  ::           ::    _______\\" << "\n" << " __________________________________ |        |" << "\n" << "/                                  \\|        |" << "\n" << "\\__________________________________/|   2    |                         |-------|                      |-------|" << "\n" << "                                    |________|                         |       |                      |       |" << "\n" << " ________                  __        \\______      __    ________    __  \\_____/  __    ________    __  \\_____/  __" << "\n" << "|        |                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |" << "\n" << "|        |                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |" << "\n" << "|   3    |                |  |                   |  |  |   4    |  |  |    |    |  |  |   5    |  |  |    |    |  |" << "\n" << "|________|                |__|                   |__|  |________|  |__|    |    |__|  |________|  |__|    |    |__|" << "\n" << " \\______                   ::                     ::    \\______     ::    ###    ::    \\______     ::    ###    ::  ______" << "\n" << "                           ::                     ::                ::           ::                ::           :: _______\\" << "\n" << " _________________________________________________________________________________________________________________|        |" << "\n" << "/                                                                                                                 |        |" << "\n" << "\\_________________________________________________________________________________________________________________|   6    |" << "\n" << "                                                                                                                  |________|" << "\n";
				cout << "\nConfigure containers:\n\n";
				cout << "    Container #1:\n";
				cout << "        Amount of free space:  ";
				cin >> input;
				machine->containers[0].set_space(stoi(input));
				cout << "        How many objects should be put inside:  ";
				cin >> input;
				machine->containers[0].put(stoi(input));
				system("cls");

				cout << "                     |-------|" << "\n" << "                     |       |" << "\n" << " ________         __  \\_____/  __" << "\n" << "|        |       |  |    |    |  |" << "\n" << "|        |       |  |    |    |  |" << "\n" << "|   1    |       |  |    |    |  |" << "\n" << "|________|       |__|    |    |__|" << "\n" << " \\______          ::    ###    ::        "<<termcolor::green<<"___"<< termcolor::white << "\n" << "                  ::           ::    "<<termcolor::green<<"_______\\"<< termcolor::white << "\n" << " __________________________________ "<<termcolor::green<<"|        |"<< termcolor::white << "\n" << "/                                  \\"<<termcolor::green<<"|        |"<< termcolor::white << "\n" << "\\__________________________________/"<<termcolor::green<<"|   2    |"<< termcolor::white <<"                         |-------|                      |-------|" << "\n" << "                                   "<<termcolor::green<<" |________| "<<termcolor::white<<"                        |       |                      |       |" << "\n" << " ________                  __        "<<termcolor::green<<"\\______  "<<termcolor::white<<"    __    ________    __  \\_____/  __    ________    __  \\_____/  __" << "\n" << "|        |                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |" << "\n" << "|        |                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |" << "\n" << "|   3    |                |  |                   |  |  |   4    |  |  |    |    |  |  |   5    |  |  |    |    |  |" << "\n" << "|________|                |__|                   |__|  |________|  |__|    |    |__|  |________|  |__|    |    |__|" << "\n" << " \\______                   ::                     ::    \\______     ::    ###    ::    \\______     ::    ###    ::  ______" << "\n" << "                           ::                     ::                ::           ::                ::           :: _______\\" << "\n" << " _________________________________________________________________________________________________________________|        |" << "\n" << "/                                                                                                                 |        |" << "\n" << "\\_________________________________________________________________________________________________________________|   6    |" << "\n" << "                                                                                                                  |________|" << "\n";
				cout << "\nConfigure containers:\n\n";
				cout << "    Container #2:\n";
				cout << "        Amount of free space:  ";
				cin >> input;
				machine->inout_containers[0].set_space(stoi(input));
				system("cls");

				cout << "                     |-------|" << "\n" << "                     |       |" << "\n" << " ________         __  \\_____/  __" << "\n" << "|        |       |  |    |    |  |" << "\n" << "|        |       |  |    |    |  |" << "\n" << "|   1    |       |  |    |    |  |" << "\n" << "|________|       |__|    |    |__|" << "\n" << " \\______          ::    ###    ::        ___" << "\n" << "                  ::           ::    _______\\" << "\n" << " __________________________________ |        |" << "\n" << "/                                  \\|        |" << "\n" << "\\__________________________________/|   2    |                         |-------|                      |-------|" << "\n" << "                                    |________|                         |       |                      |       |" << "\n" << termcolor::green <<" ________"<<termcolor::white<<"                  __        \\______      __    ________    __  \\_____/  __    ________    __  \\_____/  __" << "\n" << termcolor::green<<"|        |"<<termcolor::white<<"                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |" << "\n" << termcolor::green<<"|        |"<<termcolor::white<<"                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |" << "\n" << termcolor::green<<"|   3    | "<<termcolor::white<<"               |  |                   |  |  |   4    |  |  |    |    |  |  |   5    |  |  |    |    |  |" << "\n" <<termcolor::green<< "|________|"<<termcolor::white<<"                |__|                   |__|  |________|  |__|    |    |__|  |________|  |__|    |    |__|" << "\n" << termcolor::green<<" \\______"<<termcolor::white<<"                   ::                     ::    \\______     ::    ###    ::    \\______     ::    ###    ::  ______" << "\n" << "                           ::                     ::                ::           ::                ::           :: _______\\" << "\n" << " _________________________________________________________________________________________________________________|        |" << "\n" << "/                                                                                                                 |        |" << "\n" << "\\_________________________________________________________________________________________________________________|   6    |" << "\n" << "                                                                                                                  |________|" << "\n";
				cout << "\nConfigure containers:\n\n";
				cout << "    Container #3:\n";
				cout << "        Amount of free space:  ";
				cin >> input;
				machine->containers[1].set_space(stoi(input));
				cout << "        How many objects should be put inside:  ";
				cin >> input;
				machine->containers[1].put(stoi(input));
				system("cls");

				cout << "                     |-------|" << "\n" << "                     |       |" << "\n" << " ________         __  \\_____/  __" << "\n" << "|        |       |  |    |    |  |" << "\n" << "|        |       |  |    |    |  |" << "\n" << "|   1    |       |  |    |    |  |" << "\n" << "|________|       |__|    |    |__|" << "\n" << " \\______          ::    ###    ::        ___" << "\n" << "                  ::           ::    _______\\" << "\n" << " __________________________________ |        |" << "\n" << "/                                  \\|        |" << "\n" << "\\__________________________________/|   2    |                         |-------|                      |-------|" << "\n" << "                                    |________|                         |       |                      |       |" << "\n" << " ________                  __        \\______      __   "<<termcolor::green<<" ________"<<termcolor::white<<"    __  \\_____/  __    ________    __  \\_____/  __" << "\n" << "|        |                |  |                   |  | "<<termcolor::green<<" |        |"<<termcolor::white<<"  |  |    |    |  |  |        |  |  |    |    |  |" << "\n" << "|        |                |  |                   |  |  "<<termcolor::green<<"|        |"<<termcolor::white<<"  |  |    |    |  |  |        |  |  |    |    |  |" << "\n" << "|   3    |                |  |                   |  | "<<termcolor::green<<" |   4    |"<<termcolor::white<<"  |  |    |    |  |  |   5    |  |  |    |    |  |" << "\n" << "|________|                |__|                   |__| "<<termcolor::green<<" |________| "<<termcolor::white<<" |__|    |    |__|  |________|  |__|    |    |__|" << "\n" << " \\______                   ::                     ::    "<<termcolor::green<<"\\______"<<termcolor::white<<"     ::    ###    ::    \\______     ::    ###    ::  ______" << "\n" << "                           ::                     ::                ::           ::                ::           :: _______\\" << "\n" << " _________________________________________________________________________________________________________________|        |" << "\n" << "/                                                                                                                 |        |" << "\n" << "\\_________________________________________________________________________________________________________________|   6    |" << "\n" << "                                                                                                                  |________|" << "\n";
				cout << "\nConfigure containers:\n\n";
				cout << "    Container #4:\n";
				cout << "        Amount of free space:  ";
				cin >> input;
				machine->containers[2].set_space(stoi(input));
				cout << "        How many objects should be put inside:  ";
				cin >> input;
				machine->containers[2].put(stoi(input));
				system("cls");

				cout << "                     |-------|" << "\n" << "                     |       |" << "\n" << " ________         __  \\_____/  __" << "\n" << "|        |       |  |    |    |  |" << "\n" << "|        |       |  |    |    |  |" << "\n" << "|   1    |       |  |    |    |  |" << "\n" << "|________|       |__|    |    |__|" << "\n" << " \\______          ::    ###    ::        ___" << "\n" << "                  ::           ::    _______\\" << "\n" << " __________________________________ |        |" << "\n" << "/                                  \\|        |" << "\n" << "\\__________________________________/|   2    |                         |-------|                      |-------|" << "\n" << "                                    |________|                         |       |                      |       |" << "\n" << " ________                  __        \\______      __    ________    __  \\_____/  __    "<<termcolor::green<<"________"<<termcolor::white<<"    __  \\_____/  __" << "\n" << "|        |                |  |                   |  |  |        |  |  |    |    |  |  "<<termcolor::green<<"|        | "<<termcolor::white<<" |  |    |    |  |" << "\n" << "|        |                |  |                   |  |  |        |  |  |    |    |  |  "<<termcolor::green<<"|        |"<<termcolor::white<<"  |  |    |    |  |" << "\n" << "|   3    |                |  |                   |  |  |   4    |  |  |    |    |  |  "<<termcolor::green<<"|   5    | "<<termcolor::white<<" |  |    |    |  |" << "\n" << "|________|                |__|                   |__|  |________|  |__|    |    |__| "<<termcolor::green<<" |________| "<<termcolor::white<<" |__|    |    |__|" << "\n" << " \\______                   ::                     ::    \\______     ::    ###    ::  "<<termcolor::green<<"  \\______ "<<termcolor::white<<"    ::    ###    ::  ______" << "\n" << "                           ::                     ::                ::           ::                ::           :: _______\\" << "\n" << " _________________________________________________________________________________________________________________|        |" << "\n" << "/                                                                                                                 |        |" << "\n" << "\\_________________________________________________________________________________________________________________|   6    |" << "\n" << "                                                                                                                  |________|" << "\n";
				cout << "\nConfigure containers:\n\n";
				cout << "    Container #5:\n";
				cout << "        Amount of free space:  ";
				cin >> input;
				machine->containers[3].set_space(stoi(input));
				cout << "        How many objects should be put inside:  ";
				cin >> input;
				machine->containers[3].put(stoi(input));
				system("cls");

				cout << "                     |-------|" << "\n" << "                     |       |" << "\n" << " ________         __  \\_____/  __" << "\n" << "|        |       |  |    |    |  |" << "\n" << "|        |       |  |    |    |  |" << "\n" << "|   1    |       |  |    |    |  |" << "\n" << "|________|       |__|    |    |__|" << "\n" << " \\______          ::    ###    ::        ___" << "\n" << "                  ::           ::    _______\\" << "\n" << " __________________________________ |        |" << "\n" << "/                                  \\|        |" << "\n" << "\\__________________________________/|   2    |                         |-------|                      |-------|" << "\n" << "                                    |________|                         |       |                      |       |" << "\n" << " ________                  __        \\______      __    ________    __  \\_____/  __    ________    __  \\_____/  __" << "\n" << "|        |                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |" << "\n" << "|        |                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |" << "\n" << "|   3    |                |  |                   |  |  |   4    |  |  |    |    |  |  |   5    |  |  |    |    |  |" << "\n" << "|________|                |__|                   |__|  |________|  |__|    |    |__|  |________|  |__|    |    |__|" << "\n" << " \\______                   ::                     ::    \\______     ::    ###    ::    \\______     ::    ###    :: "<<termcolor::green<<" ______"<< termcolor::white << "\n" << "                           ::                     ::                ::           ::                ::           :: "<<termcolor::green<<"_______\\"<< termcolor::white << "\n" << " _________________________________________________________________________________________________________________"<<termcolor::green<<"|        |"<< termcolor::white << "\n" << "/                                                                                                                 "<<termcolor::green<<"|        |"<< termcolor::white << "\n" << "\\_________________________________________________________________________________________________________________"<<termcolor::green<<"|   6    |"<< termcolor::white << "\n" << "                                                                                                                  "<<termcolor::green<<"|________|"<< termcolor::white << "\n";
				cout << "\nConfigure containers:\n\n";
				cout << "    Container #6:\n";
				cout << "        Amount of free space:  ";
				cin >> input;
				machine->containers[4].set_space(stoi(input));
				system("cls");

				// Активировали линии
				for (auto line: machine->lines) {
					line.activate();
				}

				cout << "\nWould you like to turn something off? (y/n):  ";
				cin >> input;
				if (input == "y" || input == "Y"){
					system("cls");
					string drivers_scheme = "                     |-------|\n                     |   1   |\n ________         __  \\_____/  __\n|        |       |  |    |    |  |\n|        |       |  |    |    |  |\n|        |       |  |    |    |  |\n|________|       |__|    |    |__|\n0\\______          ::    ###    ::        ___\n                  ::           ::    _______\\2\n __________________________________ |        |\n/                                  \\|        | \n\\__________________________________/|        |                         |-------|                      |-------|\n                                    |________|                         |   6   |                      |   8   |\n ________                  __       3\\______      __    ________    __  \\_____/  __    ________    __  \\_____/  __\n|        |                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |\n|        |                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |\n|        |                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |\n|________|                |__|                   |__|  |________|  |__|    |    |__|  |________|  |__|    |    |__|\n4\\______                   ::                     ::   5\\______     ::    ###    ::   7\\______     ::    ###    ::  ______\n                           ::                     ::                ::           ::                ::           :: _______\\9\n _________________________________________________________________________________________________________________|        |\n/                                                                                                                 |        | \n\\_________________________________________________________________________________________________________________|        |\n                                                                                                                  |________|\n";
					vector<string> drivers_scheme_vec;
					drivers_scheme_vec.push_back(drivers_scheme);
					vector <string> temp;

					string lines_scheme =   "                     |-------|\n                     |       |\n ________         __  \\_____/  __\n|        |       |  |    |    |  |\n|        |       |  |    |    |  |\n|        |       |  |    |    |  |\n|________|       |__|    |    |__|\n \\______          ::    ###    ::        ___\n                  ::           ::    _______\\ \n __________________________________ |        |\n/     1                            \\|        | \n\\__________________________________/|        |                         |-------|                      |-------|\n                                    |________|                         |       |                      |       |\n ________                  __        \\______      __    ________    __  \\_____/  __    ________    __  \\_____/  __\n|        |                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |\n|        |                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |\n|        |                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |\n|________|                |__|                   |__|  |________|  |__|    |    |__|  |________|  |__|    |    |__|\n \\______                   ::                     ::    \\______     ::    ###    ::    \\______     ::    ###    ::  ______\n                           ::                     ::                ::           ::                ::           :: _______\\ \n _________________________________________________________________________________________________________________|        |\n/     2                                                                                                           |        | \n\\_________________________________________________________________________________________________________________|        |\n                                                                                                                  |________|\n";
					vector<string> lines_scheme_vec;
					lines_scheme_vec.push_back(lines_scheme);


					string sc_scheme =      "                     |-------|\n                     |       |\n ________         __  \\_____/  __\n|        |       |  |    |    |  |\n|        |0      |  |    |    |  |\n|        |       |  |    |    |  |\n|________|       |__|    |    |__|\n \\______          ::    ###    ::        ___\n                  ::           ::    _______\\ \n __________________________________ |        |\n/                                  \\|        |5\n\\__________________________________/|        |                         |-------|                      |-------|\n                                    |________|                         |       |                      |       |\n ________                  __        \\______      __    ________    __  \\_____/  __    ________    __  \\_____/  __\n|        |                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |\n|        |1               |  |                   |  |  |        |2 |  |    |    |  |  |        |3 |  |    |    |  |\n|        |                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |\n|________|                |__|                   |__|  |________|  |__|    |    |__|  |________|  |__|    |    |__|\n \\______                   ::                     ::    \\______     ::    ###    ::    \\______     ::    ###    ::  ______\n                           ::                     ::                ::           ::                ::           :: _______\\ \n _________________________________________________________________________________________________________________|        |\n/                                                                                                                 |        |4\n\\_________________________________________________________________________________________________________________|        |\n                                                                                                                  |________|\n";
					vector<string> sc_scheme_vec;
					sc_scheme_vec.push_back(sc_scheme);

					string sl_scheme =      "                     |-------|\n                     |       |\n ________         __  \\_____/  __\n|        |       |  |    |    |  |\n|        |       |0 |    |    |1 |\n|        |       |  |    |    |  |\n|________|       |__|    |    |__|\n \\______          ::    ###    ::        ___\n                  ::           ::    _______\\ \n __________________________________ |        |\n/                                  \\|        | \n\\__________________________________/|        |                         |-------|                      |-------|\n                                    |________|                         |       |                      |       |\n ________                  __        \\______      __    ________    __  \\_____/  __    ________    __  \\_____/  __\n|        |                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |\n|        |                |2 |                   |3 |  |        |  |4 |    |    |5 |  |        |  |6 |    |    |7 |\n|        |                |  |                   |  |  |        |  |  |    |    |  |  |        |  |  |    |    |  |\n|________|                |__|                   |__|  |________|  |__|    |    |__|  |________|  |__|    |    |__|\n \\______                   ::                     ::    \\______     ::    ###    ::    \\______     ::    ###    ::  ______\n                           ::                     ::                ::           ::                ::           :: _______\\ \n _________________________________________________________________________________________________________________|        |\n/                                                                                                                 |        | \n\\_________________________________________________________________________________________________________________|        |\n                                                                                                                  |________|\n";
					vector<string> sl_scheme_vec;
					sl_scheme_vec.push_back(sl_scheme);

					while (command != "n" && command != "N"){
						cout << "\nWhat exactly should be turned off? ( d(driver) / l(line) / sc(sensor on a container) / sl(sensor on a line):  ";
						cin >> input;
						if (input == "d"){
							system("cls");
							for (int i = 0; i < drivers_scheme_vec.size(); i++){
								if (i == 0) cout << drivers_scheme_vec[i];
								else cout << termcolor::red << drivers_scheme_vec[i].substr(1,1) << termcolor::white << drivers_scheme_vec[i].substr(2);
							}
							cout << "\nNumber of driver which should be turned off:  ";
							cin >> input;
							machine->drivers[stoi(input)].deactivate();
							temp = split(drivers_scheme, input[0]);
							drivers_scheme_vec.clear();
							for (auto it : temp){
								drivers_scheme_vec.push_back(it);
								drivers_scheme_vec.push_back("R"+input);
							}
							drivers_scheme_vec.pop_back();
							drivers_scheme = "";
							for (auto it: drivers_scheme_vec) {
								drivers_scheme += it;
							}
							temp = split(drivers_scheme, 'R');
							drivers_scheme_vec.clear();
							for (int i = 0; i < temp.size(); i++){
								if(i == 0) drivers_scheme_vec.push_back(temp[i]);
								else drivers_scheme_vec.push_back("R"+temp[i]);
							}
							system("cls");
							cout << "Current drivers scheme:\n\n";
							for (int i = 0; i < drivers_scheme_vec.size(); i++){
								if (i == 0) cout << drivers_scheme_vec[i];
								else cout << termcolor::red << drivers_scheme_vec[i].substr(1,1) << termcolor::white << drivers_scheme_vec[i].substr(2);
							}
							cout << "\nWould you like to turn off something else? (y/n)  ";
							cin >> command;
							system("cls");
						}

						else if (input == "l"){
							system("cls");
							for (int i = 0; i < lines_scheme_vec.size(); i++){
								if (i == 0) cout << lines_scheme_vec[i];
								else cout << termcolor::red << lines_scheme_vec[i].substr(1,1) << termcolor::white << lines_scheme_vec[i].substr(2);
							}
							cout << "\nNumber of line which should be disactivated:  ";
							cin >> input;
							machine->lines[stoi(input)].deactivate();
							temp = split(lines_scheme, input[0]);
							lines_scheme_vec.clear();
							for (auto it : temp){
								lines_scheme_vec.push_back(it);
								lines_scheme_vec.push_back("R"+input);
							}
							lines_scheme_vec.pop_back();
							lines_scheme = "";
							for (auto it: lines_scheme_vec) {
								lines_scheme += it;
							}
							temp = split(lines_scheme, 'R');
							lines_scheme_vec.clear();
							for (int i = 0; i < temp.size(); i++){
								if(i == 0) lines_scheme_vec.push_back(temp[i]);
								else lines_scheme_vec.push_back("R"+temp[i]);
							}
							system("cls");
							cout << "Current lines scheme:\n\n";
							for (int i = 0; i < lines_scheme_vec.size(); i++){
								if (i == 0) cout << lines_scheme_vec[i];
								else cout << termcolor::red << lines_scheme_vec[i].substr(1,1) << termcolor::white << lines_scheme_vec[i].substr(2);
							}
							cout << "\nWould you like to turn off something else? (y/n)  ";
							cin >> command;
							system("cls");
						}

						else if (input == "sc"){
							system("cls");
							for (int i = 0; i < sc_scheme_vec.size(); i++){
								if (i == 0) cout << sc_scheme_vec[i];
								else cout << termcolor::red << sc_scheme_vec[i].substr(1,1) << termcolor::white << sc_scheme_vec[i].substr(2);
							}
							cout << "\nNumber of container's sensor which should be disactivated:  ";
							cin >> input;
							machine->sensors[stoi(input)].set_state(false);
							temp = split(sc_scheme, input[0]);
							sc_scheme_vec.clear();
							for (auto it : temp){
								sc_scheme_vec.push_back(it);
								sc_scheme_vec.push_back("R"+input);
							}
							sc_scheme_vec.pop_back();
							sc_scheme = "";
							for (auto it: sc_scheme_vec) {
								sc_scheme += it;
							}
							temp = split(sc_scheme, 'R');
							sc_scheme_vec.clear();
							for (int i = 0; i < temp.size(); i++){
								if(i == 0) sc_scheme_vec.push_back(temp[i]);
								else sc_scheme_vec.push_back("R"+temp[i]);
							}
							system("cls");
							cout << "Current sensors on container scheme:\n\n";
							for (int i = 0; i < sc_scheme_vec.size(); i++){
								if (i == 0) cout << sc_scheme_vec[i];
								else cout << termcolor::red << sc_scheme_vec[i].substr(1,1) << termcolor::white << sc_scheme_vec[i].substr(2);
							}
							cout << "\nWould you like to turn off something else? (y/n)  ";
							cin >> command;
							system("cls");
						}

						else if (input == "sl"){
							system("cls");
							for (int i = 0; i < sl_scheme_vec.size(); i++){
								if (i == 0) cout << sl_scheme_vec[i];
								else cout << termcolor::red << sl_scheme_vec[i].substr(1,1) << termcolor::white << sl_scheme_vec[i].substr(2);
							}
							cout << "\nNumber of line sensor which should be disactivated:  ";
							cin >> input;
							machine->sensors[stoi(input)].set_state(false);
							temp = split(sl_scheme, input[0]);
							sl_scheme_vec.clear();
							for (auto it : temp){
								sl_scheme_vec.push_back(it);
								sl_scheme_vec.push_back("R"+input);
							}
							sl_scheme_vec.pop_back();
							sl_scheme = "";
							for (auto it: sl_scheme_vec) {
								sl_scheme += it;
							}
							temp = split(sl_scheme, 'R');
							sl_scheme_vec.clear();
							for (int i = 0; i < temp.size(); i++){
								if(i == 0) sl_scheme_vec.push_back(temp[i]);
								else sl_scheme_vec.push_back("R"+temp[i]);
							}
							system("cls");
							cout << "Current line sensors scheme:\n\n";
							for (int i = 0; i < sl_scheme_vec.size(); i++){
								if (i == 0) cout << sl_scheme_vec[i];
								else cout << termcolor::red << sl_scheme_vec[i].substr(1,1) << termcolor::white << sl_scheme_vec[i].substr(2);
							}
							cout << "\nWould you like to turn off something else? (y/n)  ";
							cin >> command;
							system("cls");
						}
					}
					system("cls");
					string scheme = "";
					for (int i = drivers_scheme_vec.size()-1; i >=0; i--){
						for(int k = drivers_scheme_vec[i].size()-1; k >= 0; k--){
							drivers_scheme+= drivers_scheme_vec[i][k];
						}
					}
					for (int i = lines_scheme_vec.size()-1; i >=0; i--){
						for(int k = lines_scheme_vec[i].size()-1; k >= 0; k--){
							lines_scheme+= lines_scheme_vec[i][k];
						}
					}
					for (int i = sc_scheme_vec.size()-1; i >=0; i--){
						for(int k = sc_scheme_vec[i].size()-1; k >= 0; k--){
							sc_scheme+= sc_scheme_vec[i][k];
						}
					}
					for (int i = sl_scheme_vec.size()-1; i >=0; i--){
						for(int k = sl_scheme_vec[i].size()-1; k >= 0; k--){
							sl_scheme+= sl_scheme_vec[i][k];
						}
					}

					for (int i = 0; i < max(max(drivers_scheme.size(), lines_scheme.size()), max(sc_scheme.size(), sl_scheme.size())); i++){
						if (drivers_scheme[i]=='R' || lines_scheme[i]=='R' || sc_scheme[i]=='R' || sl_scheme[i]=='R'){
							scheme += 'R';
							if (drivers_scheme[i]=='R') {
								lines_scheme = " " + lines_scheme;
								sc_scheme = " " + sc_scheme;
								sl_scheme = " " + sl_scheme;
							}
							else if (lines_scheme[i]=='R') {
								drivers_scheme = " " + drivers_scheme;
								sc_scheme = " " + sc_scheme;
								sl_scheme = " " + sl_scheme;
							}
							else if (sc_scheme[i]=='R') {
								drivers_scheme = " " + drivers_scheme;
								lines_scheme = " " + lines_scheme;
								sl_scheme = " " + sl_scheme;
							}
							else{
								drivers_scheme = " " + drivers_scheme;
								lines_scheme = " " + lines_scheme;
								sc_scheme = " " + sc_scheme;
							}
						}
						else if ('0' <= drivers_scheme[i] && drivers_scheme[i] <= '9' || '0' <= lines_scheme[i] && lines_scheme[i] <= '9' || '0' <= sc_scheme[i] && sc_scheme[i] <= '9' || '0' <= sl_scheme[i] && sl_scheme[i] <= '9'){
							if ('0' <= drivers_scheme[i] && drivers_scheme[i] <= '9') scheme += drivers_scheme[i];
							else if ('0' <= lines_scheme[i] && lines_scheme[i] <= '9') scheme += lines_scheme[i];
							else if ('0' <= sc_scheme[i] && sc_scheme[i] <= '9') scheme += sc_scheme[i];
							else scheme += sl_scheme[i];
						}
						else scheme += sl_scheme[i];
					}
					reverse(scheme.begin(), scheme.end());
					scheme = scheme.substr(0, scheme.size()/2);

					temp = split(scheme, 'R');
					vector<string> scheme_vec;
					for (int i = 0; i < temp.size(); i++){
						if(i == 0) scheme_vec.push_back(temp[i]);
						else scheme_vec.push_back("R"+temp[i]);
					}
					cout << "Final scheme:\n\n";
					for (int i = 0; i < scheme_vec.size(); i++){
						if (i == 0) cout << scheme_vec[i];
						else cout << termcolor::red << scheme_vec[i].substr(1,1) << termcolor::white << scheme_vec[i].substr(2);
					}

				}else system("cls");

				cout << "\nSet system speed (1 / 2 / 3):  ";
				int speed, delay;
				cin >> speed;
				switch (speed) {
					case 1 :
						delay = 1000;
						break;
					case 2 :
						delay = 500;
						break;
					case 3 :
						delay = 0;
						break;
				}
				cout << "\nHow many cycles the programme is supposed to complete:  ";
				int cycles;
				cin >> cycles;

				system("cls");
				cout << "\nProduction may be stopped due to an error or when you press any key. \nIn the second case, attempts will be made to complete the started circle, and only then will the programme stop. \n\nPress any key to start";
				getch();
				system("cls");
				int i = 0;
				// Работаем пока не закончатся заготовки в каком-либо контейнере
				while (!_kbhit()) {
					cout << "__________________________________________________|  Cycle " << ++i << "  |__________________________________________________\n";
					machine->containers[0].take();
					cout << "Q2 driver functioned properly. PCB is on the line" << endl;
					Sleep(delay);
					if (machine->line_sensors[0].get_state()) cout << "I3 sensor was triggered => attempting to run Q3 driver" << endl;
					Sleep(delay);
					if (machine->drivers[1].get_state()) cout << "Q3 driver functioned properly" << endl;
					Sleep(delay);
					if (machine->line_sensors[1].get_state()) cout << "I0 sensor was triggered => attempting to run Q4 driver" << endl;
					Sleep(delay);
					if (machine->drivers[2].get_state()) cout << "Q4 driver functioned properly. The lid of the container with the fabricated PCBs was opened" << endl;
					Sleep(delay);
					machine->inout_containers[0].put(1);
					cout << "PCB is in the container" << endl;
					Sleep(delay);
					machine->containers[1].take();
					cout << "Q6 driver functioned properly. Body is in the box on the line" << endl;
					Sleep(delay);
					if (machine->line_sensors[2].get_state()) cout << "I5 sensor was triggered => attempting to run Q5 driver" << endl;
					Sleep(delay);
					machine->inout_containers[0].take();
					cout << "Q5 driver functioned properly. Fabricated PCB is in the box on the line" << endl;
					Sleep(delay);
					if (machine->line_sensors[3].get_state()) cout << "I6 sensor was triggered => attempting to run Q7 driver" << endl;
					Sleep(delay);
					machine->containers[2].take();
					cout << "Q7 driver functioned properly. Display is in the box on the line" << endl;
					Sleep(delay);
					if (machine->line_sensors[4].get_state()) cout << "I8 sensor was triggered => attempting to run Q8 driver" << endl;
					Sleep(delay);
					if (machine->drivers[6].get_state()) cout << "Q8 driver functioned properly" << endl;
					Sleep(delay);
					if (machine->line_sensors[5].get_state()) cout << "I9 sensor was triggered => attempting to run Q9 driver" << endl;
					Sleep(delay);
					machine->containers[3].take();
					cout << "Q9 driver functioned properly. Manual and warranty card is in the box on the line" << endl;
					Sleep(delay);
					if (machine->line_sensors[6].get_state()) cout << "I11 sensor was triggered => attempting to run Q10 driver" << endl;
					Sleep(delay);
					if (machine->drivers[8].get_state()) cout << "Q10 driver functioned properly" << endl;
					Sleep(delay);
					if (machine->line_sensors[7].get_state()) cout << "I13 sensor was triggered => attempting to run Q11 driver" << endl;
					Sleep(delay);
					if (machine->drivers[9].get_state()) cout << "Q11 driver functioned properly. The lid of the container with finished products was opened" << endl;
					Sleep(delay);
					machine->containers[4].put(1);
					cout << "Finished product is in the container.\n\n";
					if (i == cycles) {
						cout <<"Congrats! Program successfully completed " << cycles << " cycles." << "\n\nType 'exit' to close program or anything else to try again:  ";
						cin >> command;
						system("cls");
						return;
					}
				}
			}
			else if (machine->get_mode() == 'm'){
				cout << "\nYou've chosen manual mode.\nEnter the name of a text file with the command sequence and necessary configurations:  ";
				string file;
				cin >> file;
				int i = 0;
				int cycles;
				ifstream in(file);
				if (in.is_open()){
					int data;
					in >> data;
					machine->containers[0].set_space(data);
					in >> data;
					machine->inout_containers[0].set_space(data);
					in >> data;
					machine->containers[1].set_space(data);
					in >> data;
					machine->containers[2].set_space(data);
					in >> data;
					machine->containers[3].set_space(data);
					in >> data;
					machine->containers[4].set_space(data);
					in >> data;
					machine->containers[0].put(data);
					in >> data;
					machine->containers[1].put(data);
					in >> data;
					machine->containers[2].put(data);
					in >> data;
					machine->containers[3].put(data);
					in >> data;
					cycles = data;
					bool state;
					while (true){
						cout << "__________________________________________________|  Cycle " << ++i << "  |__________________________________________________\n";
						configure(in);
						machine->containers[0].take();
						cout << "Q2 driver functioned properly. PCB is on the line" << endl;
						configure(in);
						configure(in);
						if (machine->line_sensors[0].get_state()) cout << "I3 sensor was triggered => attempting to run Q3 driver" << endl;
						configure(in);
						if (machine->drivers[1].get_state()) cout << "Q3 driver functioned properly" << endl;
						configure(in);
						configure(in);
						if (machine->line_sensors[1].get_state()) cout << "I0 sensor was triggered => attempting to run Q4 driver" << endl;
						configure(in);
						machine->inout_containers[0].put(1);
						cout << "Q4 driver functioned properly. The lid of the container with the fabricated PCBs was opened\nPCB is in the container" << endl;
						configure(in);
						machine->containers[1].take();
						cout << "Q6 driver functioned properly. Body is in the box on the line" << endl;
						configure(in);
						configure(in);
						if (machine->line_sensors[2].get_state()) cout << "I5 sensor was triggered => attempting to run Q5 driver" << endl;
						configure(in);
						machine->inout_containers[0].take();
						cout << "Q5 driver functioned properly. Fabricated PCB is in the box on the line" << endl;
						configure(in);
						configure(in);
						if (machine->line_sensors[3].get_state()) cout << "I6 sensor was triggered => attempting to run Q7 driver" << endl;
						configure(in);
						machine->containers[2].take();
						cout << "Q7 driver functioned properly. Display is in the box on the line" << endl;
						configure(in);
						configure(in);
						if (machine->line_sensors[4].get_state()) cout << "I8 sensor was triggered => attempting to run Q8 driver" << endl;
						configure(in);
						if (machine->drivers[6].get_state()) cout << "Q8 driver functioned properly" << endl;
						configure(in);
						configure(in);
						if (machine->line_sensors[5].get_state()) cout << "I9 sensor was triggered => attempting to run Q9 driver" << endl;
						configure(in);
						machine->containers[3].take();
						cout << "Q9 driver functioned properly. Manual and warranty card is in the box on the line" << endl;
						configure(in);
						configure(in);
						if (machine->line_sensors[6].get_state()) cout << "I11 sensor was triggered => attempting to run Q10 driver" << endl;
						configure(in);
						if (machine->drivers[8].get_state()) cout << "Q10 driver functioned properly" << endl;
						configure(in);
						configure(in);
						if (machine->line_sensors[7].get_state()) cout << "I13 sensor was triggered => attempting to run Q11 driver" << endl;
						configure(in);
						machine->containers[4].put(1);
						cout << "Q11 driver functioned properly. The lid of the container with finished products was opened\nFinished product is in the container.\n\n";
						if (i == cycles) {
							cout <<"Congrats! Program successfully completed " << cycles << " cycles." << "\n\nType 'exit' to close program or anything else to try again:  ";
							cin >> command;
							system("cls");
							return;
						}
					}
				}
				in.close();
			}
		}
		catch (const Error& err){
			cout << termcolor::on_red << "\nError occured: " << termcolor::reset << err.get_message() << "\nType 'exit' to close program or anything else to try again:  ";
			cin >> command;
			system("cls");
			return;
		}
	}

	Controller(Model* machine_i){
		machine = machine_i;
	}
};

int main(){
	while (command != "exit") {
		cout << "Select operating mode: a(auto) / m(manual):  ";
		cin >> input;
		system("cls");
		Model factory(input[0]);
		Controller controller(&factory);
		controller.start();
	}
	return 0;
}

