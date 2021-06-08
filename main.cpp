/*
 * main.cpp
 * Peter Horvath
 * COMP 15
 */

#include "Database.h"
#include <iostream>
using namespace std;

//Function: main
//Parameters: integer and array of character arrays
//Returns: integer
//Does: Takes the command line argument, calls the parameterized constructor of
//      Database with that pathname, and calls the Database run function
int main(int argc, char* argv[])
{
	if (argc != 2) {
		cerr << "usage: ./gerp directory\n";
		return 1;
	}
	Database index(argv[1]);
	index.run();
	return 0;
}
