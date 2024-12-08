export module scrollingbuffer;

import std;

import <imgui.h>;

export
template<typename T, int size>
struct ScrollingBuffer
{
    int offset;
    std::vector<T> data;
    ScrollingBuffer()
    {
        offset = 0;
        data.reserve(size);
    }
    void addPoint(const T& x)
    {
        if (data.size() < size)
            data.push_back(x);
        else
        {
            data[offset] = x;
            offset = (offset + 1) % size;
        }
    }
    void erase()
    {
        if (data.size() > 0)
        {
            data.clear();
            offset = 0;
        }
    }
};