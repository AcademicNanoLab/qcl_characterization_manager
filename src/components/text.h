#ifndef TEXT_H
#define TEXT_H

#include <QLabel>

class Text : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QString variant READ getVariant WRITE setVariant NOTIFY variantChanged) 

public:
    explicit Text(const QString &text, const QString &variant, QWidget *parent = nullptr);
    QString getVariant();
    void setVariant(QString newVariant);

signals:
    void variantChanged(QString variant);

private:
    QString variant;
};

#endif // TEXT_H
