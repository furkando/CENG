#include "GameParser.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

/**
     * Parse the file with given name and create players accordingly.
     *
     * GameParser DOES NOT have any responsibility over these Players.
     *
     * Note: The file will always exists, and there will be no erroneous input.
     *
     * @param filename The name of the file to be parsed.
     * @return  pair.first: Board size.
     *          pair.second: The vector of the constructed players.
     */
std::pair<int, std::vector<Player *> *> GameParser::parseFileWithName(const std::string &filename)
{
    std::pair<int, std::vector<Player *> *> result;
    std::vector<Player *> *players = new std::vector<Player *>();
    std::string boardSize;
    int board;
    std::string playerCount;
    int player;
    std::string line;

    std::string sid;
    std::string sx;
    std::string sy;
    Player *newPlayer;

    uint id;
    int x;
    int y;
    std::string type;

    std::ifstream readFile(filename);

    std::getline(readFile, line);
    std::istringstream lineStream(line);

    std::getline(lineStream, boardSize, ':');
    std::getline(lineStream, boardSize, ' ');
    std::getline(lineStream, boardSize);
    // boardSize = boardSize[1];
    board = std::stoi(boardSize);
    result.first = board;
    std::getline(readFile, line);
    std::istringstream lineStream2(line);
    std::getline(lineStream2, playerCount, ':');
    std::getline(lineStream2, playerCount, ' ');
    std::getline(lineStream2, playerCount);
    // playerCount = playerCount[1];
    player = std::stoi(playerCount);

    // std::cout << board << player << std::endl;
    for (int i = 0; i < player; i++)
    {
        std::getline(readFile, line);
        std::istringstream lineStream3(line);
        std::getline(lineStream3, sid, ':');
        std::getline(lineStream3, type, ':');
        std::getline(lineStream3, type, ':');
        std::getline(lineStream3, sx, ':');
        std::getline(lineStream3, sx, ':');
        std::getline(lineStream3, sy, ':');
        std::getline(lineStream3, sy);
        id = std::stoi(sid);
        x = std::stoi(sx);
        y = std::stoi(sy);
        if (type == "Ambusher")
        {
            newPlayer = new Ambusher(id, x, y);
            players->push_back(newPlayer);
        }
        else if (type == "Berserk")
        {
            newPlayer = new Berserk(id, x, y);
            players->push_back(newPlayer);
        }
        else if (type == "Dummy")
        {
            newPlayer = new Dummy(id, x, y);
            players->push_back(newPlayer);
        }
        else if (type == "Pacifist")
        {
            newPlayer = new Pacifist(id, x, y);
            players->push_back(newPlayer);
        }
        else
        {
            newPlayer = new Tracer(id, x, y);
            players->push_back(newPlayer);
        }
    }
    result.second = players;
    return result;
}