#ifndef GAMEPOC_H
#define GAMEPOC_H

#include <vector>
#include "dot.h"

class GamePoc
{
    std::vector<Dot> dots;
    std::vector<std::vector<std::pair<std::vector<double>, std::vector<double>>>> graphs;

public:
    GamePoc();
    ~GamePoc() = default;
    GamePoc(const GamePoc&) = delete;
    GamePoc(GamePoc&&) = delete;
    GamePoc& operator=(const GamePoc&) = delete;
    GamePoc& operator=(GamePoc&&) = delete;

    void Update();
    const std::vector<std::vector<std::pair<std::vector<double>, std::vector<double>>>>& GetGraphs() const;
    std::vector<Dot>& GetDots();

private:
    void SetupPOCItems();
    void SetupPOCGraphs();
    void SetupPOCDots();
};

#endif // GAMEPOC_H
