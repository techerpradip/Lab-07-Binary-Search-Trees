/*
  Copyright (c) 2021
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, S. Caplan, A. Danner, L. Meeden, Z. Palmer, A. Soni, M. Wehar
  Distributed as course material for Fall 2021
  CPSC 035: Data Structures and Algorithms
*/

#include <fstream>
#include <iostream>
#include <string>

#include "adts/BST.h"
#include "linkedBST.h"

using namespace std;

// This program expects two arguments: the name of a file to read and the type
// of traversal to use.  It will open the provided file and read each word
// within, tallying the frequency of their appearances in a BST.  It will then
// traverse that BST to print the tallies.

bool isAlphaNum(char c) {
    if (c >= 'A' && c <= 'Z') {
        return true;
    }
    if (c >= 'a' && c <= 'z') {
        return true;
    }
    if (c >= '0' && c <= '9') {
        return true;
    }
    return false;
}

string removeOuterSymbols(string s) {
    int i = 0;
    while (i < s.length() && !isAlphaNum(s[i])) {
        i++;
    }
    if (i == s.length()) {
        return "";
    }
    s = s.substr(i);
    i = s.length();
    while (!isAlphaNum(s[i - 1])) {
        i--;
    }
    s = s.substr(0, i);
    return s;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Usage: ./wordcount filename traversal" << endl;
        return 1;
    }

    string filename = argv[1];
    string traversal = argv[2];

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open file: " << filename << endl;
        return 1;
    }

    if (!(traversal == "pre" || traversal == "in" || traversal == "post" ||
          traversal == "level")) {
        cout << "Traversal must be one of: pre, in, post, level" << endl;
        return 1;
    }

    BST<string, int>* wordCounts = new LinkedBST<string, int>();
    string token;
    file >> token;
    while (!file.eof()) {
        string word = removeOuterSymbols(token);
        if (word.length() > 0) {
            if (wordCounts->contains(word)) {
                wordCounts->update(word, wordCounts->get(word) + 1);
            } else {
                wordCounts->insert(word, 1);
            }
        }
        file >> token;
    }
    file.close();

    vector<pair<string, int>> counts;
    if (traversal == "pre") {
        counts = wordCounts->traversePreOrder();
    } else if (traversal == "in") {
        counts = wordCounts->traverseInOrder();
    } else if (traversal == "post") {
        counts = wordCounts->traversePostOrder();
    } else if (traversal == "level") {
        counts = wordCounts->traverseLevelOrder();
    } else {
        cout << "Invalid traversal: " << traversal << endl;
        delete wordCounts;
        return 1;
    }

    for (int i = 0; i < counts.size(); i++) {
        cout << counts[i].first << ": " << counts[i].second << endl;
    }

    delete wordCounts;

    return 0;
}
