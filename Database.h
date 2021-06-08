/*
 * Database.h
 * header file for the Database class
 * Peter Horvath
 * COMP15
 * The Gerp (Project 2)
 */

#include <vector>
#include "DirNode.h"
#include "LinkedList.h"
#include "Entry.h"
using namespace std;

#ifndef DATABASE_H
#define DATABASE_H

class Database
{
public:
	Database();
	Database(string);
	~Database();
	void run();
private:
	//helper functions
	void FSTreeTraversal(DirNode*, string);
	void readFile(string);
	string stringProcessing(string);
	void insertWord(string);
	void insertEntry(Entry);
	void expand();
	string lowercase(string);
	void query(string, bool);
	void printMatches(string, int, bool);
	void printEntry(Entry);
	//the hash table and associated data
	LinkedList<Entry> *entries;
	int entries_capacity;
	int num_entries;
	static const int INITIAL_CAPACITY;
	static const double LOAD_FACTOR;
	//variables to help with the arrays
	int curr_file_index;
	int curr_line_number;
	//the 1D array of filenames
	vector<string> filenames;
	//the 2D array of lines
	vector<vector<string> > lines;
};

#endif
