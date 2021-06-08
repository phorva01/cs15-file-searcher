/*
 * Peter Horvath
 * StringProcessing.cpp
 * COMP15
 * Preject 2 Week 1
 */

#include <string>
#include <iostream>
#include <locale>
using namespace std;

//Function: main
//Parameters: none
//Returns: an integer
//Does: Continuously takes a string from the user and prints a string of 
//      all the alphanumeric characters of that string to cout.
int main()
{
	string s;
	int s_length;
	string t;
	int t_length;
	int i;
	while (cin >> s) {
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
		cout << s << endl << endl; //to be replaced
	}
	return 0;
}
