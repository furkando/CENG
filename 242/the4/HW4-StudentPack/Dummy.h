#ifndef HW4_DUMMY_H
#define HW4_DUMMY_H

#include "Player.h"

class Dummy : public Player
{
public:
  Dummy(uint id, int x, int y);

  // Name     : Dummy
  // Priority : { NOOP }

  // Armor    : NOARMOR
  // Weapon   : NOWEAPON
  // HP       : 1000

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
    std::string name = "Dummy";
    name.append(Player::getBoardID());
    return name;
  }
  Color::Code getColorID() const override { return Color::FG_GREEN; }
};

#endif //HW4_DUMMY_H
