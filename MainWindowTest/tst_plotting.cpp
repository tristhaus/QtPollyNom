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
    void EnteredFunctionsShallCreateCorrectNumberOfPlots_data();
    void EnteredFunctionsShallCreateCorrectNumberOfPlots();
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

void MainWindowTest::EnteredFunctionsShallCreateCorrectNumberOfPlots_data()
{
    QTest::addColumn<QString>("function1");
    QTest::addColumn<QString>("function2");
    QTest::addColumn<int>("expectedNumberOfGraphs");

    QTest::newRow("std sin")           << "sin(x)"    << ""        << 1;
    QTest::newRow("1/x")               << "1/x"       << ""        << 2;
    QTest::newRow("1/x & x")           << "1/x"       << "x"       << 3;
    QTest::newRow("2 std sin")         << "sin(x)"    << "sin(x)"  << 2;
    QTest::newRow("std tan")           << "tan(x)"    << ""        << 7;
    QTest::newRow("sqrt")              << "(x)^(0.5)" << ""        << 1;
    QTest::newRow("x to the xth")      << "x^x"       << ""        << 1;
    QTest::newRow("invalid")           << "csic(x)"   << ""        << 0;
    QTest::newRow("sin & invalid")     << "sin(x)"    << "csic(x)" << 1;
    QTest::newRow("log out of domain") << "ln(x-20)"  << ""        << 0;
}

void MainWindowTest::EnteredFunctionsShallCreateCorrectNumberOfPlots()
{
    // Arrange
    MainWindow mw;
    auto ui = mw.ui;

    QFETCH(QString, function1);
    QFETCH(QString, function2);
    QFETCH(int, expectedNumberOfGraphs);

    // spy needed such that events actually happen
    QSignalSpy spy(&(mw.gameUpdateFutureWatcher), &QFutureWatcher<void>::finished);

    QVERIFY2(ui->plot->graphCount() == 0, "graphs found that should not be there");

    // Act
    for(size_t i = 0; i < ui->funcLineEdit.size(); ++i)
    {
        ui->funcLineEdit[i]->clear();
    }

    QTest::keyClicks(ui->funcLineEdit[0], function1);
    QTest::keyClicks(ui->funcLineEdit[1], function2);
    QTest::mouseClick(ui->calcButton, Qt::LeftButton);

    spy.wait();

    // Assert
    int actualNumberOfGraphs = ui->plot->graphCount();

    QVERIFY2(spy.count() == 1, "spy did not register signal");
    QVERIFY2(expectedNumberOfGraphs == actualNumberOfGraphs,
             qPrintable(QString("function1 '%1', function2 '%2', expectedNumberOfGraphs %3, actualNumberOfGraphs %4")
                        .arg(function1, function2)
                        .arg(expectedNumberOfGraphs)
                        .arg(actualNumberOfGraphs)));
}

QTEST_MAIN(MainWindowTest)

#include "tst_plotting.moc"
