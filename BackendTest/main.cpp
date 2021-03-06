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

#include "tst_fundamental.h"
#include "tst_basex.h"
#include "tst_constant.h"
#include "tst_equality.h"
#include "tst_sum.h"
#include "tst_product.h"
#include "tst_power.h"
#include "tst_parser.h"
#include "tst_evaluator.h"
#include "tst_dot.h"
#include "tst_randomdotgenerator.h"
#include "tst_game.h"
#include "tst_printingtest.h"
#include "tst_functions.h"
#include "tst_subsetgenerator.h"
#include "tst_fixeddotgenerator.h"
#include "tst_deserializer.h"
#include "tst_memoryrepository.h"
#include "tst_diskrepository.h"

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
