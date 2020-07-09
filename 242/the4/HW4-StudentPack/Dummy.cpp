#include "Dummy.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Dummy::Dummy(uint id, int x, int y) : Player(id, x, y)
{
    name = "Dummy";
    priority = {NOOP};
    armor = NOARMOR;
    weapon = NOWEAPON;
    HP = 1000;
}