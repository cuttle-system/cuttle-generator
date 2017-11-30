#pragma once

#include <vector>

template <typename T>
void clear(std::vector<T>& vect)
{
    for (T e : vect) {
        delete e;
    }
}
