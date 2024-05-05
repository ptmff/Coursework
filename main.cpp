#include <iostream>
#include "termcolor/termcolor.hpp"
#include "classes.hpp"

using namespace std;

int main(){
	while (command != "exit") {
		cout << "\nSelect operating mode: a(auto) / m(manual):  ";
		cin >> input;
		while (!(input == "a" || input == "A" || input == "m" || input == "M")){
			cout << "" << termcolor::on_yellow << "Incorrect input, try again" << termcolor::reset << "\nSelect operating mode: a(auto) / m(manual):  ";
			cin >> input;
		}
		system("cls");
		Model factory(input[0]);
		Controller controller(&factory);
		controller.start();
	}
	return 0;
}

