#include <string>
#include "format.h"

using std::string;
using std::to_string;

string Format::ElapsedTime(long seconds) {
    int h, m, s;
    string HH, MM, SS;
    h = (int)seconds/3600;
    m = (int)(seconds - h * 3600)/60;
    s = seconds - h * 3600 - m * 60;
    if (h < 10){
        HH = '0' + to_string(h);
    } else
    {
        HH = to_string(h);
    }
    if (m < 10){
        MM = '0' + to_string(m);
    } else
    {
        MM = to_string(m);
    }
    if (s < 10){
        SS = '0' + to_string(s);
    } else
    {
        SS = to_string(s);
    }
    return HH + ':' + MM + ':' + SS;
}