#include "mainwindow.h"
#include <QStackedWidget>
#include "pages/graphpage.h"
#include "pages/welcomepage.h"
#include "pages/wizardstack.h"
#include "ui_mainwindow.h"

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
    GraphPage *graphPage = new GraphPage();

    // add the different pages/stacks
    stackedWidget->addWidget(welcomePage);
    stackedWidget->addWidget(wizard);
    stackedWidget->addWidget(graphPage);

    // connect welcome page click with entering correct pages
    connect(welcomePage,
            &WelcomePage::buttonClickedId,
            stackedWidget,
            &QStackedWidget::setCurrentIndex);

    // connect wizard finished with return to welcome page
    connect(wizard, &Wizard::finished, stackedWidget, &QStackedWidget::setCurrentIndex);

    // connect wizard finished with giving fields to analysis page
    connect(wizard, &WizardStack::sendFields, graphPage, &GraphPage::setFields);

    // connect analysis page "Back" with return to welcome page
    connect(graphPage, &GraphPage::finished, stackedWidget, &QStackedWidget::setCurrentIndex);
}

MainWindow::~MainWindow()
{
    delete ui;
}
