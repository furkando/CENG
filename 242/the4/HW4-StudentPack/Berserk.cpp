#include "Berserk.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Berserk::Berserk(uint id, int x, int y) : Player(id, x, y)
{
    name     = "Berserk";
    priority = { ATTACK, UP, LEFT, DOWN, RIGHT };
    armor    = WOODEN;
    weapon   = PISTOL;
    HP       = 100;
}
