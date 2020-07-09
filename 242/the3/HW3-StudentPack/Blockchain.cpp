#include "Blockchain.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Blockchain::Blockchain(int id) : id(id)
{
    this->head = NULL;
    this->soft = false;
    this->soft2 = false;
}

Blockchain::Blockchain(int id, Koin *head) : id(id)
{
    this->head = head;    
    this->soft = false;
    this->soft2 = false;
}

Blockchain::Blockchain(const Blockchain &rhs) : id(rhs.id)
{
    head = NULL;
    Koin *n = new Koin(rhs.head->getValue());
    Koin *o = rhs.head->getNext();
    this->head = n;
    while (o)
    {
        n->setNext(new Koin(o->getValue()));
        n = n->getNext();
        o = o->getNext();
    }
}

Blockchain &Blockchain::operator=(Blockchain &&rhs) noexcept
{
    if (this != &rhs)
    {
        delete head;
        head = rhs.head;
        rhs.head = nullptr;
    }

    return *this;
}

Blockchain::~Blockchain()
{
    Koin *node = head;
    while (node != NULL)
    {
        Koin *tmp = node;
        node = node->getNext();
        delete tmp;
    }
}

int Blockchain::getID() const
{
    return id;
}

Koin *Blockchain::getHead() const
{
    return head;
}

double Blockchain::getTotalValue() const
{
    double a = 0;
    Koin *node = head;
    while (node != NULL)
    {
        a += node->getValue();
        node = node->getNext();
    }
    return a;
}

long Blockchain::getChainLength() const
{

    long a = 0;
    Koin *node = head;
    while (node != NULL)
    {
        a += 1;
        node = node->getNext();
    }
    return a;
}

void Blockchain::operator++()
{
    Koin *minedKoin = new Koin(Utilizer::fetchRandomValue());
    Koin *node = head;
    if (!node)
    {
        head = minedKoin;
    }
    else
    {
        while (node->getNext())
            node = node->getNext();
        node->setNext(minedKoin);
    }
}

void Blockchain::operator--()
{
    if (head == nullptr)
        return;

    if (head->getNext() == NULL)
    {
        delete head;
        head = NULL;
        return;
    }

    Koin *currNode = head;
    while (currNode->getNext() && currNode->getNext()->getNext() != NULL)
    {
        currNode = currNode->getNext();
    }
    delete currNode->getNext();
    currNode->setNext(NULL);
}

Blockchain &Blockchain::operator*=(int multiplier)
{
    Koin *o = head;
    Koin *n = head;
    while (o)
    {
        *o *= multiplier;
        o = o->getNext();
    }
    this->head = n;
    return *this;
}

Blockchain &Blockchain::operator/=(int divisor)
{
    Koin *o = head;
    Koin *n = head;
    while (o)
    {
        *o /= divisor;
        o = o->getNext();
    }
    this->head = n;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Blockchain &blockchain)
{
    Blockchain *klon = const_cast<Blockchain *>(&blockchain);
    Koin *a = klon->getHead();
    os.precision(Utilizer::koinPrintPrecision());
    os << "Block " << klon->getID() << ": ";
    if (a)
    {
        while (a)
        {
            os << std::fixed;
            os << a->getValue() << "--";
            a = a->getNext();
        }
        os << "|";
        os << "(" << klon->getTotalValue() << ")";
    }
    else
    {
        os << "Empty.";
    }
    return os;
}

void Blockchain::setSoft(bool a)
{
    this->soft = a;
}

bool Blockchain::getSoft()
{
    return soft;
}

void Blockchain::setSoft2(bool a)
{
    this->soft2 = a;
}

bool Blockchain::getSoft2()
{
    return soft2;
}

void Blockchain::setHead(Koin *head)
{
    this->head = head;
}