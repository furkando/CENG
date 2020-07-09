#ifndef HW4_BERSERK_H
#define HW4_BERSERK_H

#include "Player.h"

class Berserk : public Player
{
public:
  Berserk(uint id, int x, int y);

  // Name     : Berserk
  // Priority : { ATTACK, UP, LEFT, DOWN, RIGHT }

  // Armor    : WOODEN
  // Weapon   : PISTOL
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
    std::string name = "Berserk";
    name.append(Player::getBoardID());
    return name;
  }
  Color::Code getColorID() const override { return Color::FG_BLUE; }
};

#endif //HW4_BERSERK_H
