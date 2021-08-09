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

#include <QtConcurrent>
#include <QStandardPaths>

#include "mainwindow.h"

#include <vector>
#include <sstream>
#include <iterator>
#include <functional>

MainWindow::MainWindow(std::shared_ptr<Backend::DotGenerator> dotGenerator,
                       std::shared_ptr<Backend::Repository> repository,
                       QWidget *parent)
    : MainWindow(parent)
{
    this->game = Backend::Game(dotGenerator, repository);
}

MainWindow::MainWindow(std::shared_ptr<Backend::DotGenerator> dotGenerator,
                       QWidget *parent)
    : MainWindow(parent)
{
    this->game = Backend::Game(dotGenerator);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , game()
    , focusIndicator(-2)
{
    ui->setupUi(this);
    this->UpdateWindowTitle();

    this->numberOfFunctionInputs = ui->funcLineEdit.size();

    this->waitTimer.setSingleShot(true);
    this->waitTimer.setInterval(500);

    this->SetupColors();

    this->InitializePlot();

    connect(ui->calcButton, &QPushButton::clicked, this, &MainWindow::OnCalcButtonClicked);

    for(size_t i = 0; i < this->numberOfFunctionInputs; ++i)
    {
        ui->funcLineEdit[i]->setPalette(this->parseablePalette);
        connect(ui->funcLineEdit[i], &QLineEdit::returnPressed, this, &MainWindow::OnReturnKeyPressed);
        connect(ui->funcLineEdit[i], &QLineEdit::textChanged, this, &MainWindow::OnFuncLineEditTextChanged);
    }

    connect(&gameUpdateFutureWatcher, &QFutureWatcher<void>::finished, this, &MainWindow::OnGameUpdateFinished);
    connect(&waitTimer, &QTimer::timeout, this, &MainWindow::OnWaitTimerFinished);
    connect(ui->newGameMenuAction, &QAction::triggered, this, &MainWindow::OnNewGameMenuTriggered);
    connect(ui->openGameMenuAction, &QAction::triggered, this, &MainWindow::OnOpenGameMenuTriggered);
    connect(ui->saveGameMenuAction, &QAction::triggered, this, &MainWindow::OnSaveGameMenuTriggered);
    connect(ui->aboutMenuAction, &QAction::triggered, this, &MainWindow::OnAboutMenuTriggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnNewGameMenuTriggered()
{
    // create new game
    this->game.Remake();

    // clear UI
    for(size_t i = 0; i < this->numberOfFunctionInputs; ++i)
    {
        ui->funcLineEdit[i]->clear();
    }

    this->InitializePlot();
    this->UpdateWindowTitle();
}

void MainWindow::OnOpenGameMenuTriggered()
{
    Resetter resetter([&](){ this->presetFilename.clear(); });

    auto folder = this->GetFolderForFileDialog();
    auto filter = this->GetGameFileFilter();
    QString fileName = !this->presetFilename.isEmpty() ? this->presetFilename : QFileDialog::getOpenFileName(this, "", folder, filter, nullptr, QFileDialog::Options());

    if(fileName.isEmpty())
    {
        return;
    }

    auto result = this->game.Load(fileName.toStdWString());

    if(result.first)
    {
        this->SetGameIsBusy(true);
        this->SetFunctionsInputFromGame();

        QFuture<void> updateFuture = QtConcurrent::run([=](){
            this->game.CreateGraphs();
        });
        this->gameUpdateFutureWatcher.setFuture(updateFuture);

        this->waitTimer.start();
        return;
    }

    auto messageBoxTitle = QCoreApplication::translate("MainWindow", "Error", nullptr);

    //: Arg 1 is the internal error message, not translated.
    auto messageBoxTextTemplate = QCoreApplication::translate("MainWindow", "Error occurred when trying to open a game: %1", nullptr);
    auto errorMesssage = QString::fromStdWString(result.second);
    auto messageBoxText = messageBoxTextTemplate.arg(errorMesssage);

    auto errorBox = std::make_unique<QMessageBox>(
                    QMessageBox::Icon::Critical,
                    messageBoxTitle,
                    messageBoxText);

    errorBox->exec();

    errorBox.reset();
}

void MainWindow::OnSaveGameMenuTriggered()
{
    Resetter resetter([&](){ this->presetFilename.clear(); });

    this->StartCalculation();

    auto folder = this->GetFolderForFileDialog();
    auto filter = this->GetGameFileFilter();
    QString fileName = !this->presetFilename.isEmpty() ? this->presetFilename : QFileDialog::getSaveFileName(this, "", folder, filter, nullptr, QFileDialog::Options());

    if(fileName.isEmpty())
    {
        return;
    }

    auto result = this->game.Save(fileName.toStdWString());

    if(result.first)
    {
        return;
    }

    auto messageBoxTitle = QCoreApplication::translate("MainWindow", "Error", nullptr);

    //: Arg 1 is the internal error message, not translated.
    auto messageBoxTextTemplate = QCoreApplication::translate("MainWindow", "Error occurred when trying to save a game: %1", nullptr);
    auto errorMesssage = QString::fromStdWString(result.second);
    auto messageBoxText = messageBoxTextTemplate.arg(errorMesssage);

    auto errorBox = std::make_unique<QMessageBox>(
                    QMessageBox::Icon::Critical,
                    messageBoxTitle,
                    messageBoxText);

    errorBox->exec();

    errorBox.reset();
}

void MainWindow::OnAboutMenuTriggered()
{
    auto messageBoxTitleTemplate = QCoreApplication::translate("MainWindow", "About %1", nullptr);
    auto messageBoxTitle = messageBoxTitleTemplate.arg(QCoreApplication::translate("MainWindow", "QtPollyNom", nullptr));

    //: Arg 1 is a placeholder for the program name. Format is QT Rich Text.
    auto messageBoxTextTemplate = QCoreApplication::translate("MainWindow", "A math teaching tool pretending to be a game.<br /><br />%1 Copyright (C) 2021 and later, tristhaus<br />This program comes with ABSOLUTELY NO WARRANTY.<br />This is free software, and you are welcome to redistribute it under certain conditions. See provided LICENSE file for details.<br /><br />Graphical user interface built using <a href=\"https://doc.qt.io/\">Qt</a>.<br />Icon (axes and graph) attributed to: Icons made by <a href=\"https://www.flaticon.com/authors/pixel-perfect\">Pixel perfect</a> from <a href=\"https://www.flaticon.com/\">Flaticon</a>.<br /><a href=\"https://www.qcustomplot.com/\">QCustomPlot</a> library (Version 2.1.0) by Emanuel Eichhammer used under the <a href=\"https://www.gnu.org/licenses/gpl-3.0.html\">GPL v3</a>. JSON de/serialization via <a href=\"https://rapidjson.org/\">rapidjson</a> provided by THL A29 Limited, a Tencent company, and Milo Yip used under the <a href=\"http://opensource.org/licenses/MIT\">MIT license</a>.", nullptr);
    auto messageBoxText = messageBoxTextTemplate.arg(QCoreApplication::translate("MainWindow", "QtPollyNom", nullptr));

    this->aboutMessageBox = std::make_unique<QMessageBox>(
                    QMessageBox::Icon::NoIcon,
                    messageBoxTitle,
                    messageBoxText);

    this->aboutMessageBox->setTextFormat(Qt::RichText);
    this->aboutMessageBox->setTextInteractionFlags(Qt::TextBrowserInteraction);

    this->aboutMessageBox->exec();

    this->aboutMessageBox.reset();
}

void MainWindow::InitializePlot()
{
    ui->plot->xAxis->setLabel("x");
    ui->plot->yAxis->setLabel("y");
    ui->plot->xAxis->setRange(-10, 10);
    ui->plot->yAxis->setRange(-10, 10);

    ui->plot->clearPlottables();

    this->DrawDots();

    ui->plot->replot();
}

void MainWindow::SetupColors()
{
    this->graphColors.push_back(QColor(0x00, 0x00, 0x00)); // Black
    this->graphColors.push_back(QColor(0x00, 0x00, 0xFF)); // Blue
    this->graphColors.push_back(QColor(0x94, 0x00, 0xD3)); // DarkViolet
    this->graphColors.push_back(QColor(0xA5, 0x2A, 0x2A)); // Brown
    this->graphColors.push_back(QColor(0xB0, 0xE0, 0xE6)); // PowderBlue

    QColor lightPink = QColor(0xFF, 0xB6, 0xC1);

    this->activeGoodDotColor = QColor(0xAD, 0xD8, 0xE6);   // LightBlue
    this->inactiveGoodDotColor = QColor(0x00, 0x00, 0x8B); // DarkBlue
    this->activeBadDotColor = QColor(0xFF, 0x45, 0x00);    // OrangeRed
    this->inactiveBadDotColor = lightPink;

    this->parseablePalette.setColor(QPalette::Base, Qt::white);
    this->parseablePalette.setColor(QPalette::Text, Qt::black);
    this->nonParseablePalette.setColor(QPalette::Base, lightPink);
    this->nonParseablePalette.setColor(QPalette::Text, Qt::black);
}

void MainWindow::DrawDots()
{
    // clear existing curves for dots
    auto dotCurvesIterator = this->dotCurves.begin();
    auto dotCurvesEnd = this->dotCurves.end();

    while(dotCurvesIterator != dotCurvesEnd){
        ui->plot->removePlottable(*dotCurvesIterator);
        ++dotCurvesIterator;
    }

    this->dotCurves.clear();

    // get dots and get ready to iterate
    auto dots = this->game.GetDots();

    auto dotsIterator = dots.begin();
    auto dotsEnd = dots.end();

    if(dotsIterator == dotsEnd)
    {
        return;
    }

    // create data for generic circular curve
    const int pointCount = 500;
    QVector<double> dataX(pointCount);
    QVector<double> dataY(pointCount);

    for (int i=0; i<pointCount; ++i)
    {
        double theta = i/(double)(pointCount-1)*2*M_PI;
        dataX[i] = qCos(theta);
        dataY[i] = qSin(theta);
    }

    std::function<QColor(bool, bool)> determineDotColor = [&](bool isActive, bool isGood)
    {
        if(isActive)
        {
            return isGood ? this->activeGoodDotColor : this->activeBadDotColor;

        }
        else
        {
            return isGood ? this->inactiveGoodDotColor : this->inactiveBadDotColor;
        }
    };

    while(dotsIterator != dotsEnd)
    {
        // apply data to one dot
        QCPCurve * curve = new QCPCurve(ui->plot->xAxis, ui->plot->yAxis);
        this->dotCurves.push_back(curve);

        auto coord = (*dotsIterator)->GetCoordinates();
        auto radius = (*dotsIterator)->GetRadius();

        QVector<QCPCurveData> dataCurve(pointCount);

        // shift to dot center
        for (int i=0; i<pointCount; ++i)
        {
            dataCurve[i] = QCPCurveData(i, radius * dataX[i] + coord.first, radius * dataY[i] + coord.second);
        }

        // apply color according to dot characteristics
        curve->data()->set(dataCurve, true);
        auto dotColor = determineDotColor((*dotsIterator)->IsActive(), (*dotsIterator)->IsGood());
        curve->setBrush(QBrush(dotColor));
        curve->setPen(QPen(dotColor));

        ++dotsIterator;
    }
}

void MainWindow::DrawGraphs()
{
    auto& graphs = this->game.GetGraphs();

    ui->plot->clearGraphs();

    for(unsigned long long graphIndex = 0; graphIndex < graphs.size(); ++graphIndex)
    {
        auto& graph = graphs[graphIndex];
        auto pen = QPen(graphColors[graphIndex]);

        for(unsigned long long branchIndex = 0; branchIndex < graph.size(); ++branchIndex)
        {
            auto& branch = graph[branchIndex];

            if(branch.first.empty())
            {
                continue;
            }

            QVector<double> dataX(branch.first.begin(), branch.first.end());
            QVector<double> dataY(branch.second.begin(), branch.second.end());

            auto* qcpGraph = ui->plot->addGraph();

            qcpGraph->addData(dataX, dataY, true);
            qcpGraph->setPen(pen);
        }
    }
}

void MainWindow::UpdateWindowTitle()
{
    //: Arg 1 is the window title, Arg 2 is the numerical score
    auto windowTitleTemplate = QCoreApplication::translate("MainWindow", "%1 - Score: %2", nullptr).arg(QCoreApplication::translate("MainWindow", "QtPollyNom", nullptr));

    auto score = this->game.GetScore();

    QString newTitle;

    if(score < 0)
    {
        newTitle = windowTitleTemplate.arg(QString("-∞"));
    }
    else
    {
        newTitle = windowTitleTemplate.arg(score);
    }

    this->setWindowTitle(newTitle);
}

void MainWindow::SetGameIsBusy(bool isBusy)
{
    if(isBusy)
    {
        if(this->ui->calcButton->hasFocus())
        {
            this->focusIndicator = -1;
        }

        for(size_t i = 0; i<this->numberOfFunctionInputs; ++i)
        {
            if(isBusy && this->ui->funcLineEdit[i]->hasFocus())
            {
                this->focusIndicator = static_cast<int>(i);
                break;
            }
        }
    }

    this->ui->newGameMenuAction->setDisabled(isBusy);
    this->ui->openGameMenuAction->setDisabled(isBusy);
    this->ui->saveGameMenuAction->setDisabled(isBusy);
    this->ui->aboutMenuAction->setDisabled(isBusy);
    this->ui->calcButton->setDisabled(isBusy);
    for(size_t i = 0; i<this->numberOfFunctionInputs; ++i)
    {
        this->ui->funcLineEdit[i]->setDisabled(isBusy);
    }

    if(!isBusy)
    {
        if(this->focusIndicator >= 0)
        {
            this->ui->funcLineEdit[this->focusIndicator]->setFocus(Qt::FocusReason::PopupFocusReason);
        }
        else if(this->focusIndicator == -1)
        {
            this->ui->calcButton->setFocus(Qt::FocusReason::PopupFocusReason);
        }
    }
}

void MainWindow::UpdateGui()
{
    this->SetGameIsBusy(false);

    this->DrawDots();
    this->DrawGraphs();
    this->UpdateWindowTitle();

    ui->plot->replot();
}

void MainWindow::SetFunctionsInputFromGame()
{
    std::vector<std::wstring> funcStrings = this->game.GetFunctions();

    for(size_t i = 0; i<this->numberOfFunctionInputs && i<funcStrings.size(); ++i)
    {
        this->ui->funcLineEdit[i]->setText(QString::fromStdWString(funcStrings[i]));
    }
}

void MainWindow::StartCalculation()
{
    this->SetGameIsBusy(true);

    std::vector<std::wstring> funcStrings;

    for(size_t i = 0; i<this->numberOfFunctionInputs; ++i)
    {
        funcStrings.emplace_back(this->ui->funcLineEdit[i]->text().toStdWString());
    }

    QFuture<void> updateFuture = QtConcurrent::run([=](){
        this->game.Update(funcStrings);
    });
    this->gameUpdateFutureWatcher.setFuture(updateFuture);

    this->waitTimer.start();
}

QString MainWindow::GetGameFileFilter()
{
    return QCoreApplication::translate("MainWindow", "Games (*.qpn)", nullptr);
}

QString MainWindow::GetFolderForFileDialog()
{
    auto list = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    return QString(list.first());
}

void MainWindow::OnCalcButtonClicked()
{
    this->StartCalculation();
}

void MainWindow::OnReturnKeyPressed()
{
    this->StartCalculation();
}

void MainWindow::OnFuncLineEditTextChanged()
{
    auto funcLineEdit = dynamic_cast<QLineEdit*>(QObject::sender());
    auto funcString = std::wstring(funcLineEdit->text().toStdWString());

    bool isParseable = funcString.empty() || this->game.IsParseable(funcString);

#ifdef _DEBUG
    if(funcString == L"slow")
    {
        isParseable = true;
    }
#endif

    QPalette & palette = isParseable ? this->parseablePalette : this->nonParseablePalette;

    funcLineEdit->setPalette(palette);
}

void MainWindow::OnGameUpdateFinished()
{
    this->waitTimer.stop();
    if(this->waitingMessageBox)
    {
        this->waitingMessageBox->close();
        this->waitingMessageBox.reset();
    }

    this->UpdateGui();
}

void MainWindow::OnWaitTimerFinished()
{
    if(this->gameUpdateFutureWatcher.isFinished() || this->gameUpdateFutureWatcher.isCanceled())
    {
        return;
    }

    this->waitingMessageBox = std::make_unique<QMessageBox>(
                QMessageBox::Icon::NoIcon,
                QCoreApplication::translate("MainWindow", "Plotting", nullptr),
                QCoreApplication::translate("MainWindow", "Waiting for plot to complete ...", nullptr));

    auto * button = new QPushButton(QCoreApplication::translate("MainWindow", "Stop waiting", nullptr));
    this->waitingMessageBox->addButton(button, QMessageBox::ButtonRole::NoRole);

    connect(&(*(this->waitingMessageBox)), &QMessageBox::buttonClicked, this, &MainWindow::OnWaitingMessageBoxButtonClicked);

    this->waitingMessageBox->exec();

    this->waitingMessageBox.reset();
}

void MainWindow::OnWaitingMessageBoxButtonClicked()
{
    this->gameUpdateFutureWatcher.cancel();
    this->game.Clear();
    this->UpdateGui();
}
