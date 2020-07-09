#include <stdio.h>
#include <string.h>

// INPUT: inp must be NULL terminated char array //
// OUTPUT: out must be a big enough (64 bytes) char array //
void compute_hash(unsigned char *inp, unsigned char *out)
{
    unsigned char strbuf[64];
    unsigned long lowHash;
    unsigned long highHash;
    unsigned int i;
    unsigned char c, *cp;
    unsigned long tmp, tmpLow, tmpHigh;

    cp = inp;
    for (i = 0; i < 2000; i++)
    {
        highHash = 0;
        lowHash = 5381;
        while ((c = *cp++))
        {
            tmpLow = lowHash << 5;
            tmpHigh = (highHash << 5) + ((lowHash & 0xF8000000) >> 27);
            tmp = lowHash + tmpLow;
            if ((tmp < lowHash) || (tmp < tmpLow))
                tmpHigh += 1;
            tmpLow = tmp;
            tmpHigh = highHash + tmpHigh;
            tmp = tmpLow + c;
            if ((tmp < c) || (tmp < tmpLow))
                tmpHigh += 1;
            tmpLow = tmp;
            lowHash = tmpLow;
            highHash = tmpHigh;
        }
        sprintf(strbuf, "%lx%lx", highHash, lowHash);
        cp = strbuf;
        c = *cp;
        while (c)
        {
            if (c == '0')
            {
                cp++;
                c = *cp;
            }
            else
                break;
        }
    }
    strcpy(out, cp);
}
