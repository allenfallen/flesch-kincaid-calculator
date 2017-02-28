#include "processor.h"

processor::processor(std::string path)
{
    filePath = path;
    lines = sentences = words = syllables = 0;
    paragraphs = 1;
    topNum = 10;
    print[0] = &processor::printGeneral;
    print[1] = &processor::printFrequency;
    print[2] = &processor::printTopWords;
    print[3] = &processor::printAllWords;
}

processor::~processor()
{
    lines = paragraphs = sentences = words = syllables = topNum = 0;
    input = std::ifstream();
    output = std::ofstream();
    filePath = std::string();
}

void processor::askText()
{
    std::cout<<"Please enter your text file: ";
    getline(std::cin, filePath);
    filePath = "text/" + filePath;
}

bool processor::readText(std::string path)
{
    std::string line, processedToken;
    char* token;

    // Open the text file
    if(path != "")
        filePath = path;
    input.open(filePath);

    // Read the text file
    if(input.is_open())
    {
        // Record time
        time = clock();

        // Read each line
        while(getline(input, line))
        {
            // Increment line count for each line
            lines++;

            // Increment paragraph count for each empty line
            paragraphs += (!line.length() || line[0] == '\r');

            // Create tokens from the line
            token = strtok(const_cast<char*>(line.c_str()), " —\"\r");
            while(token && (processedToken = processToken(token)).compare("") != 0)
            {
                // Sentence count (from unaltered token)
                sentences += endOfSentence(token);

                // Word count
                words++;

                // Syllables
                syllables += numSyllables(processedToken);

                // Insert node into heap
                node<std::string> n(processedToken, lines, paragraphs);
                hp[processedToken[0] - 'A']<<n;

                // Continue creating more tokens from the line if possible
                token = strtok(NULL, " —\"\r");
            }
        }
        // Calculate Flesch-Kincaid reading level
        readingLevel = 206.835 - (1.015*((float)words/sentences)) - (84.6*(float)syllables/words);

        // Calculate top words using a min-heap
        calculateTopWords();

        // End record time
        time = clock() - time;
    }
    // Invalid filepath/cannot open file
    else
    {
        std::cout<<"INVALID FILE!"<<std::endl;
        return false;
    }
    input.close();
    return true;
}

bool processor::save(std::string path)
{
    path = "output/" + path;
    output.open(path);
    if(output.is_open())
    {
        processor *ptr = this;
        for(int i = 0; i < 4; i++)
            (ptr->*print[i])(output);
    }
    else
    {
        std::cout<<"CANNOT WRITE TO FILE!"<<std::endl;
        return false;
    }
    output.close();
    std::cout<<"Finished!"<<std::endl;
    return true;
}

bool processor::endOfSentence(const std::string &token)
{
    // Not a sentence if does not have '.', '!', or '?' at all
    if(token.find_first_of(".!?") > token.length())
        return false;

    // Delimiters for honorifics/abbreviations that use the period
    if(token.find('.') < token.length())
    {
        std::string key[] = {"Dr", "Mr", "Ms", "Mrs", "St", "Prof", "Capt", "Cpt", "M", "Mme", "p.s", "P.S"};
        for(int i = 0; i < 12; i++)
            if(token.find(key[i]) < token.length())
                return false;
    }
    return true;
}

std::string processor::processToken(std::string token)
{
    int begin = 0, end = token.length()-1;

    // Remove non-alphabet characters before the word
    while(token[begin] && !isalpha(token[begin]))
        begin++;

    // Remove non-alphabet characters after the word
    while(token[end] && !isalpha(token[end]))
        end--;

    // Possessive apostrophe
    if(token.find("'s") < end)
        token = token.substr(0, token.find("'s"));

    // Format upper/lower cases
    for(int i = 0; i < token.length(); i++)
        token[i] = tolower(token[i]);
    token[begin] = toupper(token[begin]);

    if(token == "" || token.length() < end-begin+1)
        return "";

    return token.substr(begin, end+1);
}

uint processor::numSyllables(std::string token)
{
    uint syllables = 0;
    std::string vowelKey = "aeiouy";
    std::string notSilentE[] = {"ble", "cle", "dle", "fle", "gle", "kle", "ple", "tle", "zle"};
    std::string notSingleSyllable[] = {"ia", "io", "iu", "eo", "ao", "ua", "uo", "oil"};
    uint pos;
    token[0] = tolower(token[0]);

    for(int i = 0; i < token.length(); i++)
    {
        // Search and increment for each vowel in the word
        if(vowelKey.find(token[i]) < vowelKey.size())
        {
            syllables++;

            // Vowel pairs are typically a single syllable
            if(i >= 1 && vowelKey.find(token[i-1]) < vowelKey.size())
                syllables--;

            // Vowels surrounding the 'y' are typically 2 syllables
            pos = token.find('y');
            if(pos < token.length() && vowelKey.find(pos-1) < vowelKey.size() && vowelKey.find(pos+1) < vowelKey.size())
                syllables++;

            // More anomalies in the vowel-syllable relationship
            for(int i = 0; i < (sizeof(notSingleSyllable)/sizeof(notSingleSyllable[i])); i++)
                if(token.find(notSingleSyllable[i]) < token.length())
                    syllables++;
        }
    }

    // Final Silent 'e'. If 'e' is the only syllable, it cannot be silent
    if(syllables && token[token.length()-1] == 'e')
    {
        syllables--;

        // Check for anomalies in which the final 'e' is not silent
        for(int i = 0; i < (sizeof(notSilentE)/sizeof(notSilentE[0])); i++)
            if(token.length() > notSilentE[i].length() &&
               token.substr(token.length()-notSilentE[i].length()).compare(notSilentE[i]) == 0)
            {
                syllables++;
                break;
            }
    }
    return syllables;
}

void processor::calculateTopWords()
{
    std::string line, currWord;
    uint count;
    myPair<std::string, uint> junk;
    node<std::string> n;
    heap<node<std::string>, ascend<node<std::string>>> temp;

    for(int i = 0; i < 26; i++)
    {
        temp = hp[i];
        while(temp.size())
        {
            temp>>n;
            currWord = n.data;
            count++;

            // Add to count as long as next is the same word
            while(temp.size() && currWord.compare(temp.peek().data) == 0)
            {
                temp>>n;
                count++;
            }

            // Insert into min-heap
            // Extract the lowest value if a higher value is found
            if(topNumHeap.size() < topNum)
                topNumHeap<<(myPair<std::string, uint>(currWord, count));
            else if(topNumHeap.peek() < count)
            {
                topNumHeap>>junk;
                topNumHeap<<(myPair<std::string, uint>(currWord, count));
            }
            count = 0;
        }
        temp.clear();
    }
}

void processor::printGeneral(std::ostream& out)
{
    out<<std::setw(60)<<std::setfill('=')<<""<<std::endl;
    out<<"General Info"<<std::endl;
    out<<std::setw(60)<<""<<std::endl<<std::setfill(' ');

    out<<std::setw(20)<<std::left<<"Words:"<<std::right<<std::setw(8)<<std::setprecision(5)<<words<<std::endl;
    out<<std::setw(20)<<std::left<<"Lines:"<<std::right<<std::setw(8)<<std::setprecision(5)<<lines<<std::endl;
    out<<std::setw(20)<<std::left<<"Sentences:"<<std::right<<std::setw(8)<<std::setprecision(5)<<sentences<<std::endl;
    out<<std::setw(20)<<std::left<<"Paragraphs:"<<std::right<<std::setw(8)<<std::setprecision(5)<<paragraphs<<std::endl;
    out<<std::setw(20)<<std::left<<"Reading level:"<<std::right<<std::setw(8)<<std::setprecision(5)<<readingLevel<<std::endl;
    out<<std::setw(20)<<std::left<<"Time:"<<std::right<<std::setw(8)<<std::setprecision(5)<<(double)time/CLOCKS_PER_SEC<<" s"<<std::endl;
}

void processor::printFrequency(std::ostream& out)
{
    out<<std::endl<<std::setw(60)<<std::setfill('=')<<""<<std::endl;
    out<<"Frequency of first letter of each word"<<std::endl;
    out<<std::setw(60)<<""<<std::endl<<std::setfill(' ');

    for(int i = 0; i < 26; i++)
        out<<(char)('A' + i)<<": "
            <<std::setfill(' ')<<std::setw(6)<<std::setprecision(3)
            <<(((float)hp[i].size())/words) * 100<<" %"<<std::endl;
}

void processor::printTopWords(std::ostream& out)
{
    out<<std::endl<<std::setw(60)<<std::setfill('=')<<""<<std::endl;
    out<<"Top "<<topNum<<" words"<<std::endl;
    out<<std::setw(60)<<""<<std::endl<<std::setfill(' ');

    myPair<std::string, uint> arr[topNum];
    for(int i = topNum-1; i > -1; i--)
        topNumHeap>>(arr[i]);
    for(int i = 0; i < topNum; i++)
        out<<std::left<<std::setw(4)<<i+1<<std::setw(7)<<arr[i].key<<std::right<<std::setw(8)<<arr[i].value
              <<std::setw(13)<<((float)arr[i].value/words) * 100<<" %"<<std::endl;
}

void processor::printAllWords(std::ostream& out)
{
    out<<std::endl<<std::setw(60)<<std::setfill('=')<<""<<std::endl;
    out<<"Word list"<<std::endl;
    out<<std::setw(60)<<""<<std::endl<<std::setfill(' ');

    node<std::string> n;
    std::string currWord;
    uint wordCount = 0;
    std::vector<uint> lineNum;
    std::vector<uint> paraNum;
    for(int i = 0; i < 26; i++)
    {
        while((hp[i].size()))
        {
            hp[i]>>n;
            currWord = n.data;
            wordCount++;
            lineNum = n.lineNum;
            paraNum = n.paraNum;

            // Add to count/lineNum/paraNum as long as the next in the heap is the same word
            while(hp[i].size() && currWord.compare(hp[i].peek().data) == 0)
            {
                hp[i]>>n;
                wordCount++;
                lineNum.push_back(n.lineNum[0]);
                paraNum.push_back(n.paraNum[0]);
            }

            // Word
            out<<currWord<<std::endl;

            // Word Count and frequency
            out<<"Count: "<<std::left<<std::setw(6)<<wordCount
               <<std::setw(6)<<std::setprecision(6)<<(float)wordCount/words * 100<<" %"<<std::endl;

            // Line num(s)
            out<<"Line: ";
            for(std::vector<uint>::iterator it = lineNum.begin(); it != lineNum.end(); it++)
                output<<*it<<" ";
            out<<std::endl;

            // Paragraph num(s)
            out<<"Paragraph: ";
            for(std::vector<uint>::iterator it = paraNum.begin(); it != paraNum.end(); it++)
                output<<*it<<" ";
            out<<std::endl<<std::endl;

            wordCount = 0;
        }
    }
}
