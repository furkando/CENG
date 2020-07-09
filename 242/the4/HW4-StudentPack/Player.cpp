#include "Player.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Player::Player(uint id, int x, int y) : id(id), coordinate(x, y)
{
}

Player::~Player()
{
}

uint Player::getID() const
{
    return id;
}
const Coordinate &Player::getCoord() const
{
    return coordinate;
}

int Player::getHP() const
{
    return HP;
}

/**
     * Board ID is two-decimal ID for the Player.
     *
     * Player ID = 0, 91
     * Board ID = "00", "91"
     *
     * @return BoardID of the user.
     */
std::string Player::getBoardID() const
{
    std::string boardID = "0";
    if (id < 10)
    {
        boardID.append(std::to_string(id));
    }
    else
    {
        boardID = std::to_string(id);
    }
    return boardID;
}

/**
     * Decide whether the player is dead.
     *
     * @return true if the player's hp <= 0, false otherwise.
     */
bool Player::isDead() const
{
    if (HP <= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/**
     * Execute the given move for the player's coordinates.
     *
     * Important note: Priority list does NOT matter here.
     *
     * NOOP and ATTACK are no-op.
     *
     * Do not forget to print the move.
     * "-playerFullName(playerHP)- moved UP/DOWN/LEFT/RIGHT."
     *
     * "Tracer00(100) moved UP."
     *
     * @param move Move to make.
     */
void Player::executeMove(Move move)
{
    coordinate = coordinate + move;
    if (move == 1)
    {
        std::cout << getFullName() << '(' << getHP() << ")"
                  << " moved "
                  << "UP." << std::endl;
    }
    else if (move == 2)
    {
        std::cout << getFullName() << '(' << getHP() << ")"
                  << " moved "
                  << "DOWN." << std::endl;
    }
    else if (move == 3)
    {
        std::cout << getFullName() << '(' << getHP() << ")"
                  << " moved "
                  << "LEFT." << std::endl;
    }
    else if (move == 4)
    {
        std::cout << getFullName() << '(' << getHP() << ")"
                  << " moved "
                  << "RIGHT." << std::endl;
    }
}

/**
     * Attack the given player, and decide whether the attacked player is dead.
     *
     * Important note: Priority list does NOT matter here.
     *
     * Formulae : RHS's HP -= max((LHS's damage - RHS's armor), 0)
     *
     * Do not forget to print the attack.
     *
     * "-lhsFullName(lhsHP)- ATTACKED -rhsFullName(rhsHP)-! (-damageDone-)
     *
     * "Tracer00(100) ATTACKED Tracer01(100)! (-10)"
     *
     * @param player Player to be attacked.
     * @return true if attacked player is dead, false otherwise.
     */
bool Player::attackTo(Player *player)
{
    int damageDone = Entity::damageForWeapon(weapon) - Entity::damageReductionForArmor(player->armor);
    if (id == player->getID())
    {
        return false;
    }
    std::cout << getFullName() << '(' << HP << ')' << " ATTACKED " << player->getFullName() << '(' << player->HP << ')' << "! (" << -damageDone << ')' << std::endl;
    player->HP -= std::max(damageDone, 0);
    if (player->HP <= 0)
    {
        std::cout << player->getFullName() << '(' << player->HP << ')' << " DIED." << std::endl;
        return true;
    }
    else
    {
        return false;
    }
}
