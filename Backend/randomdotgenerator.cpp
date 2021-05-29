#include <random>
#include "randomdotgenerator.h"

namespace Backend
{
    RandomDotgenerator::RandomDotgenerator(unsigned short goodDots, unsigned short badDots)
        : goodDots(goodDots),
          badDots(badDots)
    {
    }

    std::vector<std::shared_ptr<Dot>> RandomDotgenerator::Generate()
    {
        const int maxLine = 19;
        const int maxPositions = maxLine*maxLine;
        const double downCorrection = 9.0;

        if(goodDots + badDots > maxPositions)
        {
            throw std::exception("programmer mistake: cannot use more than maxPositions dots");
        }

        std::vector<int> allPositions;
        for (int i = 0; i<maxPositions; ++i)
        {
            allPositions.push_back(i);
        }

        std::vector<std::shared_ptr<Dot>> retval;

        auto createDot = [&](const bool kind)
        {
            auto index = RandomDotgenerator::GetRandomIndexUpToIncluding(allPositions.size() - 1);
            int position = allPositions[index];
            allPositions.erase(allPositions.begin() + index);

            auto x = static_cast<double>(position/maxLine) - downCorrection + RandomDotgenerator::GetRandomOffset();
            auto y = static_cast<double>(position%maxLine) - downCorrection + RandomDotgenerator::GetRandomOffset();

            retval.push_back(std::make_shared<Dot>(x, y, kind));
        };

        for (int i = 0; i<goodDots; ++i)
        {
            const bool goodDot = true;

            createDot(goodDot);
        }

        for (int i = 0; i<badDots; ++i)
        {
            const bool goodDot = false;

            createDot(goodDot);
        }

        return retval;
    }

    /* static class member */ size_t RandomDotgenerator::GetRandomIndexUpToIncluding(size_t upper)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dis(0, upper);
        return dis(gen);
    }

    /* static class member */ double RandomDotgenerator::GetRandomOffset()
    {
        const double maxOffset = 0.15;

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution  dis(-maxOffset, maxOffset);
        return dis(gen);
    }
}
