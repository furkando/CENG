#include "GameEngine.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

/**
     * Constructor.
     *
     * GameEngine "owns" these players.
     * GameEngine also "owns" the vector.
     *
     * @param boardSize The side length of the board.
     * @param players All players to participate in the game.
     */
GameEngine::GameEngine(uint boardSize, std::vector<Player *> *players) : board(Board(boardSize, players))
{
    currentRound = 0;
}

GameEngine::~GameEngine()
{

    Player *winner = getWinnerPlayer();
    //delete winner;
    // delete winner;
    if (winner != nullptr)
        board.deletePlayer(winner);
    delete board.getPlayers();
}

const Board &GameEngine::getBoard() const
{
    return board;
}

/**
     * Indexing.
     *
     * Find the player with given ID.
     *
     * nullptr if not exists.
     *
     * @param id ID of the player.
     * @return The player with given ID.
     */
Player *GameEngine::operator[](uint id) const
{
    for (std::vector<Player *>::iterator it = board.getPlayers()->begin(); it != board.getPlayers()->end(); ++it)
    {
        if ((*it)->getID() == id)
        {
            return *it;
        }
    }
    return nullptr;
}

/**
     * Decide whether the game is finished.
     *
     * @return true if there is only 1 player (alive), on top of the hill; or there are 0 players. False otherwise.
     */
bool GameEngine::isFinished() const
{
    int numOfPlayers = 0;
    std::vector<Coordinate> coor;
    for (std::vector<Player *>::iterator it = board.getPlayers()->begin(); it != board.getPlayers()->end(); ++it)
    {
        if (!(*it)->isDead())
        {
            coor.push_back((*it)->getCoord());
            numOfPlayers += 1;
        }
    }
    if (numOfPlayers > 1)
    {
        return false;
    }
    else
    {
        if (numOfPlayers == 0)
        {
            return true;
        }
        else
        {
            if (board.isCoordHill(coor[0]))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

/**
     * Take turn for every player.
     *
     * How-to:
     * - Announce turn start (cout).
     * Example: -- START ROUND 1  --
     * - board.updateStorm(currentRound)
     * - For every player (sorted according to their IDs) that isn't dead (HP <= 0):
     *      - takeTurnForPlayer(player).
     * - Announce turn end (cout).
     * Example: -- END ROUND 1 --
     */
void GameEngine::takeTurn()
{
    currentRound += 1;
    int before = 0, after = 0;
    int size = board.getPlayers()->size();
    std::cout << "-- START ROUND " << currentRound << " --" << std::endl;
    board.updateStorm(currentRound);
    for (std::vector<Player *>::iterator it = board.getPlayers()->begin(); it != board.getPlayers()->end(); ++it)
    {
        //if (!(*it)->isDead())
        //{
        before = (*it)->getID();
        takeTurnForPlayer((*it)->getID());
        if (it == board.getPlayers()->end())
            break;
        after = (*it)->getID();
        if (before != after)
            --it;
        //}
    }
    std::cout << "-- END ROUND " << currentRound << " --" << std::endl;
}

/**
     * The most important (algorithm-wise) method.
     *
     * How-to:
     * - Get player with ID. Return NOOP if not exists.
     * - Get player's priority list.
     * - Get player's visibility from the board (visibleCoordsFromCoord).
     *
     * - If the player is in the storm (isStormInCoord), announce the damage and give player stormDamage.
     * - Example: Tracer01(10) is STORMED! (-10)
     *
     * - If dead, announce the death, remove player from the board/list/anywhere, and return NOOP.
     * - Example: Tracer01(0) DIED.
     *
     * - For MOVE in player's priority list:
     *          - If the MOVE is NOOP:
     *              - return NOOP.
     *          - Else if the MOVE is ATTACK:
     *              - Get all players that this player can attack (board[coord] for each in visibilityCoords).
     *              - If none, continue.
     *              - Else:
     *                  - Pick the one with most priority (lowest ID).
     *                  - isPlayerDead = player.attackTo(thatPlayer).
     *                  - if isPlayerIsDead:
     *                      - announce the death.
     *                      - remove thatPlayer from the board/list/anywhere.
     *                  - return ATTACK.
     *          - Else (UP/DOWN/LEFT/RIGHT):
     *              - calculateCoordWithMove(move).
     *              - If the new coordinate is different than the player's (meaning it's able to do that move)
     *              AND the player is getting closer to the hill;
     *                  - player.executeMove(MOVE).
     *                  - return MOVE.
     *              - Else:
     *                  - continue.
     *
     * // If the priority list is exhausted;
     * return NOOP.
     *
     * @param player Player ID to move.
     * @return move Decided move.
     */
Move GameEngine::takeTurnForPlayer(uint playerID)
{
    std::vector<Move> priority;
    std::vector<Coordinate> visibles;
    std::vector<Player *> players;
    Player *main = (*this)[playerID];
    Player *low;
    bool isDead;
    int i = 0, j;
    int size;
    int damage;
    Coordinate newCoord(0, 0);
    Coordinate Hill(board.getSize() / 2, board.getSize() / 2);
    if (main == nullptr)
    {
        return NOOP;
    }
    else
    {
        priority = main->getPriorityList();
        visibles = board.visibleCoordsFromCoord(main->getCoord());
        size = priority.size();

        if (board.isStormInCoord(main->getCoord()))
        {
            damage = (((currentRound - 1) / STORM_WIDTH_COEFF) * STORM_DAMAGE_PER_LEVEL);
            std::cout << main->getFullName() << '(' << main->getHP() << ')' << " is STORMED! "
                      << "(" << -damage << ')' << std::endl;
            main->setHP(main->getHP() - damage);
            if (main->isDead())
            {
                std::cout << main->getFullName() << '(' << main->getHP() << ")"
                          << " DIED." << std::endl;
                board.deletePlayer(main);
                //main = nullptr;
                //delete main;
                return NOOP;
            }
        }
        for (i = 0; i < size; i++)
        {
            if (priority[i] == NOOP)
            {
                return NOOP;
            }
            else if (priority[i] == ATTACK)
            {
                for (j = 0; j < visibles.size(); j++)
                {

                    if (board[visibles[j]] != nullptr && !board[visibles[j]]->isDead())
                    {
                        players.push_back(board[visibles[j]]);
                    }
                }
                if (players.empty())
                {

                    continue;
                }
                else
                {
                    low = players[0];
                    for (j = 1; j < players.size(); j++)
                    {
                        if (players[j]->getID() < low->getID())
                        {
                            low = players[j];
                        }
                    }

                    isDead = main->attackTo(low);
                    if (isDead)
                    {
                        board.deletePlayer(low);
                        // low = nullptr;
                        // delete low;
                    }
                    return ATTACK;
                }
            }
            else
            {
                newCoord = board.calculateCoordWithMove(priority[i], main->getCoord());
                if (newCoord != main->getCoord() && (newCoord - Hill < main->getCoord() - Hill))
                {
                    main->executeMove(priority[i]);
                    return priority[i];
                }
                else
                {
                    continue;
                }
            }
        }
        return NOOP;
    }
}

/**
     * Find the winner player.
     *
     * nullptr if there are 0 players left, or the game isn't finished yet.
     *
     * @return The winner player.
     */
Player *GameEngine::getWinnerPlayer() const
{
    int numOfPlayers = board.getPlayers()->size();
    if (numOfPlayers == 1 && isFinished() == 1)
    {
        Player* winner = (*board.getPlayers())[0];
        return winner;
    }
    else
    {
        return nullptr;
    }
}
