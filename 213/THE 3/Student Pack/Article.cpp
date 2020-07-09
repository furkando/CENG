#include "Article.h"

Article::Article(int table_size, int h1_param, int h2_param)
{
    int i;
    table = new std::pair<std::string, int>[table_size];
    std::pair<std::string, int> make_pair(EMPTY_KEY, EMPTY_INDEX);

    for (i = 0; i < table_size; i++)
    {

        table[i] = make_pair;
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
    int i = 0, index, p = 0;
    std::vector<int>::iterator it;
    if (nth == 1)
    {
        index = hash_function(key, i);
        if (table[index].first == key)
        {
            return table[index].second;
        }
    }
    for (int k = 1; k < nth; k++)
    {
        i++;
        index = hash_function(key, i);
        path.push_back(index);
    }
    for (int x = 0; x < table_size; x++)
    {
        if (table[index].first == key)
        {
            break;
        }
        i++;
        p++;
        if (p == table_size)
        {
            return -1;
        }
        index = hash_function(key, i);
        path.push_back(index);
        if (table[index].first == EMPTY_KEY && table[index].second != MARKED_INDEX)
        {
            return -1;
        }
    }
    it = path.end();
    it--;
    return table[*it].second;
}

int Article::insert(std::string key, int original_index)
{

    if (getLoadFactor() > MAX_LOAD_FACTOR)
    {
        expand_table();
    }
    int i = 0, index, temp;
    index = hash_function(key, i);
    while (table[index].second != -1 && table[index].first != "")
    {
        if (table[index].first == key && original_index < table[index].second)
        {
            temp = table[index].second;
            table[index].second = original_index;
            original_index = temp;
        }
        i++;
        index = hash_function(key, i);
    }
    table[index].first = key;
    table[index].second = original_index;
    n++;
    return i;
}

int Article::remove(std::string key, int nth)
{
    int i = 0, index;
    std::vector<int>::iterator it;
    std::vector<int> path;
    
    if (nth == 1)
    {
        index = hash_function(key, i);
        if (table[index].first == key)
        {
            table[index].first = EMPTY_KEY;
            table[index].second = MARKED_INDEX;

            return 0;
        }
    }
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
        if (table[index].first == EMPTY_KEY && table[index].second != MARKED_INDEX)
        {
            return -1;
        }
    }
    it = path.end();
    it--;
    table[*it].first = EMPTY_KEY;
    table[*it].second = MARKED_INDEX;
    n--;
    return i;
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
    std::fstream file;
    file.open(filepath.c_str());
    if (!file.is_open())
        return;
    int a = 0;
    std::string word;
    while (file >> word)
    {
        a++;
        insert(word, a);
    }
}

void Article::expand_table()
{

    int newSize, oldSize, temp;
    oldSize = table_size;
    newSize = nextPrimeAfter(2 * table_size);
    this->h2_param = firstPrimeBefore(newSize);
    std::pair<std::string, int> *newTable;
    newTable = new std::pair<std::string, int>[newSize];
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
                if (newTable[index].first == table[i].first && table[i].second < newTable[index].second)
                {
                    temp = newTable[index].second;
                    newTable[index].second = table[i].second;
                    table[i].second = temp;
                }
                index = hash_function(table[i].first, j);
                j++;
            }
            newTable[index] = table[i];
        }
    }
    std::pair<std::string, int> make_pair(EMPTY_KEY, EMPTY_INDEX);
    table = new std::pair<std::string, int>[newSize];
    for (int i = 0; i < newSize; i++)
    {
        table[i] = make_pair;
    }
    for (int i = 0; i < newSize; i++)
    {
        if (newTable[i].first != EMPTY_KEY || newTable[i].second == MARKED_INDEX)
        {
            table[i].first = newTable[i].first;
            table[i].second = newTable[i].second;
        }
    }
    delete[] newTable;
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
