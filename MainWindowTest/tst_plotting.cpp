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

#include <QtTest>

#include "../Frontend/mainwindow.h"

class MainWindowTest : public QObject
{
    Q_OBJECT

public:
    MainWindowTest();
    ~MainWindowTest();

private slots:
    void ConstructionShallWorkCompletely();
    void CalcButtonShallTriggerPlotting();
    void EnterKeyShallTriggerPlotting();
};

MainWindowTest::MainWindowTest()
{
}

MainWindowTest::~MainWindowTest()
{
}

void MainWindowTest::ConstructionShallWorkCompletely()
{
    // Arrange, Act
    MainWindow mw;
    auto ui = mw.ui;

    // Assert
    QVERIFY2(ui->menubar, "menu bar not created");
    QVERIFY2(ui->statusbar, "status bar not created");
    QVERIFY2(ui->centralwidget, "central widget not created");
    QVERIFY2(ui->verticalLayout, "vert layout not created");
    QVERIFY2(ui->verticalLayout_2, "vert layout 2 not created");
    QVERIFY2(ui->verticalLayout_3, "vert layout 3 not created");
    QVERIFY2(ui->plot, "plot not created");
    QVERIFY2(ui->inputFrame0, "input frame0 not created");
    QVERIFY2(ui->inputFrame1, "input frame1 not created");
    QVERIFY2(ui->calcButton, "calc button not created");

    QVERIFY2(ui->funcLayout.size() == ui->funcLabel.size() && ui->funcLayout.size() == ui->funcLineEdit.size(), "sizes do not match");

    for(size_t i = 0; i < ui->funcLabel.size(); ++i)
    {
        QVERIFY2(ui->funcLabel[i], "func label not created");
        QVERIFY2(ui->funcLayout[i], "func layout not created");
        QVERIFY2(ui->funcLineEdit[i], "func line edit not created");
    }
}

void MainWindowTest::CalcButtonShallTriggerPlotting()
{
    // Arrange
    MainWindow mw;
    auto ui = mw.ui;

    QString function = "sin(x)";

    // spy needed such that events actually happen
    QSignalSpy spy(&(mw.gameUpdateFutureWatcher), &QFutureWatcher<void>::finished);

    QVERIFY2(ui->plot->graphCount() == 0, "graphs found that should not be there");

    // Act
    ui->funcLineEdit[2]->clear();
    QTest::keyClicks(ui->funcLineEdit[2], function);
    QTest::mouseClick(ui->calcButton, Qt::LeftButton);

    spy.wait();

    // Assert
    QVERIFY2(spy.count() == 1, "spy did not register signal");
    QVERIFY2(ui->plot->graphCount() > 0, "no graph found");
}

void MainWindowTest::EnterKeyShallTriggerPlotting()
{
    // Arrange
    MainWindow mw;
    auto ui = mw.ui;

    QString function = "sin(x)\r";

    // spy needed such that events actually happen
    QSignalSpy spy(&(mw.gameUpdateFutureWatcher), &QFutureWatcher<void>::finished);

    QVERIFY2(ui->plot->graphCount() == 0, "graphs found that should not be there");

    // Act
    ui->funcLineEdit[2]->clear();
    QTest::keyClicks(ui->funcLineEdit[2], function);

    spy.wait();

    // Assert
    QVERIFY2(spy.count() == 1, "spy did not register signal");
    QVERIFY2(ui->plot->graphCount() > 0, "no graph found");
}

QTEST_MAIN(MainWindowTest)

#include "tst_plotting.moc"
