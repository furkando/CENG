#include "Article.h"

Article::Article(int table_size, int h1_param, int h2_param)
{
    int i;
    table = new std::pair<std::string, int>[table_size];
    for (i = 0; i < table_size; i++)
    {

        table[i].first = "";
        table[i].second = -1;
    }
    this->table_size = table_size;
    this->h1_param = h1_param;
    this->h2_param = h2_param;
}

Article::~Article()
{
    delete[] table;
}

int Article::get(std::string key, int nth, std::vector<int> &path) const
{
    int i = 0, index, z = 0, p = 0;
    std::vector<int>::iterator it;

    index = hash_function(key, i);
    if (table[index].first == key)
    {
        return table[index].second;
    }
    
    for (int k = 1; k < nth; k++)
    {
        i++;
        index = hash_function(key, i);
        path.push_back(index);
    }

    while (table[index].first != key)
    {
        p++;
        i++;
        index = hash_function(key, i);
        path.push_back(index);
        if (table[index].first == EMPTY_KEY && p != 1)
        {
            return -1;
        }
    }

    if (path.size() > 0)
    {
        it = path.end();
        it--;
        for (int n = 2; n < nth; n++)
        {
            it--;
        }
    }
    else
    {
        it = path.begin();
    }

    return table[*it].second;
}

int Article::insert(std::string key, int original_index)
{
    if (getLoadFactor() > MAX_LOAD_FACTOR)
    {
        expand_table();
    }
    int i = 0, index;
    index = hash_function(key, i);

    while (table[index].second != -1 && table[index].first != "")
    {
        i++;
        index = hash_function(key, i);
    }
    table[index].first = key;
    table[index].second = original_index;
    return i;
}

int Article::remove(std::string key, int nth)
{
    int i = 0, index, u = 0, count = 0;
    std::vector<int>::iterator it;
    std::vector<int> path;
    for (u = 0; u < table_size; u++)
    {
        if (table[u].first == key)
        {
            count++;
        }
    }
    if (count < nth)
    {
        return -1;
    }
    if (nth == 1)
    {
        index = hash_function(key, i);
        if (table[index].first == key)
        {
            table[index].first = EMPTY_KEY;
            table[index].second = MARKED_INDEX;
            return 0;
        }
        while (table[index].first != key)
        {
            i++;
            index = hash_function(key, i);
            path.push_back(index);
        }
    }
    else
    {
        for (int k = 1; k < nth; k++)
        {
            i++;
            index = hash_function(key, i);
            path.push_back(index);
        }
        while (table[index].first != key)
        {
            i++;
            index = hash_function(key, i);
            path.push_back(index);
        }
    }

    if (path.size() > 0)
    {
        it = path.end();
        it--;
        for (int n = 2; n < nth; n++)
        {
            it--;
        }
    }
    else
    {
        it = path.begin();
    }

    table[*it].first = EMPTY_KEY;
    table[*it].second = MARKED_INDEX;
    int z = 0;
    index = hash_function(key, z);
    while (table[index].second != -1 && table[index].first != "")
    {
        z++;
        index = hash_function(key, z);
    }
    return z;
}

double Article::getLoadFactor() const
{
    int i;
    double j = 0;
    for (i = 0; i < table_size; i++)
    {
        if (table[i].second != -1 && table[i].first != "")
        {
            j++;
        }
    }
    return j / table_size;
}

void Article::getAllWordsFromFile(std::string filepath)
{
    /*#########################################
    #                  TO-DO                  #
    #       Parse the words from the file     #
    #      word by word and insert them to    #
    #                hash table.              #
    #   For your own inputs, you can use the  #
    #  'inputify.py' script to fix them to    #
    #            the correct format.          #
    #########################################*/
}

void Article::expand_table()
{

    int newSize, oldSize;
    oldSize = table_size;
    newSize = nextPrimeAfter(2 * table_size);
    this->h2_param = firstPrimeBefore(newSize);
    std::pair<std::string, int> *newTable = new std::pair<std::string, int>[newSize];
    this->table_size = newSize;
    for (int i = 0; i < newSize; i++)
    {
        newTable[i].first = "";
        newTable[i].second = -1;
    }
    for (int i = 0; i < oldSize; i++)
    {
        if (table[i].second != -1 && table[i].first != "")
        {
            int j = 0;
            int index = hash_function(table[i].first, j);
            while (newTable[index].second != -1 && newTable[index].first != "")
            {
                index = hash_function(table[i].first, j);
                j++;
            }
            newTable[index] = table[i];
        }
    }
    //table = new std::pair<std::string, int>[newSize];
    table = newTable;
    // if(newTable){
    //         delete[] newTable;
    // }
    //delete[] newTable;
}

int Article::hash_function(std::string &key, int i) const
{
    int converted, hash1, hash2, tableIndex = 0;
    converted = convertStrToInt(key);
    hash1 = h1(converted);
    hash2 = h2(converted);
    tableIndex = (hash1 + i * hash2) % table_size;
    return tableIndex;
}

int Article::h1(int key) const
{
    int count = 0, result;
    while (key)
    {
        if (key & 1)
        {
            count++;
        }
        key >>= 1;
    }
    result = count * h1_param;
    return result;
}

int Article::h2(int key) const
{
    int result, mod;
    mod = key % h2_param;
    result = h2_param - mod;
    return result;
}
