#include "GA.h"
#include "TS.h"
using namespace std;

void menu() {
	cout << "--------------------Minimum Steiner Tree Problem---------------\n" << endl;
	cout << "                       1¡¢Genetic Algorithm" << endl;
	cout << "                       2¡¢Tabu Search" << endl;
	cout << "                       3¡¢Quit\n" << endl;
	cout << "---------------------------------------------------------------\n" << endl;
	cout << "Enter your choice: ";
}

int main() {
	menu();
	int choice;
	cin >> choice;
	while (1) {
		if (choice == 1) {
			GA myGA;
			myGA.run();
		}
		else if (choice == 2) {
			TS myTS;
			myTS.run();
		}
		else if (choice == 3) {
			cout << "Bye!" << endl;
			break;
		}
		else {
			cout << "wrong choice! Try again£º";
		}
		cout << endl << endl << endl;
		cout << "Enter your choice: ";
		cin >> choice;
	}
	system("pause");
	return 0;
}