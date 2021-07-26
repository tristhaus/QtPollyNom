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

#if defined(_SKIP_LONG_TEST)
#elif defined(_USE_LONG_TEST)
#else
#error "you need to make a choice between using or skipping long tests, -D_USE_LONG_TEST -D_SKIP_LONG_TEST"
#endif

#include <QtTest>
#include <QtWidgets/QMenuBar>

#include "../Frontend/mainwindow.h"
#include "../TestHelper/fixeddotgenerator.h"

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
#ifdef _DEBUG
#if defined(_USE_LONG_TEST)
    void SlowCalculationShallTriggerDialogAndWaitingForCompletionShallYieldGraphs();
    void SlowCalculationShallTriggerDialogAndCancelShallYieldNoGraph();
#endif
#endif
    void AfterCalculationUIShallRefocusCorrectlyToLineEdit();
    void AfterCalculationUIShallRefocusCorrectlyToButton();
    void AfterHittingGoodDotsUIShallCorrectlySetTitle();
    void AfterHittingBadDotUIShallCorrectlySetTitle();
    void NewGameButtonShallCreateNewGame();
#if defined(_USE_LONG_TEST)
    void AboutButtonShallTriggerDialogAndOKShallClose();
#endif
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

#ifdef _DEBUG
#if defined(_USE_LONG_TEST)

void MainWindowTest::SlowCalculationShallTriggerDialogAndWaitingForCompletionShallYieldGraphs()
{
    // Arrange
    MainWindow mw;
    auto ui = mw.ui;

    QString function = "sin(x)";
    QString slow = "slow";

    // spy needed such that events actually happen
    QSignalSpy spyUpdate(&(mw.gameUpdateFutureWatcher), &QFutureWatcher<void>::finished);
    QSignalSpy spyTimer(&(mw.waitTimer), &QTimer::timeout);

    QVERIFY2(ui->plot->graphCount() == 0, "graphs found that should not be there");

    // Act
    ui->funcLineEdit[0]->clear();
    ui->funcLineEdit[1]->clear();
    QTest::keyClicks(ui->funcLineEdit[0], function);
    QTest::keyClicks(ui->funcLineEdit[1], slow);
    QTest::mouseClick(ui->calcButton, Qt::LeftButton);

    bool waitingMessageBoxFound = false;
    bool waitingMessageBoxHasOneButton = false;
    auto interval = mw.waitTimer.interval();
    QTimer::singleShot(interval * 1.2, [&]()
    {
        waitingMessageBoxFound = mw.waitingMessageBox != nullptr;
        waitingMessageBoxHasOneButton = mw.waitingMessageBox->buttons().count() == 1 && mw.waitingMessageBox->buttons().first() != nullptr;
    });

    spyTimer.wait();
    spyUpdate.wait();

    // Assert
    QVERIFY2(spyUpdate.count() == 1, "spyUpdate did not register signal");
    QVERIFY2(spyTimer.count() == 1, "spyTimer did not register signal");
    QVERIFY2(waitingMessageBoxFound, "waitingMessageBox not found");
    QVERIFY2(waitingMessageBoxHasOneButton, "waitingMessageBox does not have exactly one button");
    QVERIFY2(ui->plot->graphCount() > 0, "no graph found");

    QVERIFY2(mw.waitingMessageBox == nullptr, "waitingMessageBox still reachable");
}

void MainWindowTest::SlowCalculationShallTriggerDialogAndCancelShallYieldNoGraph()
{
    // Arrange
    MainWindow mw;
    auto ui = mw.ui;

    QString function = "sin(x)";
    QString slow = "slow";

    // spy needed such that events actually happen
    QSignalSpy spyUpdate(&(mw.gameUpdateFutureWatcher), &QFutureWatcher<void>::finished);
    QSignalSpy spyTimer(&(mw.waitTimer), &QTimer::timeout);

    QVERIFY2(ui->plot->graphCount() == 0, "graphs found that should not be there");

    // Act
    ui->funcLineEdit[0]->clear();
    ui->funcLineEdit[1]->clear();
    QTest::keyClicks(ui->funcLineEdit[0], function);
    QTest::keyClicks(ui->funcLineEdit[1], slow);
    QTest::mouseClick(ui->calcButton, Qt::LeftButton);

    bool waitingMessageBoxFound = false;
    bool waitingMessageBoxHasOneButton = false;
    auto interval = mw.waitTimer.interval();
    QTimer::singleShot(interval * 1.2, [&]()
    {
        waitingMessageBoxFound = mw.waitingMessageBox != nullptr;
        waitingMessageBoxHasOneButton = mw.waitingMessageBox->buttons().count() == 1 && mw.waitingMessageBox->buttons().first() != nullptr;
        if(waitingMessageBoxFound && waitingMessageBoxHasOneButton)
        {
            QTest::mouseClick(mw.waitingMessageBox->buttons().first(), Qt::LeftButton);
        }
    });

    spyTimer.wait();
    spyUpdate.wait();

    // Assert
    QVERIFY2(spyUpdate.count() == 1, "spyUpdate did not register signal");
    QVERIFY2(spyTimer.count() == 1, "spyTimer did not register signal");
    QVERIFY2(waitingMessageBoxFound, "waitingMessageBox not found");
    QVERIFY2(waitingMessageBoxHasOneButton, "waitingMessageBox does not have exactly one button");
    QVERIFY2(ui->plot->graphCount() == 0, "graph found");

    QVERIFY2(mw.waitingMessageBox == nullptr, "waitingMessageBox still reachable");
}

#endif
#endif

void MainWindowTest::AfterCalculationUIShallRefocusCorrectlyToLineEdit()
{
    // Arrange
    MainWindow mw;
    auto ui = mw.ui;

    QString function = "sin(x)\r";
    const size_t index = 2;

    // spy needed such that events actually happen
    QSignalSpy spyUpdate(&(mw.gameUpdateFutureWatcher), &QFutureWatcher<void>::finished);

    // without this, focus testing is not possible
    QApplication::setActiveWindow(&mw);

    // Act
    for(size_t i = 0; i < ui->funcLineEdit.size(); ++i)
    {
        ui->funcLineEdit[i]->clear();
    }

    QTest::mouseClick(ui->funcLineEdit[index], Qt::LeftButton);
    bool hasFocusBeforeCalculation = ui->funcLineEdit[2]->hasFocus();
    QTest::keyClicks(ui->funcLineEdit[index], function);

    spyUpdate.wait();

    // Assert
    QVERIFY2(spyUpdate.count() == 1, "spyUpdate did not register signal");
    QVERIFY2(hasFocusBeforeCalculation, "relevant line edit does not have focus before calcuation");
    QVERIFY2(ui->funcLineEdit[index]->hasFocus(), "relevant line edit does not have focus after calcuation");
}

void MainWindowTest::AfterCalculationUIShallRefocusCorrectlyToButton()
{
    // Arrange
    MainWindow mw;
    auto ui = mw.ui;

    QString function = "sin(x)";

    // spy needed such that events actually happen
    QSignalSpy spyUpdate(&(mw.gameUpdateFutureWatcher), &QFutureWatcher<void>::finished);

    // without this, focus testing is not possible
    QApplication::setActiveWindow(&mw);

    // Act
    for(size_t i = 0; i < ui->funcLineEdit.size(); ++i)
    {
        ui->funcLineEdit[i]->clear();
    }

    QTest::keyClicks(ui->funcLineEdit[3], function);
    QTest::mouseClick(ui->calcButton, Qt::LeftButton);

    spyUpdate.wait();

    // Assert
    QVERIFY2(spyUpdate.count() == 1, "spyUpdate did not register signal");
    QVERIFY2(ui->calcButton->hasFocus(), "button does not have focus after calcuation");
}

void MainWindowTest::AfterHittingGoodDotsUIShallCorrectlySetTitle()
{
    // Arrange
    MainWindow mw(std::make_shared<FixedDotGenerator>());
    auto ui = mw.ui;

    QString function1 = "1/x";
    QString function2 = "(x-3.0)*(x+4.0)";

    // spy needed such that events actually happen
    QSignalSpy spyUpdate(&(mw.gameUpdateFutureWatcher), &QFutureWatcher<void>::finished);

    // Act
    for(size_t i = 0; i < ui->funcLineEdit.size(); ++i)
    {
        ui->funcLineEdit[i]->clear();
    }

    QTest::keyClicks(ui->funcLineEdit[0], function1);
    QTest::keyClicks(ui->funcLineEdit[1], function2);
    QTest::mouseClick(ui->calcButton, Qt::LeftButton);

    spyUpdate.wait();

    // Assert
    QRegularExpression verification(" 4$");
    QVERIFY2(mw.windowTitle().contains(verification), "correct score was not part of the window title");
}

void MainWindowTest::AfterHittingBadDotUIShallCorrectlySetTitle()
{
    // Arrange
    MainWindow mw(std::make_shared<FixedDotGenerator>());
    auto ui = mw.ui;

    QString function1 = "1/x";
    QString function2 = "5.05";

    // spy needed such that events actually happen
    QSignalSpy spyUpdate(&(mw.gameUpdateFutureWatcher), &QFutureWatcher<void>::finished);

    // Act
    for(size_t i = 0; i < ui->funcLineEdit.size(); ++i)
    {
        ui->funcLineEdit[i]->clear();
    }

    QTest::keyClicks(ui->funcLineEdit[0], function1);
    QTest::keyClicks(ui->funcLineEdit[1], function2);
    QTest::mouseClick(ui->calcButton, Qt::LeftButton);

    spyUpdate.wait();

    // Assert
    QRegularExpression verification("-∞");
    QVERIFY2(mw.windowTitle().contains(verification), "correct score was not part of the window title");
}

void MainWindowTest::NewGameButtonShallCreateNewGame()
{
    // Arrange
    MainWindow mw(std::make_shared<FixedDotGenerator>(2));
    auto ui = mw.ui;

    QString function1 = "1/x";
    QString function2 = "(x-3.0)*(x+4.0)";

    // spy needed such that events actually happen
    QSignalSpy spyUpdate(&(mw.gameUpdateFutureWatcher), &QFutureWatcher<void>::finished);
    QSignalSpy spyNewGameAction(ui->newGameMenuAction, &QAction::triggered);

    // Act
    for(size_t i = 0; i < ui->funcLineEdit.size(); ++i)
    {
        ui->funcLineEdit[i]->clear();
    }

    auto dots1 = mw.game.GetDots();

    QTest::keyClicks(ui->funcLineEdit[0], function1);
    QTest::keyClicks(ui->funcLineEdit[1], function2);
    QTest::mouseClick(ui->calcButton, Qt::LeftButton);

    spyUpdate.wait();

    QMenuBar *menuBar = ui->menubar;
    QVERIFY2(menuBar != nullptr, "menuBar not found");
    if (menuBar != nullptr)
    {
        QList<QAction *> actions = menuBar->actions();
        for(qsizetype i = 0; i < actions.size(); ++i)
        {
            auto & action = actions[i];
            if (action->objectName() == QString::fromUtf8("newGame"))
            {
                action->trigger();
                break;
            }
        }
    }

    auto dots2 = mw.game.GetDots();

    // Assert
    const double tolerance = 1e-9;
    for(unsigned int iter1 = 0; iter1 < dots1.size(); ++iter1)
    {
        auto & dot1 = dots1[iter1];
        for(unsigned int iter2 = 0; iter2 < dots2.size(); ++iter2)
        {
            auto & dot2 = dots2[iter2];

            auto evalFirst = abs(dot1->GetCoordinates().first - dot2->GetCoordinates().first) < tolerance;
            auto evalSecond = abs(dot1->GetCoordinates().second - dot2->GetCoordinates().second) < tolerance;
            auto evalRadius = abs(dot1->GetRadius() - dot2->GetRadius()) < tolerance;
            auto evalKind = dot1->IsGood() == dot2->IsGood();
            QVERIFY2(!(evalFirst && evalSecond && evalRadius && evalKind), "dots match");
        }
    }

    QRegularExpression verification(" 0$");
    QVERIFY2(mw.windowTitle().contains(verification), "correct score was not part of the window title");

    for(std::vector<QLineEdit>::size_type j = 0; j < ui->funcLineEdit.size(); ++j)
    {
        QVERIFY2(ui->funcLineEdit[j]->text().isEmpty(), "line edit not empty");
    }

    QVERIFY2(ui->plot->graphCount() == 0, "graphs found");
}

#if defined(_USE_LONG_TEST)
void MainWindowTest::AboutButtonShallTriggerDialogAndOKShallClose()
{
    // Arrange
    MainWindow mw;
    auto ui = mw.ui;

    // spy needed such that events actually happen
    QSignalSpy spyAboutAction(ui->aboutMenuAction, &QAction::triggered);

    QVERIFY2(ui->plot->graphCount() == 0, "graphs found that should not be there");

    // Act
    bool aboutMessageBoxFound = false;
    bool aboutMessageBoxHasOneButton = false;
    int interval = 1000;
    QTimer::singleShot(interval, [&]()
    {
        aboutMessageBoxFound = mw.aboutMessageBox != nullptr;
        aboutMessageBoxHasOneButton = mw.aboutMessageBox->buttons().count() == 1 && mw.aboutMessageBox->buttons().first() != nullptr;
        if(aboutMessageBoxFound && aboutMessageBoxHasOneButton)
        {
            QTest::mouseClick(mw.aboutMessageBox->buttons().first(), Qt::LeftButton);
        }
    });

    QMenuBar *menuBar = ui->menubar;
    QVERIFY2(menuBar != nullptr, "menuBar not found");
    if (menuBar != nullptr)
    {
        QList<QAction *> actions = menuBar->actions();
        for(qsizetype i = 0; i < actions.size(); ++i)
        {
            auto & action = actions[i];
            if (action->objectName() == QString::fromUtf8("about"))
            {
                action->trigger();
                break;
            }
        }
    }

    spyAboutAction.wait();

    // Assert
    QVERIFY2(aboutMessageBoxFound, "aboutMessageBox not found");
    QVERIFY2(aboutMessageBoxHasOneButton, "aboutMessageBox does not have exactly one button");

    QVERIFY2(mw.aboutMessageBox == nullptr, "aboutMessageBox still reachable");
}
#endif

QTEST_MAIN(MainWindowTest)

#include "tst_plotting.moc"
