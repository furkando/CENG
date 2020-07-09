#ifndef HW4_AMBUSHER_H
#define HW4_AMBUSHER_H

#include "Player.h"

class Ambusher : public Player
{
  public:
    Ambusher(uint id, int x, int y);

    // Name     : Ambusher
    // Priority : { ATTACK }

    // Armor    : NOARMOR
    // Weapon   : SEMIAUTO
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
        std::string name = "Ambusher";
        name.append(Player::getBoardID());
        return name;
    }

    Color::Code getColorID() const override { return Color::FG_RED; }
};

#endif //HW4_AMBUSHER_H
