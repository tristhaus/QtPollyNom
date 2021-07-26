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
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <vector>
#include "qcustomplot.h"
#include "../Backend/game.h"
#include "../Backend/dotgenerator.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow
    {
    public:
        QMenuBar *menubar;
        QAction *newGameMenuAction;
        QAction *aboutMenuAction;
        QStatusBar *statusbar;
        QWidget *centralwidget;
        QVBoxLayout *verticalLayout_2;
        QCustomPlot *plot;
        QVBoxLayout *verticalLayout_3;
        QFrame *inputFrame0;
        QHBoxLayout *horizontalLayout;
        QFrame *inputFrame1;
        QVBoxLayout *verticalLayout;

        std::vector<QHBoxLayout*> funcLayout;
        std::vector<QLabel*> funcLabel;
        std::vector<QLineEdit*> funcLineEdit;

        QPushButton *calcButton;

        void setupUi(QMainWindow *MainWindow)
        {
            if (MainWindow->objectName().isEmpty())
                MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
            MainWindow->resize(600, 821);
            centralwidget = new QWidget(MainWindow);
            centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
            verticalLayout_2 = new QVBoxLayout(centralwidget);
            verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
            plot = new QCustomPlot(centralwidget);
            plot->setObjectName(QString::fromUtf8("plot"));
            verticalLayout_3 = new QVBoxLayout(plot);
            verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));

            verticalLayout_2->addWidget(plot);

            inputFrame0 = new QFrame(centralwidget);
            inputFrame0->setObjectName(QString::fromUtf8("inputFrame0"));
            QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
            sizePolicy.setHorizontalStretch(0);
            sizePolicy.setVerticalStretch(0);
            sizePolicy.setHeightForWidth(inputFrame0->sizePolicy().hasHeightForWidth());
            inputFrame0->setSizePolicy(sizePolicy);
            inputFrame0->setFrameShape(QFrame::StyledPanel);
            inputFrame0->setFrameShadow(QFrame::Raised);
            horizontalLayout = new QHBoxLayout(inputFrame0);
            horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
            inputFrame1 = new QFrame(inputFrame0);
            inputFrame1->setObjectName(QString::fromUtf8("inputFrame1"));
            inputFrame1->setFrameShape(QFrame::StyledPanel);
            inputFrame1->setFrameShadow(QFrame::Raised);
            verticalLayout = new QVBoxLayout(inputFrame1);
            verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

            funcLayout.emplace_back(new QHBoxLayout());
            funcLayout.back()->setObjectName(QString::fromUtf8("funcLayout0"));

            funcLabel.emplace_back(new QLabel(inputFrame1));
            funcLabel.back()->setObjectName(QString::fromUtf8("funcLabel0"));
            funcLayout.back()->addWidget(funcLabel.back());

            funcLineEdit.emplace_back(new QLineEdit(inputFrame1));
            funcLineEdit.back()->setObjectName(QString::fromUtf8("funcLineEdit0"));
            funcLayout.back()->addWidget(funcLineEdit.back());

            verticalLayout->addLayout(funcLayout.back());


            funcLayout.emplace_back(new QHBoxLayout());
            funcLayout.back()->setObjectName(QString::fromUtf8("funcLayout1"));

            funcLabel.emplace_back(new QLabel(inputFrame1));
            funcLabel.back()->setObjectName(QString::fromUtf8("funcLabel1"));
            funcLayout.back()->addWidget(funcLabel.back());

            funcLineEdit.emplace_back(new QLineEdit(inputFrame1));
            funcLineEdit.back()->setObjectName(QString::fromUtf8("funcLineEdit1"));
            funcLayout.back()->addWidget(funcLineEdit.back());

            verticalLayout->addLayout(funcLayout.back());


            funcLayout.emplace_back(new QHBoxLayout());
            funcLayout.back()->setObjectName(QString::fromUtf8("funcLayout2"));

            funcLabel.emplace_back(new QLabel(inputFrame1));
            funcLabel.back()->setObjectName(QString::fromUtf8("funcLabel2"));
            funcLayout.back()->addWidget(funcLabel.back());

            funcLineEdit.emplace_back(new QLineEdit(inputFrame1));
            funcLineEdit.back()->setObjectName(QString::fromUtf8("funcLineEdit2"));
            funcLayout.back()->addWidget(funcLineEdit.back());

            verticalLayout->addLayout(funcLayout.back());


            funcLayout.emplace_back(new QHBoxLayout());
            funcLayout.back()->setObjectName(QString::fromUtf8("funcLayout3"));

            funcLabel.emplace_back(new QLabel(inputFrame1));
            funcLabel.back()->setObjectName(QString::fromUtf8("funcLabel3"));
            funcLayout.back()->addWidget(funcLabel.back());

            funcLineEdit.emplace_back(new QLineEdit(inputFrame1));
            funcLineEdit.back()->setObjectName(QString::fromUtf8("funcLineEdit3"));
            funcLayout.back()->addWidget(funcLineEdit.back());

            verticalLayout->addLayout(funcLayout.back());


            funcLayout.emplace_back(new QHBoxLayout());
            funcLayout.back()->setObjectName(QString::fromUtf8("funcLayout4"));

            funcLabel.emplace_back(new QLabel(inputFrame1));
            funcLabel.back()->setObjectName(QString::fromUtf8("funcLabel4"));
            funcLayout.back()->addWidget(funcLabel.back());

            funcLineEdit.emplace_back(new QLineEdit(inputFrame1));
            funcLineEdit.back()->setObjectName(QString::fromUtf8("funcLineEdit4"));
            funcLayout.back()->addWidget(funcLineEdit.back());

            verticalLayout->addLayout(funcLayout.back());


            horizontalLayout->addWidget(inputFrame1);

            calcButton = new QPushButton(inputFrame0);
            calcButton->setObjectName(QString::fromUtf8("calcButton"));

            horizontalLayout->addWidget(calcButton);


            verticalLayout_2->addWidget(inputFrame0);

            MainWindow->setCentralWidget(centralwidget);
            menubar = new QMenuBar(MainWindow);
            menubar->setObjectName(QString::fromUtf8("menubar"));
            menubar->setGeometry(QRect(0, 0, 600, 21));

            auto newGameActionLabel = QCoreApplication::translate("MainWindow", "New Game", nullptr);
            newGameMenuAction = menubar->addAction(newGameActionLabel);
            newGameMenuAction->setObjectName(QString::fromUtf8("newGame"));

            //: Arg 1 is a placeholder for the program name
            auto aboutActionLabelTemplate = QCoreApplication::translate("MainWindow", "About %1", nullptr);
            auto aboutActionLabel = aboutActionLabelTemplate.arg(QCoreApplication::translate("MainWindow", "QtPollyNom", nullptr));
            aboutMenuAction = menubar->addAction(aboutActionLabel);
            aboutMenuAction->setObjectName(QString::fromUtf8("about"));
            MainWindow->setMenuBar(menubar);

            statusbar = new QStatusBar(MainWindow);
            statusbar->setObjectName(QString::fromUtf8("statusbar"));
            MainWindow->setStatusBar(statusbar);

            retranslateUi(MainWindow);

            QMetaObject::connectSlotsByName(MainWindow);
        }

        void retranslateUi(QMainWindow *MainWindow)
        {
            MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "QtPollyNom", nullptr));

            //: Arg 1 is the 1-based number of the function
            auto funcLabelTextTemplate = QCoreApplication::translate("MainWindow", "Function %1:", nullptr);

            for(unsigned int i = 0; i<funcLabel.size(); ++i)
            {
                auto text = funcLabelTextTemplate.arg(i + 1);
                funcLabel[i]->setText(text);
            }

            funcLineEdit[0]->setText(QString("1/x"));
            funcLineEdit[1]->setText(QString("(x-3.0)*(x+4.0)"));
            calcButton->setText(QCoreApplication::translate("MainWindow", "Calc!", nullptr));
        }

    };
}
QT_END_NAMESPACE

class MainWindowTest;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend MainWindowTest;

private:
    Ui::MainWindow *ui;
    size_t numberOfFunctionInputs;

    /*!
     * \brief focusIndicator indicates the focus before a calculation
     *
     * 0 to numberOfFunctionInputs-1 indicates the lineEdits
     * -1 indicates the button
     */
    int focusIndicator;

    Backend::Game game;
    QFutureWatcher<void> gameUpdateFutureWatcher;
    QTimer waitTimer;

    std::unique_ptr<QMessageBox> waitingMessageBox;
    std::unique_ptr<QMessageBox> aboutMessageBox;

    std::vector<QCPAbstractPlottable*> dotCurves;

    std::vector<QColor> graphColors;
    QColor activeGoodDotColor;
    QColor inactiveGoodDotColor;
    QColor activeBadDotColor;
    QColor inactiveBadDotColor;
    QPalette parseablePalette;
    QPalette nonParseablePalette;

public:
    /*!
     * \brief Initializes a new instance using the supplied dot generator.
     * \param dotGenerator The generator for dots to use.
     * \param parent The QT-defined parent.
     */
    MainWindow(std::shared_ptr<Backend::DotGenerator> dotGenerator, QWidget *parent = nullptr);

    /*!
     * \brief Initializes a new instance and lets Game decide its own dot generator.
     * \param parent The QT-defined parent.
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void OnNewGameMenuTriggered();
    void OnAboutMenuTriggered();
    void OnCalcButtonClicked();
    void OnReturnKeyPressed();
    void OnGameUpdateFinished();
    void OnWaitTimerFinished();
    void OnWaitingMessageBoxButtonClicked();
    void OnFuncLineEditTextChanged();

private:
    void InitializePlot();
    void SetupColors();
    void DrawDots();
    void DrawGraphs();
    void UpdateWindowTitle();
    void SetGameIsBusy(bool isBusy);
    void UpdateGui();
    void StartCalculation();
};
#endif // MAINWINDOW_H
