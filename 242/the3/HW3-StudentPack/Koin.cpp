#include "Koin.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Koin::Koin(double value)
{
    this->value = value;
    this->next = NULL;
}

Koin::Koin(const Koin &rhs)
{
    this->value = rhs.value;
    this->next = rhs.next;
}

Koin &Koin::operator=(const Koin &rhs)
{
    this->value = rhs.value;
    this->next = rhs.next;
    return *this;
}

Koin::~Koin()
{
#if 0
/*
    Koin *tmp;
    tmp = this->next;
    while(tmp) {
        tmp = tmp->next;
    }
    
    delete this->next;
*/
    Koin *node = this->next;
    while (node != NULL)
    {
        Koin *tmp = node;
        node = node->next;
        delete tmp;
    }
#endif
}

double Koin::getValue() const
{
    return value;
}

Koin *Koin::getNext() const
{
    return next;
}

void Koin::setNext(Koin *next)
{
    this->next = next;
}

bool Koin::operator==(const Koin &rhs) const
{
    if ((this->value - Utilizer::doubleSensitivity()) <= rhs.value && rhs.value <= (this->value + Utilizer::doubleSensitivity()))
    {
        if (this->next != rhs.next)
        {
            return false;
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool Koin::operator!=(const Koin &rhs) const
{
    if ((this->value - Utilizer::doubleSensitivity()) <= rhs.value && rhs.value <= (this->value + Utilizer::doubleSensitivity()))
    {
        if (this->next != rhs.next)
        {
            return true;
        }
        return false;
    }
    else
    {
        return true;
    }
}

Koin &Koin::operator*=(int multiplier)
{
    value *= multiplier;
    return *this;
}

Koin &Koin::operator/=(int divisor)
{
    value /= divisor;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Koin &koin)
{
    Koin *klon = const_cast<Koin *>(&koin);
    os.precision(Utilizer::koinPrintPrecision());
    while (klon)
    {
        os << std::fixed;
        os << klon->getValue() << "--";
        klon = klon->getNext();
    }
    os << "|";
    return os;
}
