# QtPollyNom

Copyright tristhaus 2021 and later.

![main](/../screenshot/screenshot.png?raw=true)

## For Users

Currently, this is an alpha-stage demo of what QtPollyNom shall become. A pre-release of the current status has been made available. For comparison and an impression of the eventual feature set, see [PollyNom](https://github.com/tristhaus/pollynom). See Release tab for up-to-date information on what is already supported. You may contact me at `qtpollynom` at `gmail.com` for comments on the program.

## For Developers

I am currently not looking for contributors since this is primarily a showcase. This may change at a later stage.

In order to compile the [frontend tests](/MainWindowTest/), you need to define one of two preprocessor constants
 * `_SKIP_LONG_TEST` if you wish to skip the long-running tests
 * `_USE_LONG_TEST` if you wish to execute those tests

The simplest thing in Qt Creator is to add an additional argument to the qmake step like `"DEFINES+=_USE_LONG_TEST"`.

The project is now localized and you should build the translation to be included into the resource file of the frontend, which boils down to running `lrelease`. I suggest adding steps
  * `/path/to/lupdate-pro.exe -noobsolete -locations none /path/to/QtPollyNomSuper.pro`
  * `/path/to/lrelease-pro.exe /path/to/QtPollyNomSuper.pro`

to the build process.

## License

All source code licensed under GPL v3 (see LICENSE for terms).

## Attributions

Graphical user interface built using [Qt](https://doc.qt.io/).

Icon (axes and graph) attributed to: Icons made by [Pixel perfect](https://www.flaticon.com/authors/pixel-perfect) from [Flaticon](https://www.flaticon.com/)

[QCustomPlot](https://www.qcustomplot.com/) library (Version 2.1.0) by Emanuel Eichhammer used under the [GPL v3](https://www.gnu.org/licenses/gpl-3.0.html).

JSON de/serialization via [rapidjson](https://rapidjson.org/) provided by THL A29 Limited, a Tencent company, and Milo Yip used under the [MIT license](http://opensource.org/licenses/MIT).

GoogleTest used under the following conditions:

```
Copyright 2008, Google Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    * Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the
distribution.
    * Neither the name of Google Inc. nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```
