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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFuture>
#include <vector>
#include "qcustomplot.h"
#include "../Backend/gamepoc.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_calcButton_clicked();
    void on_update_finished();

private:
    void InitializePlot();
    void SetupColors();
    void DrawDots();
    void DrawGraphs();
    void SetGameIsBusy(bool isBusy);

private:
    Ui::MainWindow *ui;

    GamePoc gamePoc;
    QFutureWatcher<void> gameUpdateFutureWatcher;

    std::vector<QCPAbstractPlottable*> dotCurves;

    std::vector<QColor> graphColors;
    QColor activeGoodDotColor;
    QColor inactiveGoodDotColor;
    QColor activeBadDotColor;
    QColor inactiveBadDotColor;
};
#endif // MAINWINDOW_H
