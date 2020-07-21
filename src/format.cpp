#include "format.h"

#include <string>

using std::string;
using std::to_string;

// Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hours, minutes;
    string strHH, strMM, strSS;
    hours = seconds/3600;
    seconds = seconds%3600;
    minutes = seconds/60;
    seconds = seconds%60;
    strHH = (hours < 10)? "0" + to_string(hours): to_string(hours);
    strMM = (minutes < 10)? "0" + to_string(minutes): to_string(minutes);
    strSS = (seconds < 10)? "0" + to_string(seconds): to_string(seconds);
    return  strHH + ":" + strMM + ":" + strSS;
}