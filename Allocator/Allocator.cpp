#include <cstddef>
#include <cstdlib>
#include <map>
#include <list>
#include <cmath>
#include <vector>

using namespace std;
struct cmp {
    bool operator()(const size_t a, const size_t b) const {
        return a < b;
    }
};

class Allocator
{
public:
    map<size_t, pair<size_t, bool>, cmp> Blocks;
    vector<pair<size_t, size_t>> Buddies;
    Allocator(size_t size)
    {
        size_t mem_size = getNextPowerOf2(size);
        auto pointer = (size_t)(malloc(mem_size));
        Blocks.insert(pair<size_t, pair<size_t, bool>>(pointer, pair<size_t, bool>(mem_size, true)));
    };

    void* alloc(size_t size)
    {
        for (auto element: Blocks)
        {
            if (element.second.first >= (2 * size))
            {
                size_t newSize = (size_t)((Blocks.find(element.first)->second.first)/2);
                size_t ptr = (Blocks.find(element.first)->first);
                Blocks.find(element.first)->second.first = newSize;
                Blocks.insert(pair<size_t, pair<size_t, bool>>(ptr + newSize, pair<size_t, bool>(newSize, true)));
                Buddies.push_back(pair<size_t, size_t>(ptr, ptr + newSize));
                alloc(size);
            } else
            {
                if (element.second.first >= size && element.second.second)
                {
                    Blocks.find(element.first)->second.second = false;
                    return (void*)element.first;
                }
            }
            return (void*)-1;
        }
    }

    void dealloc(void* memPoint)
    {
        Blocks.find((size_t)memPoint)->second.second = true;
        uniteBuddies();
    }

private:
    static unsigned long getNextPowerOf2(unsigned long number)
    {
        if ((number & (number-1)) == 0)
            return number;
        auto logFromNumber = (int)log2(number);
        return (int)pow(2, logFromNumber + 1);
    }

    void uniteBuddies()
    {
        for (size_t idx=0; idx < Buddies.size(); idx++)
        {
            auto firstIdx = Buddies[idx].first;
            auto secondIdx = Buddies[idx].second;
            auto first = Blocks.find(firstIdx);
            auto second = Blocks.find(secondIdx);
            if (first == Blocks.end() || second == Blocks.end())
            {
                continue;
            }
            if (first->second.first == second->second.first && first->second.second && second->second.second)
            {
                Blocks.erase(second->first);
                Blocks.find(first->first)->second.first *= 2;
                Buddies.erase(Buddies.begin() + idx);
                uniteBuddies();
            }
        }
    }
};