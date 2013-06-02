#pragma once
#include "ofMain.h"

class Sequence
{
public:
    Sequence() : start(0), stop(0)
    //  ^^^^^^^^^^ ^^^^^^^^^^^^^^^^^^^
    {}
    
    Sequence(long _start, long _stop)
    : start(_start), stop(_stop) // Use member-initializer list here too
    {}
    
    long start;
    long stop;
    
};


