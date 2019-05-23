
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "filelib.h"
#include "random.h"
#include "map.h"
#include "vector.h"
#include "strlib.h"
#include "random.h"
using namespace std;

/* Function prototypes */
void printIntro();
Vector<string> readFile();
int askN();
Map<Vector<string>, Vector<string>> createMap(Vector<string>& words, int& n);
int askNumWords(int& n);
void randomGenerate(int& numWords, Map<Vector<string>, Vector<string>>& map, int& n);

/* Function main();
 * ----------------------
 * Welcomes to user and reads the input file into a Vector.
 * Creates a map of n-1 words to the nth word for all of the
 * words in a file, then randomly generates text based
 * on the original text until the user inputs "0" as the
 * desired number of words to be generated.
 */
int main() {
    printIntro();
    Vector<string> words = readFile();
    int n = askN();
    Map<Vector<string>, Vector<string>> map = createMap(words, n);
    int numWords = askNumWords(n);
    while (numWords != 0) {
        randomGenerate(numWords, map, n);
        numWords = askNumWords(n);
    }
    cout << "Exiting." << endl;
    return 0;
}

/* Function printIntro();
 * ----------------------
 * Displays a welcome message to the user that explains the purpose
 * of N-grams.
 */
void printIntro() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." <<endl;
}

/*
 * Function readFile();
 * --------------------
 * Prompts user for file and stores the content of the file
 * in a vector. Returns the vector to main().
 */
Vector<string> readFile() {
    ifstream stream;
    Vector<string> words;
    cout << endl;
    string filename = promptUserForFile(stream, "Input file name? ");
    string strword;
    while (stream >> strword) {
        words.add(strword);
    }
    stream.close();
    return words;
}

/*
 * Function askN();
 * --------------------
 * Prompts user for an n value and returns it to main().
 */
int askN() {
    int n = 0;
    while (n < 2) {
        n = getInteger("Value of N?");
        if (n < 2) {
            cout << "Please enter an integer greater than or equal to 2" << endl;
        }
    }
    return n;
}

/*
 * Function createMap();
 * --------------------
 * Creates a map of Vectors that will store keys consisting of n-1 words and
 * values consisting of the word(s) that come after the group of n-1 words.
 * (any nth word that comes after each specific group of n-1 words)
 * It does so by creating a "window" vector that shifts over once its
 * size becomes n-1 words.
 *
 * Takes in the vector of words of the file and the value
 * of n. The function takes care of wrapping by creating
 * a new vector wrapVec. Returns the newly created map to main().
 */
Map<Vector<string>, Vector<string>> createMap(Vector<string>& words, int& n) {
    Map<Vector<string>, Vector<string>> map;
    Vector<string> window;
    Vector<string> wrapVec = words;
    for (int a = 0; a < n - 1; a++) {
        wrapVec.add(words[a]);
    }
    for (int j = 0; j < wrapVec.size(); j++) {
        if (window.size() == n - 1) {
            if(!map.containsKey(window)) {
                Vector<string> val;
                val.add(wrapVec.get(j));
                map.put(window, val);
            }
            else {
                Vector<string> val = map.get(window) += wrapVec.get(j);
                map.put(window, val);
            }
            window.remove(0);
            window.add(wrapVec[j]);
        }
        else {
            window.add(wrapVec[j]);
        }
    }
    return map;
}

/*
 * Function askNumWords();
 * --------------------
 * Prompts user for the number of words they would like to
 * randomly generate. Returns that number to main, or quits
 * the program if the user enters 0. Takes in parameter
 * int n to ensure that the number of words is at least n.
 */
int askNumWords (int& n) {
    int numWords;
    cout << endl;
    numWords = getInteger("# of random words to generate (0 to quit)? ");
    while (true) {
        if (numWords < n && numWords > 0) {
            cout << "Must be at least " << n << " words." << endl;
            numWords = getInteger("# of random words to generate (0 to quit)? ");
        }
        else if (numWords == 0) {
            break;
        }
        else {
            return numWords;
        }
    }
    return 0;
}

/*
 * Function randomGenerate();
 * --------------------
 * Randomly generates text by taking in the original map, n,
 * and the number of words to be generated. A random key value
 * is generated, which will choose a key from the original map.
 * The corresponding key is the startingKey, which will start off
 * the text.
 *
 * Another random value will then be generated to choose one of the
 * values associated with that original key. The key "window" will
 * then shift to the next n-1 words. The process will continue
 * until numWords is reached.
 */
void randomGenerate(int& numWords, Map<Vector<string>, Vector<string>>& map, int& n) {
    Vector<Vector<string>> originalMapKeys = map.keys();
    int randomIntKey = randomInteger(0, originalMapKeys.size() - 1);
    Vector<string> startingKey = originalMapKeys[randomIntKey];
    cout << "...";
    for (int j = 0; j < n - 1; j++) {
        cout << startingKey[j] << " ";
    }
    Vector<string> originalMapValues = map.get(originalMapKeys[randomIntKey]);
    for (int i = 0; i < numWords - (n - 1); i++) {
        int randomValKey = randomInteger(0, originalMapValues.size() - 1);
        cout << originalMapValues[randomValKey] << " " ;
        startingKey.remove(0);
        while (startingKey.size() < (n - 1)) {
            startingKey.add(originalMapValues[randomValKey]);
        }
        originalMapValues = map.get(startingKey);
    }
    cout << "...";
    cout << endl;
}
