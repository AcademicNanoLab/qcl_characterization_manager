#include "directoryselector.h"
#include <QHBoxLayout>
#include <QStandardPaths>
#include "pushbutton.h"
#include "text.h"

DirectorySelector::DirectorySelector(QObject *dirChangeObject,
                                     const char *dirChangeAction,
                                     const QString &buttonText,
                                     QObject *buttonClickObject,
                                     const char *buttonClickAction,
                                     QWidget *parent)
    : Widget{parent}
    , chooseDirTextEdit(new QLineEdit())
    , fileDialog(new QFileDialog())
{
    QHBoxLayout *layout = new QHBoxLayout();
    setLayout(layout);

    Text *chooseDirText = new Text("Directory:", "body");
    layout->addWidget(chooseDirText);

    chooseDirTextEdit->setDisabled(true);
    layout->addWidget(chooseDirTextEdit);

    PushButton *chooseDirButton = new PushButton("Choose", "contained-secondary");
    layout->addWidget(chooseDirButton);
    PushButton *button = new PushButton(buttonText, "contained-secondary");
    layout->addWidget(button);

    // connect signals
    connect(chooseDirButton, &PushButton::clicked, this, &DirectorySelector::chooseDir);
    connect(this, SIGNAL(dirChanged(QString)), dirChangeObject, dirChangeAction);
    connect(button, SIGNAL(clicked()), buttonClickObject, buttonClickAction);

    // setup file dialog
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (!dataPath.isEmpty())
        fileDialog->setDirectory(dataPath);

    // initialize
    chooseDirTextEdit->setText(dataPath);
}

void DirectorySelector::chooseDir()
{
    QString dir = fileDialog->getExistingDirectory();
    chooseDirTextEdit->setText(dir);
    emit dirChanged(dir);
}
