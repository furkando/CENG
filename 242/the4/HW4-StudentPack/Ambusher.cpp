#include "Ambusher.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Ambusher::Ambusher(uint id, int x, int y) : Player(id, x, y)
{
    name     = "Ambusher";
    priority = { ATTACK };
    armor    = NOARMOR;
    weapon   = SEMIAUTO;
    HP       = 100;
}