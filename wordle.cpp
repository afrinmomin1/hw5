#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here (did not use any helper functions)


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // container to store all matching words
    set<string> results;
    // length of target words
    size_t n = in.size();

    // build a frequency map of floating letters
    map<char, int> floatFreq;
    for(char c : floating) {
        floatFreq[c]++;
    }

    // iterate through each candidate word in the dict
    for(const string& word: dict) {
        // skip the word if length is not a match
        if(word.size() != n) {
            continue;
        }

        // flag var to track if the word passes all the checks
        bool match = true;
        // count how many times each floating letter appears in the word's blank slots
        map<char, int> seenFreq;

        // single pass over the characters
        for(size_t i = 0; i < n; i++) {
            char w  = word[i];
            // check 1: all chars are lowercase
            // use asci values to ensure each char falls in the a-z range
            if(w < 'a' || w > 'z') {
                // change flag to false (reject the word)
                match = false;
                break;
            }

            // check 2: check fixed-position (green) letters
            // if in[i] != '-', then word[i] has to equal in[i]
            if(in[i] != '-' && in[i] != w) {
                // no match in a fixed slot
                match = false;
                break;
            }

            // check 3: check float constraints in blank spots
            // only check the blank spots
            if(in[i] == '-') {
                map<char, int>::iterator it = floatFreq.find(w);
                if(it != floatFreq.end()) {
                    seenFreq[w]++;
                }
            }
        }
        if(!match) {
            continue;
        }

        // verify that each floating letter appears as often as needed
        for(const pair<const char, int>& k : floatFreq) {
            char letter = k.first;
            int count  = k.second;
            if(seenFreq[letter] < count) {
                match = false;
                break;
            }
        }
        if(!match) {
            continue;
        }

        // if all the checks are passed, add word to the results set
        if(match) {
            results.insert(word);
        }
    }

    // return the complete set of words
    return results;
}

// Define any helper functions here

// i chose not to use any helper functions in order to stay within the bounds of the actual word list and ensure each word is only checked once.

