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

    /*!
     * \class Evaluator
     * \brief The Evaluator class accepts an expression, a set of dots and an interval to create a graph
     * and find if any of the dots have been hit by the graph.
     */
    class Evaluator final
    {
    private:

        /*!
         * \brief Epsilon is the lower limit for increments etc.
         */
        const double Epsilon = 1e-4;

        /*!
         * \brief TargetDistance is the intended upper bound for x increments, which should still be above \ref Epsilon.
         */
        const double TargetDistance = 5e-3;

        /*!
         * \brief InitialIncrement is the initial increment for x.
         */
        const double InitialIncrement = 1e-3;

        /*!
         * \brief LargeIncrement is the increment to find branches.
         */
        const double LargeIncrement = 1e-2;

        std::shared_ptr<Expression> expression;
        const double minX;
        const double maxX;
        const double limit;

        std::vector<std::pair<std::vector<double>, std::vector<double>>> graphData;

        bool EvaluateWasCalled;
        bool AddPointToCurrentBranchAtWasCalled;

    public:
        /*!
         * \brief Initializes a new instance for the given expression and parameters.
         * \param expression The expression to evaluate.
         * \param minX The minimal x to consider.
         * \param maxX The maximal x to consider.
         * \param limit The absolute value of y after which the point shall not be included in the resulting data.
         */
        Evaluator(std::shared_ptr<Expression> expression, double minX, double maxX, double limit);
        ~Evaluator() = default;
        Evaluator(const Evaluator&) = delete;
        Evaluator& operator=(const Evaluator&) = delete;
        Evaluator(Evaluator&&) = delete;
        Evaluator& operator=(Evaluator&&) = delete;

        /*!
         * \brief Evaluate creates the graph data in branches and provides the graph.
         *
         * Provided for production purposes. Using this invalidates the use of AddPointToCurrentBranchAt().
         */
        std::vector<std::pair<std::vector<double>, std::vector<double>>> Evaluate();

        /*!
         * \brief AddPointAt evaluates the contained expression at the given x coordinate.
         * \param x The free coordinate at which to perform the evaluation.
         * \returns true if the evaluation yielded a point.
         *
         * Provided for testing purposes. Using this invalidates the use of Evaluate().
         */
        bool AddPointToCurrentBranchAt(double x);

        /*!
         * \brief GetGraph gets the graph data in branches.
         * \return The branches containing the graph data.
         *
         * Provided for production purposes. Using this invalidates the use of AddPointToCurrentBranchAt().
         */
        std::vector<std::pair<std::vector<double>, std::vector<double>>> GetGraph();

    private:
        void CreateGraph();
        void AddCompletePointToCurrentBranch(double x, double y);
        void EnsureAtLeastOneBranch();
        void WorkAnInterval(double (*direction)(double), double& x, double xInCurrentInterval, double& xOld);
    };

}

#endif // EVALUATOR_H
