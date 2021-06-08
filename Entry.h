/*
 * Entry.h
 * implementation of the Entry struct
 * Peter Horvath
 * COMP15
 * The Gerp (Proj 2)
 */

using namespace std;

#ifndef ENTRY_H
#define ENTRY_H

struct Entry
{
	string word; //the lowercase word
	string cased_word; //word in its original case
	int hash_value; //resultant value of hashing the lowercase word
	int file_index; //index for filenames, which stores each word's filename
	int line_number; //the line number of this word
};

#endif
