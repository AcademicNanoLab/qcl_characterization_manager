#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include "components/widget.h"

class WelcomePage : public Widget
{
    Q_OBJECT
public:
    explicit WelcomePage(QWidget *parent = nullptr);

signals:
    void buttonClickedId(int id);

public slots:
    void buttonClickedIdSlot(int id) { emit buttonClickedId(id + 1); }
};

#endif // WELCOMEPAGE_H
