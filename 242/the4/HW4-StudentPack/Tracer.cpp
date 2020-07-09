#include "Tracer.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Tracer::Tracer(uint id, int x, int y) : Player(id, x, y)
{

    name = "Tracer";
    priority = {UP, LEFT, DOWN, RIGHT, ATTACK};
    armor = BRICK;
    weapon = SHOVEL;
    HP = 100;
}