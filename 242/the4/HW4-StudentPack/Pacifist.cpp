#include "Pacifist.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Pacifist::Pacifist(uint id, int x, int y) : Player(id, x, y)
{
    name = "Pacifist";
    priority = {UP, LEFT, DOWN, RIGHT};
    armor = METAL;
    weapon = NOWEAPON;
    HP = 100;
}