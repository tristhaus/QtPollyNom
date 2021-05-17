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

#include "gamepoc.h"

GamePoc::GamePoc()
{
    this->SetupPOCItems();
}

void GamePoc::Update()
{
    this->SetupPOCGraphs();
    dots[0].SetIsActive(true);
    dots[1].SetIsActive(true);
}

const std::vector<std::vector<std::pair<std::vector<double>, std::vector<double>>>>& GamePoc::GetGraphs() const
{
    return graphs;
}

std::vector<Dot>& GamePoc::GetDots()
{
    return dots;
}

void GamePoc::SetupPOCItems()
{
    SetupPOCGraphs();
    SetupPOCDots();
}

void GamePoc::SetupPOCGraphs()
{
    this->graphs.clear();

    std::vector<double> firstBranchHyperbolaX;
    std::vector<double> firstBranchHyperbolaY;
    std::vector<double> secondBranchHyperbolaX;
    std::vector<double> secondBranchHyperbolaY;

    std::vector<double> branchPolynomialX;
    std::vector<double> branchPolynomialY;

    const int grain = 10000;
    for(int i=0; i<(grain + 1); ++i)
    {
        double x1 = -10.0 + 9.999 * i / grain;
        double y1 = 1/x1;

        firstBranchHyperbolaX.push_back(x1);
        firstBranchHyperbolaY.push_back(y1);

        double x2 = 0.0 + 9.999 * i / grain;
        double y2 = 1/x2;

        secondBranchHyperbolaX.push_back(x2);
        secondBranchHyperbolaY.push_back(y2);

        double x3 = -10.0 + 20.0 * i / grain;
        double y3 = (x3-3.0)*(x3+4.0);

        branchPolynomialX.push_back(x3);
        branchPolynomialY.push_back(y3);
    }

    std::pair<std::vector<double>, std::vector<double>> firstBranchHyperbola = std::make_pair(firstBranchHyperbolaX, firstBranchHyperbolaY);
    std::pair<std::vector<double>, std::vector<double>> secondBranchHyperbola = std::make_pair(secondBranchHyperbolaX, secondBranchHyperbolaY);

    std::pair<std::vector<double>, std::vector<double>> branchPolynomial = std::make_pair(branchPolynomialX, branchPolynomialY);

    std::vector<std::pair<std::vector<double>, std::vector<double>>> graphHyperbola;
    graphHyperbola.push_back(firstBranchHyperbola);
    graphHyperbola.push_back(secondBranchHyperbola);

    std::vector<std::pair<std::vector<double>, std::vector<double>>> graphPolynomial;
    graphPolynomial.push_back(branchPolynomial);

    this->graphs.push_back(graphHyperbola);
    this->graphs.push_back(graphPolynomial);
}

void GamePoc::SetupPOCDots()
{
    // dots to be made active
    dots.emplace_back(Dot(1.0, 1.0, true));
    dots.emplace_back(Dot(-8.0, -0.25, true));

    // dots to remain inactive
    dots.emplace_back(Dot(5.0, -5.0, true));
    dots.emplace_back(Dot(2.5, 5.0, false));
}
