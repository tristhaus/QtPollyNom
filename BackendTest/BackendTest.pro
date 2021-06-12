#
# This file is part of QtPollyNom.
# 
# QtPollyNom is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# QtPollyNom is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with QtPollyNom.  If not, see <http://www.gnu.org/licenses/>.
# 
#

include(../Backend/backend.pri)

# you may need to change this
GOOGLETEST_DIR = D:\\VSProject\\googletest

include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11 thread
CONFIG -= app_bundle

HEADERS += \
        fixeddotgenerator.h \
        testexpressionbuilder.h \
        tst_basex.h \
        tst_constant.h \
        tst_dot.h \
        tst_equality.h \
        tst_evaluating.h \
        tst_evaluator.h \
        tst_functions.h \
        tst_fundamental.h \
        tst_game.h \
        tst_parser.h \
        tst_power.h \
        tst_printingtest.h \
        tst_product.h \
        tst_randomdotgenerator.h \
        tst_sum.h

SOURCES += \
        fixeddotgenerator.cpp \
        main.cpp \
        testexpressionbuilder.cpp
