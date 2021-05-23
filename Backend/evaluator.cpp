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

#include <algorithm>
#include "evaluator.h"

namespace Backend {

    Evaluator::Evaluator(std::shared_ptr<Expression> expression/*, std::vector<Dot&> & dots*/)
        : expression(expression),
          /*dots(dots),*/
          graphData(std::make_pair(std::vector<double>(),std::vector<double>()))
    {
    }

    std::pair<std::vector<double>, std::vector<double> > Evaluator::Evaluate()
    {
        // TODO : the actual logic
        return this->graphData;
    }

    bool Evaluator::AddPointAt(double x)
    {
        auto evaluationResult = this->expression->Evaluate(x);
        if(evaluationResult.has_value())
        {
            this->AddCompletePoint(x, evaluationResult.value());
            return true;
        }
        else
        {
            return false;
        }
    }

    void Evaluator::AddCompletePoint(double x, double y)
    {
        // keep list sorted by x value
        if(graphData.first.empty() || x > graphData.first.back())
        {
            graphData.first.emplace_back(x);
            graphData.second.emplace_back(y);
            return;
        }

        // we need to locate the correct position
        auto xBegin = graphData.first.begin();
        auto xEnd = graphData.first.end();
        auto xIt = std::lower_bound(xBegin, xEnd, x);

        auto yIt = graphData.second.begin() + (xIt - xBegin);

        graphData.first.insert(xIt, x);
        graphData.second.insert(yIt, y);
    }

}
