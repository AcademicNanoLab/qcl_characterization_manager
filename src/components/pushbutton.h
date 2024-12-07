#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>
#include <QStyle>

// QPushButton with variant property that allows for styling
class PushButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QString variant READ getVariant WRITE setVariant NOTIFY variantChanged)

public:
    explicit PushButton(const QIcon &icon, QWidget *parent = nullptr);
    explicit PushButton(const QString &text, const QString &variant, QWidget *parent = nullptr);
    QString getVariant();
    void setVariant(QString _variant);

signals:
    void variantChanged(QString variant);

private:
    QString variant;
};

// PushButton with specified id, responds to other buttons in ButtonGroup being clicked
class ButtonGroupButton : public PushButton
{
    Q_OBJECT
public:
    explicit ButtonGroupButton(int id,
                               const QString &text,
                               const QString &variant = "contained",
                               QWidget *parent = nullptr);
    int id;

public slots:
    void buttonClickedIdSlot(int id)
    {
        if (this->id < id) {
            this->setVariant("contained");
        } else if (this->id == id) {
            this->setVariant("outlined");
        } else {
            this->setVariant("disabled");
        }
        this->style()->polish(this);
    }
};

// QPushButton with image
class ImageButton : public ButtonGroupButton
{
    Q_OBJECT
public:
    explicit ImageButton(int id, const QString &filePath, QWidget *parent = nullptr);
    QString filePath;

signals:
    void buttonClicked(int id, const QString &filePath);

public slots:
    void clickedSlot() { emit buttonClicked(id, filePath); }
    void buttonClickedIdSlot(int id)
    {
        if (this->id == id)
            this->setVariant("outlined");
        else
            this->setVariant("disabled");
        this->style()->polish(this);
    }
};

#endif // PUSHBUTTON_H
