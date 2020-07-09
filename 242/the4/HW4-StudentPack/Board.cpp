#include "Board.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

Board::Board(uint boardSize, std::vector<Player *> *players) : boardSize(boardSize), players(players)
{
}

Board::~Board()
{
    // for (std::vector<Player *>::iterator it = players->begin(); it != players->end(); ++it)
    // {
    //     delete *it;
    // }
    //players->clear();
}

uint Board::getSize() const
{
    return boardSize;
}

/**
     * Decide whether the coordinate is in the board limits.
     *
     * @param coord Coordinate to search.
     * @return true if coord is in limits, false otherwise.
     */
bool Board::isCoordInBoard(const Coordinate &coord) const
{
    if (coord.x < 0 || coord.y < 0 || coord.x >= boardSize || coord.y >= boardSize)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/**
     * Decide whether the given coordinate is in storm.
     *
     * @param coord Coordinate to search.
     * @return true if covered in storm, false otherwise.
     */
bool Board::isStormInCoord(const Coordinate &coord) const
{
    int i;
    for (i = 0; i < storm.size(); i++)
    {
        if (storm[i] == coord)
        {
            return true;
        }
    }
    return false;
}

/**
     * Decide whether the given coordinate is the hill.
     *
     * @param coord Coordinate to search.
     * @return true if the coord is at the very center of the board, false otherwise.
     */
bool Board::isCoordHill(const Coordinate &coord) const
{
    if (boardSize % 2 == 1 && coord.x == boardSize / 2 && coord.y == boardSize / 2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
     * Indexing.
     *
     * Find the player in coordinate.
     *
     * nullptr if player does not exists in given coordinates, or !isCoordInBoard
     *
     * @param coord  The coordinate to search.
     * @return The player in coordinate.
     */
Player *Board::operator[](const Coordinate &coord) const
{
    int i;
    int size = players->size();
    if (!isCoordInBoard(coord))
    {
        return nullptr;
    }
    else
    {
        for (std::vector<Player *>::iterator it = players->begin(); it != players->end(); ++it)
        {
            if ((*it)->getCoord() == coord && !(*it)->isDead())
            {
                return *it;
            }
        }
        return nullptr;
    }
}

/**
     * Calculate the new coordinate with the given move and coordinate.
     *
     * NOOP and ATTACK are no-op, return coord.
     *
     * The new coordinate cannot be outside of the borders.
     * If it's the case, return coord.
     *
     * Also, if there's another player in the new coordinate,
     * return coord.
     *
     * @param move Move to be made.
     * @param coord The coordinate to be moved.
     * @return Calculated coordinate after the move.
     */
Coordinate Board::calculateCoordWithMove(Move move, const Coordinate &coord) const
{

    if (move == -1 || move == 0 || !isCoordInBoard(coord + move))
    {

        return coord;
    }
    else if ((*this)[coord + move] != nullptr)
    {
        if ((*this)[coord + move]->isDead())
        {
            return (coord + move);
        }
        else
        {
            return coord;
        }
    }
    else
    {
        return (coord + move);
    }
}

/**
     * Find the visible coordinates from given coordinate.
     *
     * Explanation: The immediate UP/DOWN/LEFT/RIGHT tiles must be calculated.
     *
     * There could be max of 4 tiles, and min of 2 tiles (on corners).
     * Every found coordinate must be in the board limits.
     *
     * If the given coordinate is not in board, return a vector with size = 0. Order does NOT matter.
     *
     * Example:
     *
     * 01----
     * 02HH--
     * ------
     *
     * visibleCoordsFromCoord(Coordinate(0, 0)) == { (1, 0), (0, 1) }
     * visibleCoordsFromCoord(Coordinate(1, 1)) == { (1, 0), (2, 1), (1, 2), (0, 1) }
     * visibleCoordsFromCoord(Coordinate(-1, 0)) == { }
     *
     * @param coord The coordinate to search.
     * @return All coordinates visible.
     */
std::vector<Coordinate> Board::visibleCoordsFromCoord(const Coordinate &coord) const
{
    std::vector<Coordinate> visibles;
    if (!isCoordInBoard(coord))
    {
        return visibles;
    }
    if (isCoordInBoard(coord + UP))
    {
        visibles.push_back(coord + UP);
    }
    if (isCoordInBoard(coord + LEFT))
    {
        visibles.push_back(coord + LEFT);
    }
    if (isCoordInBoard(coord + RIGHT))
    {
        visibles.push_back(coord + RIGHT);
    }
    if (isCoordInBoard(coord + DOWN))
    {
        visibles.push_back(coord + DOWN);
    }
    return visibles;
}

/**
     * Calculate the storm according to the currentRound.
     *
     * @param currentRound The current round being played.
     */
void Board::updateStorm(uint currentRound)
{
    int i, size;
    size = currentRound / STORM_WIDTH_COEFF;
    Coordinate start = Coordinate(size - 1, size - 1);
    if (currentRound > 1 && currentRound % STORM_WIDTH_COEFF == 1 && currentRound <= STORM_WIDTH_COEFF * (boardSize / 2 + 1))
    {
        for (i = 0; i < (boardSize - (2 * (size - 1))); i++)
        {
            storm.push_back(start);
            start.y += 1;
        }
        start = Coordinate(size - 1, size - 1);
        for (i = 0; i < (boardSize - (2 * (size - 1))); i++)
        {
            storm.push_back(start);
            start.x += 1;
        }
        start = Coordinate(size - 1, boardSize - size);
        for (i = 0; i < (boardSize - (2 * (size - 1))); i++)
        {
            storm.push_back(start);
            start.x += 1;
        }
        start = Coordinate(boardSize - size, size - 1);
        for (i = 0; i < (boardSize - (2 * (size - 1))); i++)
        {
            storm.push_back(start);
            start.y += 1;
        }
    }
}