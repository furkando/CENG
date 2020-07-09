#ifndef HW4_TRACER_H
#define HW4_TRACER_H

#include "Player.h"

class Tracer : public Player
{
  public:
    Tracer(uint id, int x, int y);

    // Name     : Tracer
    // Priority : { UP, LEFT, DOWN, RIGHT, ATTACK }

    // Armor    : BRICK
    // Weapon   : SHOVEL
    // HP       : 100

    // DO NOT MODIFY THE UPPER PART
    // ADD OWN PUBLIC METHODS/PROPERTIES/OVERRIDES BELOW
    Armor getArmor() const override
    {
        return armor;
    }

    Weapon getWeapon() const override
    {
        return weapon;
    }

    std::vector<Move> getPriorityList() const override
    {
        return priority;
    }

    const std::string getFullName() const override
    {
        std::string name = "Tracer";
        name.append(Player::getBoardID());
        return name;
    }
    Color::Code getColorID() const override { return Color::FG_CYAN; }
};

#endif //HW4_TRACER_H
