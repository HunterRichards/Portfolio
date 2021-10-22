#include <iostream>
#include <iomanip>
#include <conio.h>
#include <chrono>
#include <thread>
#include <stdlib.h>

using namespace std;

void TimeH(int sec) {
	int rsec;
	int rmin;
	int rhour;
	string timeOfDay = "AM";

	// Finds total seconds through modulo 60 (Which results in seconds for clock)
	rsec = sec % 60;
	// Finds total seconds divided by 60 with modulo 60 (which results in minutes for clock)
	rmin = (sec / 60) % 60;
	// Finds total hours by dividing by 60 for (seconds) and another 60 for (minutes) which provides total hour amount for clock
	rhour = (sec / 60) / 60;

	//Ensures clock fits standard 12 hour format
	if (rhour >= 12 || rhour == 0) {
		//If total hours is over 12, go to PM (default is AM)
		if (rhour == 12) {
			timeOfDay = "PM";
		}
		//If hour is at 0, go to AM
		else if (rhour == 0) {
			timeOfDay = "AM";
			rhour = 12;
		}
		//Adjusts clock to 12 hour format (if hours over 12) and sets to PM
		else {
			rhour = rhour % 12;
			timeOfDay = "PM";
		}
	}
	//Prints 12 hour clock
	cout << endl << "        >-> 12 HOUR CLOCK <-<       " << endl << "            " << rhour << " : " << rmin << " : " << rsec << " " << timeOfDay << "             " << endl << endl << endl;
}
void TimeD(int sec) {
	int rsec;
	int rmin;
	int rhour;
	string timeOfDay = "AM";

	// Finds total seconds through modulo 60 (Which results in seconds for clock)
	rsec = sec % 60;
	// Finds total seconds divided by 60 with modulo 60 (which results in minutes for clock)
	rmin = (sec / 60) % 60;
	// Finds total hours by dividing by 60 for (seconds) and another 60 for (minutes) which provides total hour amount for clock
	rhour = (sec / 60) / 60;

	//Adjusts clock to change to PM if over 12 hours
	if (rhour >= 12) {
		timeOfDay = "PM";
	}
	//Prints 24 hour clock
	cout << "        >-> 24 HOUR CLOCK <-<       " << endl << "            " << rhour << " : " << rmin << " : " << rsec << " " << timeOfDay << "             " << endl << endl;
}
int ButtonPress(int rsec) {
	char value = 'Z';

	cout << endl << " >>> Button Pressed <<<" << endl << endl;
	cout << "------------ CONFIG MENU -------------" << endl;
	cout << ">> ADD 1 HOUR:   Type H" << endl;
	cout << ">> ADD 1 MINUTE: Type M" << endl;
	cout << ">> ADD 1 SECOND: Type S" << endl;
	cout << ">> PAUSE CLOCK:  Type P" << endl;
	cout << ">> EXIT CLOCK:   Type E" << endl;
	cout << "------------ CONFIG MENU -------------" << endl;

	//Clears previous input for better visual effect
	_getch();
	//Obtains new value for config menu
	value = _getch();

	//Exit clock entirely
	if (tolower(value) == 'e') {
		return -1;
	}
	//Pauses the clock
	else if (tolower(value) == 'p') {
		return -2;
	}
	//Adjust time by an hour (3600 seconds)
	else if (tolower(value) == 'h') {
		return 3600;
	}
	//Adjust time by an minute (60 seconds)
	else if (tolower(value) == 'm') {
		return 60;
	}
	//Adjust time by an second
	else if (tolower(value) == 's') {
		return 1;
	}
	//Returns to clock if invalid input
	else
	{
		return 0;
	}
}
int main() {
	//Main second time variable
	int sec = 0;
	//Option variable for potetnial time adjustement
	int option = 0;
	//Pauses loop
	bool pause = false;
	//Forever loop
	while (true)
	{
		//If seconds go over a day (86400 seconds), the modulo is returned to start back from zero
		sec = sec % 86400;
		//Main clock program screen
		cout << "--------------- CLOCK ---------------" << endl;
		//Calls standard 12-hour clock
		TimeH(sec);
		//Calls 24-hour clock
		TimeD(sec);
		cout << "--------------- CLOCK ---------------" << endl;
		cout << "------ PRESS BUTTON TO CONFIG -------" << endl;
		//Seeks any form of input
		if (_kbhit()) {
			//Clears console for config menu
			cout << "\033[2J\033[1;1H";
			//Activates config menu with potential time adjustement
			option = ButtonPress(sec);
			//Clears console for config menu
			cout << "\033[2J\033[1;1H";
			//If exit option is used in config menu, exit program
			if (option == -1) {
				return 0;
			}
			//If pause option is used in config menu, pause clock
			else if (option == -2) {
				pause = !pause;
			}
			//Time adjustment is added to total second amount
			else {
				sec = sec + option;
			}
		}
		//Puts program to sleep for 1 second
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		//Clears screen for updated time
		cout << "\033[2J\033[1;1H";
		//If not paused, add a second
		if (!pause) {
			//Adds a second
			sec++;
		}
	}
}