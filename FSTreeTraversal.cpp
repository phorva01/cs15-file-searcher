/*
 * Peter Horvath
 * FSTreeTraversal.cpp
 * COMP15
 * Proj 2 Phase I
 */

#include "FSTree.h"
#include "DirNode.h"
#include <iostream>
#include <string>
using namespace std;

void recursive_print(DirNode*, string);

//Function: main
//Parameters: an integer and an array of command line arguments
//Returns: an integer
//Does: Takes a string of a directory name from the command line and constructs
//      an FSTree of the directory. It then calls the print function on that
//      directory.
int main(int argc, char* argv[])
{
	if (argc != 2) {
		cerr << "This program should be called with the name of a directory\n";
		return 1;
	}

	string path = argv[1];
	FSTree tree(path); //constructs an FSTree of the given directory
	DirNode *root = tree.getRoot();
	recursive_print(root, root->getName());
	return 0;
}

//Function: recursive_print
//Parameters: a DirNode and a string
//Returns: void
//Does: Calls itself on each subdirectory of the given node. String p stores
//      the path up to the current node and is gradually added to with each
//      call. 
void recursive_print(DirNode *node, string p)
{
	if (p != node->getName()) {
		p += "/";
		p += node->getName();
	}
	if (node->hasSubDir()) {
		for (int i = 0; i < node->numSubDirs(); i++)
			recursive_print(node->getSubDir(i), p);
	}
	if (node->hasFiles()) {
		for (int i = 0; i < node->numFiles(); i++)
			cout << p << "/" << node->getFile(i) << endl;
	}
	cout << p << endl;
}

