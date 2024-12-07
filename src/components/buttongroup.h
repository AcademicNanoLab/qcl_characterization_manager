#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QAbstractButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QStyle>
#include "pushbutton.h" // IWYU pragma: keep (lots of files that include buttongroup.h will need pushbutton.h)

class Wizard;

// Combines usage of QButtonGroup and QGroup.
// Add new buttons using addButton().
class ButtonGroup : public QGroupBox
{
    Q_OBJECT
    QButtonGroup *buttonGroup;

public:
    enum BoxLayout { VLayout, HLayout };
    explicit ButtonGroup(BoxLayout boxLayout, QWidget *parent = nullptr);
    void addButton(QAbstractButton *button, int id = -1);
    void addMenuButton(const QObject *object,
                       const char *changeSignal,
                       const QString &text,
                       int id = -1);
    void addImageButton(const QObject *object,
                        const char *changeSignal,
                        const QString &filePath,
                        int id = -1);
    void initButton();
    void clear();

signals:
    void buttonClicked(int id);
    void buttonClickedId(int id);
    void buttonClickedText(QString id);
    void buttonClickedPtr(QAbstractButton *button);

public slots:
    void idClickedSlot(int id)
    {
        emit buttonClickedId(id);
        emit buttonClicked(0);
    }
    void buttonClickedSlot(QAbstractButton *button)
    {
        emit buttonClickedText(button->text());
        emit buttonClickedPtr(button);
    }
};

#endif // BUTTONGROUP_H
