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

INCLUDEPATH += $$PWD\..\Include

HEADERS += \
    $$PWD/classes.h \
    $$PWD/deserializer.h \
    $$PWD/dot.h \
    $$PWD/dotgenerator.h \
    $$PWD/evaluator.h \
    $$PWD/functions.h \
    $$PWD/expression.h \
    $$PWD/basex.h \
    $$PWD/constant.h \
    $$PWD/game.h \
    $$PWD/mathhelper.h \
    $$PWD/parser.h \
    $$PWD/power.h \
    $$PWD/product.h \
    $$PWD/randomdotgenerator.h \
    $$PWD/repository.h \
    $$PWD/sum.h

SOURCES += \
    $$PWD/deserializer.cpp \
    $$PWD/dot.cpp \
    $$PWD/evaluator.cpp \
    $$PWD/functions.cpp \
    $$PWD/basex.cpp \
    $$PWD/constant.cpp \
    $$PWD/game.cpp \
    $$PWD/mathhelper.cpp \
    $$PWD/parser.cpp \
    $$PWD/power.cpp \
    $$PWD/product.cpp \
    $$PWD/randomdotgenerator.cpp \
    $$PWD/sum.cpp
