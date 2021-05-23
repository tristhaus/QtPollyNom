/*
 * This file is part of QtPollyNom.
 *
 * QtPollyNom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QtPollyNom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QtPollyNom.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <memory>
#include "expression.h"
#include "dot.h"

namespace Backend {

    class Evaluator
    {
    private:
        std::shared_ptr<Expression> expression;
        // std::vector<Dot&> dots; // todo
        std::pair<std::vector<double>, std::vector<double>> graphData;

    public:
        Evaluator(std::shared_ptr<Expression> expression/*, std::vector<Dot&> & dots*/);
        ~Evaluator() = default;
        Evaluator(const Evaluator&) = delete;
        Evaluator& operator=(const Evaluator&) = delete;
        Evaluator(Evaluator&&) = delete;
        Evaluator& operator=(Evaluator&&) = delete;

        std::pair<std::vector<double>, std::vector<double>> Evaluate();

        /*!
         * \brief AddPointAt evaluates the contained expression at the given x coordinate.
         * \param x The free coordinate at which to perform the evaluation.
         * \returns true if the evaluation yielded a point.
         *
         * Provided for testing purposes.
         */
        bool AddPointAt(double x);

    private:
        void AddCompletePoint(double x, double y);
    };

}

#endif // EVALUATOR_H
