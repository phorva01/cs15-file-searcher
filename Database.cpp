/*
 * Database.cpp
 * The implementation of the Database class.
 * Peter Horvath
 * COMP 15
 * The Gerp (Project 2)
 */

#include "FSTree.h"
#include "DirNode.h"
#include "Entry.h"
#include <fstream>
#include <sstream>
#include <string>
#include <locale>
#include "Database.h"
#include <iostream>
#include <functional>
using namespace std;

const int Database::INITIAL_CAPACITY = 100000000;
const double Database:: LOAD_FACTOR = 0.75;

//Function: Database
//Parameters: none
//Returns: none
//Does: Default constructor. This should never be called.
Database::Database()
{
	cerr << "Default construct of Database called! Doing nothing...\n";
}

//Function: Database
//Parameters: string
//Returns: none
//Does: Parameterized constructor. Builds a file system tree, allocates space,
//      and calls FSTreeTraversal to populate filenames, lines, and entries
//      from the FSTree.
Database::Database(string pathname)
{
	FSTree tree(pathname); //constructs an FSTree of the given directory
	DirNode *root = tree.getRoot();
	curr_file_index = 0;
	entries = new LinkedList<Entry>[INITIAL_CAPACITY];
	entries_capacity = INITIAL_CAPACITY;
	FSTreeTraversal(root, root->getName());
}

//Function: FSTreeTraversal
//Parameters: DirNode, string
//Returns: void
//Does: Traveses the file system tree, finding every file to be read (sent to
//      readFile). Stores every filename in the 1D array filenames.
void Database::FSTreeTraversal(DirNode *node, string p)
{
	if (p != node->getName()) {
		p += "/";
		p += node->getName();
	}
	if (node->hasSubDir()) {
		for (int i = 0; i < node->numSubDirs(); i++)
			FSTreeTraversal(node->getSubDir(i), p);
	}
	if (node->hasFiles()) {
		for (int i = 0; i < node->numFiles(); i++) {
			p += "/";
			p += node->getFile(i);
			vector<string> v;
			lines.push_back(v);
			readFile(p);
			filenames.push_back(p);
			curr_file_index++;
		}
	}
}

//Function: readFile
//Parameters: string
//Returns: void
//Does: Given the filename, opens the file and reads in every line and word.
//      Lines are stored in the 2D array lines and words are passed to 
//      insertWord (to be stored in the hash table entries).
void Database::readFile(string path)
{
	ifstream inFile(path.c_str());
	string curr_line;
	cerr << "path " << path << endl;
	curr_line_number = 1;
	while (getline(inFile, curr_line)) {
		lines[curr_file_index].push_back(curr_line);
		istringstream line(curr_line);
		do {
			string word;
			line >> word;
			word = stringProcessing(word);
			insertWord(word);
		} while (line);
		curr_line_number++;
	}
}

//Function: insertWord
//Parameters: string
//Returns: void
//Does: Creates a new Entry with the given word and passes it to insertEntry.
void Database::insertWord(string word)
{
	string lower_word = lowercase(word);
	hash<string> hasher;
	int the_hash = hasher(lower_word);
	if (the_hash < 0)
		the_hash *= -1;
	Entry entry;
	entry.word = lower_word;
	entry.cased_word = word;
	entry.file_index = curr_file_index;
	entry.line_number = curr_line_number;
	entry.hash_value = the_hash;
	insertEntry(entry);
}

//Function: insertEntry
//Parameters: Entry
//Returns: void
//Does: Inserts an Entry into the hash table. Calls expand if necessary.
void Database::insertEntry(Entry entry)
{
	entries[entry.hash_value % entries_capacity].insert(entry);
	num_entries++;
	double current_load = num_entries / entries_capacity;
	if (current_load >= LOAD_FACTOR)
		expand();
}

//Function: expand
//Parameters: none
//Returns: void
//Does: Doubles the capacity of the hash table and reinserts every entry into
//      its new bucket.
void Database::expand()
{
	LinkedList<Entry> *temporary = new LinkedList<Entry>[entries_capacity * 2];
	for (int i = 0; i < entries_capacity; i++) {
		for (int j = 0; j < entries[i].get_length(); j++) {
			Entry curr_entry = entries[i].get_value_at(j);
			int bucket = curr_entry.hash_value % (entries_capacity * 2);
			temporary[bucket].insert(curr_entry);
		}
	}
	entries_capacity *= 2;
	entries = temporary;
}

//Function: stringProcessing
//Parameters: string
//Returns: string
//Does: Removes any leading or trailing non-alphanumeric characters from the
//      string and returns it.
string Database::stringProcessing(string s)
{
	int s_length;
	string t;
	int t_length;
	int i;
	s_length = s.length();
	i = 0;
	while (!isalnum(s[i]) && i < s_length)
		i++;
	t = s.substr(i, s_length);
	t_length = t.length();
	i = t_length - 1;
	while (!isalnum(t[i]) && i >= 0)
		i--;
	s = t.substr(0, i + 1);
	return s;
}

//Function: lowercase
//Parameters: string
//Returns: string
//Does: Changes every character in the string to lowercase and returns it.
string Database::lowercase(string s)
{
	locale loc;
	string new_string = "";
	int s_length = s.length();
	for (int i = 0; i < s_length; i++)
		new_string += tolower(s[i], loc);
	return new_string;
}

//Function: ~Database
//Parameters: none
//Returns: none
//Does: the default destructor
Database::~Database()
{
	delete [] entries;
}

//Function: run
//Parameters: none
//Returns: void
//Does: Continuously prompts the user for queries and sends them of to the
//      query function. run ends when the user uses CTRL+D or the command @q 
//      or @quit.
void Database::run()
{
	cout << "Query? ";
	string query1;
	string query2;
	while (cin >> query1) {
		if (query1 == "@i" || query1 == "@insensitive") {
			cin >> query2;
			query(query2, true);
		}
		else if (query1 == "@q" || query1 == "@quit") {
			break;
		}
		else {
			query(query1, false);
		}
		cout << "Query? ";
	}
	cout << endl << "Goodbye! Thank you and have a nice day.\n";
}

//Function: query
//Parameters: string, bool
//Returns: void
//Does: Gets the hash value of the queried word and passes on the word, the
//      value, and the bool insensitive (which determines whether the search
//      is case insensitive or not) to printMatches.
void Database::query(string q, bool insensitive)
{
	string lower_q = lowercase(q);
	hash<string> hasher;
	int the_hash = hasher(lower_q);
	if (the_hash < 0) {
		the_hash *= -1;
	}
	if (insensitive) {
		printMatches(lower_q, the_hash, true);
	}
	else {
		printMatches(q, the_hash, false);
	}
}

//Function: printMatches
//Parameters: string, int, bool
//Returns: void
//Does: Goes to the proper bucket in the hash table and searches the chain for
//      matches, which get sent to printEntry. insensitive determines whether
//      the search is case insensitive or not.
void Database::printMatches(string q, int hash_value, bool insensitive)
{
	int i = hash_value % entries_capacity;
	bool word_found = false;
	for (int j = 0; j < entries[i].get_length(); j++) {
		if (insensitive) {
			if (entries[i].get_value_at(j).word == q) {
				word_found = true;
				printEntry(entries[i].get_value_at(j));
			}
		}
		else {
			if (entries[i].get_value_at(j).cased_word == q) {
				word_found = true;
				printEntry(entries[i].get_value_at(j));
			}
		}
	}
	if (!word_found) {
		cout << "query not found\n";
	}
}

//Function: printEntry
//Parameters: Entry
//Returns: void
//Does: Accesses the filename, linenumber, and line data stored in the 1D and
//      2D arrays
void Database::printEntry(Entry entry)
{
	cout << filenames[entry.file_index] << ":" << entry.line_number << ": "
		 << lines[entry.file_index][entry.line_number - 1] << endl;
}

