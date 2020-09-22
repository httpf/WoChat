#ifndef COMMON_CPP
#define COMMON_CPP

#include <string>

bool isID(std::string str)
{
    for (int i = 0; i < str.size(); i++)
    {
        int tmp = (int)str[i];
        if (tmp >= 48 && tmp <= 57)
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return false;
}

#endif