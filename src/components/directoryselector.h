#ifndef DIRECTORYSELECTOR_H
#define DIRECTORYSELECTOR_H

#include <QFileDialog>
#include <QLineEdit>
#include "widget.h"

class DirectorySelector : public Widget
{
    Q_OBJECT
public:
    explicit DirectorySelector(QObject *dirChangeObject,
                               const char *dirChangeAction,
                               const QString &buttonText,
                               QObject *buttonClickObject,
                               const char *buttonClickAction,
                               QWidget *parent = nullptr);
    QLineEdit *chooseDirTextEdit;
    QFileDialog *fileDialog;

signals:
    void dirChanged(const QString &dir);

public slots:
    void chooseDir();
};

#endif // DIRECTORYSELECTOR_H
