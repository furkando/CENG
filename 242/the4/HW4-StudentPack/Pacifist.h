#ifndef HW4_PACIFIST_H
#define HW4_PACIFIST_H

#include "Player.h"

class Pacifist : public Player
{
  public:
    Pacifist(uint id, int x, int y);

    // Name     : Pacifist
    // Priority : { UP, LEFT, DOWN, RIGHT }

    // Armor    : METAL
    // Weapon   : NOWEAPON
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
        std::string name = "Pacifist";
        name.append(Player::getBoardID());
        return name;
    }
    Color::Code getColorID() const override { return Color::FG_YELLOW; }
};

#endif //HW4_PACIFIST_H
