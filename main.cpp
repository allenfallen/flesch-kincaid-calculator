#include <iostream>
#include "processor/processor.h"

using namespace std;

int main()
{
    processor txtProcessor;

    txtProcessor.askText();
    txtProcessor.readText();
    txtProcessor.save();
    return 0;
}
