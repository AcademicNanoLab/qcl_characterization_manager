/**
 * \file        mainwindow.cpp
 * \brief       Implements the MainWindow class. Initializes and connects UI pages for the application.
 * \author      Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "mainwindow.h"
#include <QStackedWidget>
#include "ui/pages/ProcessCustomPage.h"
#include "ui/pages/WelcomePage.h"
#include "ui/pages/WizardStack.h"
#include "ui_mainwindow.h"

/**
 * \brief Constructs the main window and initializes the stacked widget navigation.
 *
 * This sets up the central widget as a QStackedWidget containing the WelcomePage,
 * WizardStack, and ProcessCustomPage. It also establishes signal-slot connections
 * for navigation between these pages.
 *
 * \param parent The parent widget, passed to the QMainWindow base class.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStackedWidget *stackedWidget = new QStackedWidget();
    setCentralWidget(stackedWidget);

    // define different things
    WizardStack *wizard = new WizardStack();
    WelcomePage *welcomePage = new WelcomePage();
	ProcessCustomPage *processPage = new ProcessCustomPage("  Process Customised Grace & LaTex Files");

    // add the different pages/stacks
    stackedWidget->addWidget(welcomePage);
    stackedWidget->addWidget(wizard);
    stackedWidget->addWidget(processPage);

    // connect welcome page click with entering correct pages
    connect(welcomePage,
            &WelcomePage::buttonClickedId,
            stackedWidget,
            &QStackedWidget::setCurrentIndex);

    // connect wizard finished with return to welcome page
    connect(wizard, &Wizard::finished, stackedWidget, &QStackedWidget::setCurrentIndex);

    // connect analysis page "Back" with return to welcome page
	connect(processPage, &ProcessCustomPage::finished, stackedWidget, &QStackedWidget::setCurrentIndex);
}

/**
 * \brief Destructor for MainWindow. Cleans up the UI.
 */
MainWindow::~MainWindow()
{
    delete ui;
}