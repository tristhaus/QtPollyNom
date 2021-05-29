#ifndef RANDOMDOTGENERATOR_H
#define RANDOMDOTGENERATOR_H

#include "dotgenerator.h"

namespace Backend
{
    /*!
     * \class RandomDotgenerator
     * \brief The RandomDotgenerator class creates dots a grid points using a offset. It can generate 361 dots total.
     */
    class RandomDotgenerator final : public DotGenerator
    {
    private:
        unsigned short goodDots;
        unsigned short badDots;
    public:
        /*!
         * \brief Initializes a new instance.
         * \param goodDots The number of good dots to create.
         * \param badDots The number of bad dots to create.
         */
        RandomDotgenerator(unsigned short goodDots, unsigned short badDots);

        /*!
         * \reimp
         */
        virtual std::vector<std::shared_ptr<Dot>> Generate();

    private:
        static size_t GetRandomIndexUpToIncluding(size_t upper);
        static double GetRandomOffset();
    };
}


#endif // RANDOMDOTGENERATOR_H
