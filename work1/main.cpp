#include <iostream>
#include <fstream>
#include <set>
#include <map>

size_t SIZE;

class KVItem
{
public:
    const char* word;
    mutable int count;
    KVItem(const char* word)
    {
        this->word = word;
        this->count = 1;
    }
};

struct Comparator
{
    bool operator()(const char *left, const KVItem right)
    {
        return strcmp(left, right.word) < 0;
    }

    bool operator()(const KVItem left, const char *right)
    {
        return strcmp(left.word, right) < 0;
    }

    bool operator()(const KVItem left, const KVItem right)
    {
        return strcmp(left.word, right.word) < 0;
    }
};

struct Comparator2
{
    bool operator()(const int left, const KVItem right)
    {
        return left < right.count;
    }

    bool operator()(const KVItem left, const int right)
    {
        return left.count < right;
    }

    bool operator()(const KVItem left, const KVItem right)
    {
        return left.count < right.count;
    }
};

size_t getFileSize(char* fileName)
{
    std::ifstream in(fileName);
    in.seekg(0, std::ios_base::end);
    return in.tellg();
}

void printSlice(size_t start, size_t finnish, char* buffer)
{
    for (int j = start; j<finnish; j++)
    {
        char k = *(buffer + j);
        std::cout << k;
    }
}

char* getText(char* fileName)
{
    std::ifstream in(fileName);
    char symbol;
    size_t fileSize = getFileSize(fileName);
    SIZE = fileSize;
    std::cout << fileSize << std::endl;
    char* buffer = new char[fileSize + 1];
    int i = 0;
    while (in.get(symbol))
    {
        buffer[i] = (char)tolower((int)symbol);
        i++;
    }
    buffer[i] = 0;
    in.close();
    return buffer;
}


std::set<KVItem, Comparator> parse(char* buffer)
{
    size_t startIndex = 0;
    size_t finnishIndex = 0;
    size_t wordLength = 0;
    char *word;
    std::set<KVItem, Comparator> frDict;
    for (int i = 0; i < SIZE; i++)
    {
        if (!isalpha(buffer[i]) and buffer[i] != '-')
        {
            if (isalpha(buffer[i-1]))
            {
                //printSlice(startIndex, finnishIndex, buffer);
                //std::cout << std::endl;
                word = (char*)malloc(wordLength);
                for (int k=0; k < wordLength; k++)
                {
                    word[k] = buffer[startIndex+k];
                }
                auto item = frDict.find(word);
                if (item != frDict.end())
                {
                    item->count = item->count + 1;
                }
                else
                {
                    KVItem item = KVItem(word);
                    frDict.insert(item);
                }
            }
            startIndex = i + 1;
            finnishIndex = i + 1;
            wordLength = 0;
        }
        else
        {
            finnishIndex++;
            wordLength++;
        }
    }
    word = (char*)malloc(wordLength);
    for (int k=0; k < wordLength; k++)
    {
        word[k] = buffer[startIndex+k];
    }
    auto item = frDict.find(word);
    if (item != frDict.end())
    {
        item->count = item->count + 1;
    }
    else
    {
        KVItem item = KVItem(word);
        frDict.insert(item);
    }
    return frDict;
}

std::map<std::string, size_t> parseStd(char* buffer)
{
    size_t startIndex = 0;
    size_t finnishIndex = 0;
    size_t wordLength = 0;
    std::string word;
    std::map<std::string, size_t> frDict = std::map<std::string, size_t>();
    for (int i = 0; i < SIZE; i++)
    {
        if (!isalpha(buffer[i]) and buffer[i] != '-')
        {
            if (isalpha(buffer[i-1])) {
                for (int k=0; k < wordLength; k++)
                {
                    word += buffer[startIndex+k];
                }
                auto item = frDict.find(word);
                if (item != frDict.end())
                {
                    item->second = item->second + 1;
                }
                else
                {
                    frDict.insert(std::pair<std::string, size_t>(word, 1));
                }
            }
            startIndex = i + 1;
            finnishIndex = i + 1;
            wordLength = 0;
        } else
        {
            word = "";
            finnishIndex++;
            wordLength++;
        }
    }
    std::cout << word << std::endl;
    for (int k=0; k < wordLength; k++)
    {
        word += buffer[startIndex+k];
    }
    auto item = frDict.find(word);
    if (item != frDict.end())
    {
        item->second = item->second + 1;
    }
    else
    {
        frDict.insert(std::pair<std::string, size_t>(word, 1));
    }
    return frDict;
}

std::set<KVItem, Comparator2> sort_by_count(std::set<KVItem, Comparator> map)
{
    std::set<KVItem, Comparator2> map2;
    for (KVItem item: map)
    {
        map2.insert(item);
    }
    return map2;
}

int main()
{
    char* fileName = "test1.txt";
    char* Text = getText(fileName);
    clock_t start = clock();
    const std::set<KVItem, Comparator> dict = parse(Text);
    clock_t finnish = clock();
    std::cout << std::endl;
    for (KVItem i: dict)
    {
        std::cout << i.word << " - " << i.count << std::endl;
    }

    clock_t startS = clock();
    const std::map<std::string, size_t> dict2 = parseStd(Text);
    clock_t finnishS = clock();
    for (auto p: dict2)
    {
        std::cout << p.first << " - " << p.second << std::endl;
    }
    double tM = ((double)(finnish - start)*1000)/CLOCKS_PER_SEC;
    double tS = ((double)(finnishS - startS)*1000)/CLOCKS_PER_SEC;
    std::cout << tM << " milliseconds in my alloc" << std::endl;
    std::cout << tS << " milliseconds in std::map" << std::endl;
    std::cout << "In " << tS/tM << " times alloc faster than std::map" << std::endl;
    std::set<KVItem, Comparator2> map = sort_by_count(dict);
    for (KVItem item: map)
    {
        std::cout << item.word << " - " << item.count << std::endl;
    }
    return 0;
}