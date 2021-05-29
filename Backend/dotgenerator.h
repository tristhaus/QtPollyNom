#ifndef DOTGENERATOR_H
#define DOTGENERATOR_H

#include "dot.h"

namespace Backend
{
    /*!
     * \brief The DotGenerator class forms the base for ways to generate dots.
     *
     * It is abstract.
     */
    class DotGenerator
    {
    public:
        explicit DotGenerator()
        {
        }
        virtual ~DotGenerator()
        {
        }
        DotGenerator(const DotGenerator&) = delete;
        DotGenerator(DotGenerator&&) = delete;
        DotGenerator& operator=(const DotGenerator&) = delete;
        DotGenerator& operator=(DotGenerator&&) = delete;

        /*!
         * \brief Perform the generation of dots.
         * \return A collection of good and/or bad dots.
         */
        virtual std::vector<std::shared_ptr<Dot>> Generate() = 0;
    };
}

#endif // DOTGENERATOR_H
