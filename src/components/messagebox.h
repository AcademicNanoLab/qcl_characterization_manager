#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QMessageBox>

// Wrapper for QMessageBox that ensures cursor changes to pointer for buttons in QMessageBox
class MessageBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit MessageBox(QMessageBox::Icon icon,
                        const QString &title,
                        const QString &text,
                        QMessageBox::StandardButtons buttons = NoButton,
                        QWidget *parent = nullptr,
                        Qt::WindowFlags f = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
signals:
};

#endif // MESSAGEBOX_H
