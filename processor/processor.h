#ifndef EDITORSDREAM_H
#define EDITORSDREAM_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <ctime>
#include <iomanip>
#include <cstring>
#include "containers/node/node.h"
#include "containers/heap/heap.h"
#include "containers/pair/pair.h"

class processor
{
    public:
        processor(std::string path = "");
        ~processor();

        void askText();
        bool readText(std::string path = "");
        bool save(std::string path = "output.txt");

        typedef void (processor::*fptr)(std::ostream&);

    private:

        heap<node<std::string>, ascend<node<std::string>>> hp[26];
        heap<myPair<std::string, uint>, ascend<myPair<std::string, uint>>> topNumHeap;
        fptr print[4];

        // Files
        std::ifstream input;
        std::ofstream output;
        std::string filePath;

        // General info
        uint lines, paragraphs, sentences, words, syllables, topNum;
        float readingLevel;
        clock_t time;

        // Text/info processing
        bool endOfSentence(const std::string &token);
        std::string processToken(std::string token);
        uint numSyllables(std::string token);
        void calculateTopWords();

        // Display
        void printGeneral(std::ostream& out);
        void printFrequency(std::ostream& out);
        void printTopWords(std::ostream& out);
        void printAllWords(std::ostream& out);
};

#endif // EDITORSDREAM_H
