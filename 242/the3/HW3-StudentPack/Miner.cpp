#include "Miner.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Miner::Miner(std::string name)
{
    this->name = name;
}

int Miner::getNextAvailableBlockchainID() const
{
    int a = 0;
    if (miner.size() == 0)
    {
        return a;
    }
    a = miner[miner.size() - 1]->getID() + 1;
    return a;
}

Miner::~Miner()
{
    int i = 0, j = 0;
    int size = miner.size();
    for (i = 0; i < size; i++)
    {
        if (miner[i]->getSoft2() == true)
        {
            miner[i]->setHead(NULL);
        }
    }
    for (i = 0; i < size; i++)
    {
        delete miner[i];
    }
    miner.clear();
}

void Miner::createNewBlockchain()
{
    Blockchain *a = new Blockchain(getNextAvailableBlockchainID());
    miner.push_back(a);
}

void Miner::mineUntil(int cycleCount)
{
    int i = 0, j = 0;
    for (i = 0; i < cycleCount; i++)
    {
        for (j = 0; j < miner.size(); j++)
        {
            ++*miner[j];
        }
    }
}

void Miner::demineUntil(int cycleCount)
{
    int i = 0, j = 0;
    for (i = 0; i < cycleCount; i++)
    {
        for (j = 0; j < miner.size(); j++)
        {
            if (miner[j]->getSoft() != true)

            {
                --*miner[j];
            }
        }
    }
}

double Miner::getTotalValue() const
{
    int i = 0;
    double sum = 0;
    for (i = 0; i < miner.size(); i++)
    {
        if (miner[i]->getSoft2() != true)
        {
            sum += miner[i]->getTotalValue();
        }
    }
    return sum;
}

long Miner::getBlockchainCount() const
{
    return miner.size();
}

Blockchain *Miner::operator[](int id) const
{
    if (miner.size() == 0 || id > miner.size())
    {
        return nullptr;
    }
    return miner[id];
}

bool Miner::softFork(int blockchainID)
{
    if (miner.size() == 0 || blockchainID > miner.size())
    {
        return false;
    }
    else
    {
        Blockchain *tmp = miner[blockchainID];
        Koin *node = tmp->getHead();
        Blockchain *a;
        if (node == NULL)
        {
            a = new Blockchain(getNextAvailableBlockchainID());
        }
        else
        {
            while (node != NULL)
            {
                if (node->getNext() == NULL)
                {
                    break;
                }
                node = node->getNext();
            }
            a = new Blockchain(getNextAvailableBlockchainID(), node);
        }
        a->setSoft2(true);
        a->setSoft(true);
        tmp->setSoft(true);
        miner.push_back(a);
        return true;
    }
}

bool Miner::hardFork(int blockchainID)
{
    if (miner.size() == 0 || blockchainID > miner.size())
    {
        return false;
    }
    else
    {
        Blockchain *tmp = miner[blockchainID];
        Koin *node = tmp->getHead();
        Blockchain *a;
        if (node == NULL)
        {
            a = new Blockchain(getNextAvailableBlockchainID());
        }
        else
        {
            while (node != NULL)
            {
                if (node->getNext() == NULL)
                {
                    break;
                }
                node = node->getNext();
            }
            Koin *tmp = new Koin(node->getValue());
            a = new Blockchain(getNextAvailableBlockchainID(), tmp);
        }
        miner.push_back(a);
        return true;
    }
}

std::ostream &operator<<(std::ostream &os, const Miner &miner)
{
    Miner *klon = const_cast<Miner *>(&miner);
    Koin *a;
    os.precision(Utilizer::koinPrintPrecision());
    os << "-- BEGIN MINER --"
       << "\n";
    os << "Miner name: " << klon->getName() << "\n";
    os << "Blockchain count: " << klon->getBlockchainCount() << "\n";
    os << std::fixed;
    os << "Total value: " << klon->getTotalValue() << "\n\n";

    for (int i = 0; i < klon->getBlockchainCount(); i++)
    {
        a = klon->getBlock(i)->getHead();
        os.precision(Utilizer::koinPrintPrecision());
        os << "Block " << klon->getBlock(i)->getID() << ": ";
        if (a)
        {
            while (a)
            {
                os << std::fixed;
                os << a->getValue() << "--";
                a = a->getNext();
            }
            os << "|";
            os << "(" << klon->getBlock(i)->getTotalValue() << ")";
        }
        else
        {
            os << "Empty.";
        }
        os << "\n";
    }
    os << "\n"
       << "-- END MINER --\n";
    return os;
}

std::string Miner::getName() const
{
    return name;
}

Blockchain* Miner::getBlock(int id)
{
    return miner[id];
}
