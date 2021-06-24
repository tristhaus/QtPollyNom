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

#include "mainwindow.h"

#include <vector>
#include <iterator>
#include <functional>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , focusIndicator(-2)
{
    ui->setupUi(this);
    this->numberOfFunctionInputs = ui->funcLineEdit.size();

    this->waitTimer.setSingleShot(true);
    this->waitTimer.setInterval(500);

    this->SetupColors();

    this->InitializePlot();

    connect(ui->calcButton, &QPushButton::clicked, this, &MainWindow::OnCalcButtonClicked);

    for(size_t i = 0; i<this->numberOfFunctionInputs; ++i)
    {
        connect(ui->funcLineEdit[i], &QLineEdit::returnPressed, this, &MainWindow::OnReturnKeyPressed);
    }

    connect(&gameUpdateFutureWatcher, &QFutureWatcher<void>::finished, this, &MainWindow::OnGameUpdateFinished);
    connect(&waitTimer, &QTimer::timeout, this, &MainWindow::OnWaitTimerFinished);
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::SetupColors(){
    this->graphColors.push_back(QColor(0x00, 0x00, 0x00)); // Black
    this->graphColors.push_back(QColor(0x00, 0x00, 0xFF)); // Blue
    this->graphColors.push_back(QColor(0x94, 0x00, 0xD3)); // DarkViolet
    this->graphColors.push_back(QColor(0xA5, 0x2A, 0x2A)); // Brown
    this->graphColors.push_back(QColor(0xB0, 0xE0, 0xE6)); // PowderBlue

    this->activeGoodDotColor = QColor(0xAD, 0xD8, 0xE6);   // LightBlue
    this->inactiveGoodDotColor = QColor(0x00, 0x00, 0x8B); // DarkBlue
    this->activeBadDotColor = QColor(0xFF, 0x45, 0x00);    // OrangeRed
    this->inactiveBadDotColor = QColor(0xFF, 0xB6, 0xC1);  // LightPink
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
    auto dots = this->gamePoc.GetDots();

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
    auto& graphs = this->gamePoc.GetGraphs();

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

    ui->plot->replot();
}

void MainWindow::StartCalculation()
{
    SetGameIsBusy(true);

    std::vector<std::string> funcStrings;

    for(size_t i = 0; i<this->numberOfFunctionInputs; ++i)
    {
        funcStrings.emplace_back(this->ui->funcLineEdit[i]->text().toLocal8Bit().constData());
    }

    QFuture<void> updateFuture = QtConcurrent::run([=](){
        this->gamePoc.Update(funcStrings);
    });
    this->gameUpdateFutureWatcher.setFuture(updateFuture);

    this->waitTimer.start();
}

void MainWindow::OnCalcButtonClicked()
{
    this->StartCalculation();
}

void MainWindow::OnReturnKeyPressed()
{
    this->StartCalculation();
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
                "Plotting",
                "Waiting for plot to complete ...");

    auto * button = new QPushButton("Stop waiting");
    this->waitingMessageBox->addButton(button, QMessageBox::ButtonRole::NoRole);

    connect(&(*(this->waitingMessageBox)), &QMessageBox::buttonClicked, this, &MainWindow::OnWaitingMessageBoxButtonClicked);

    this->waitingMessageBox->exec();
}

void MainWindow::OnWaitingMessageBoxButtonClicked()
{
    this->gameUpdateFutureWatcher.cancel();
    this->gamePoc.Clear();
    this->UpdateGui();
}
