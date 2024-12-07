#include "headerpage.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include "text.h"

HeaderPage::HeaderPage(const QString &title, QWidget *parent)
    : QScrollArea{parent}
    , title(title)
    , contents(new Widget())
{
    QVBoxLayout *layout = new QVBoxLayout();

    // specify layout
    contents->setLayout(layout);
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0, 0, 0, 0);

    // add the title
    layout->addWidget(new Text(title, "h1", contents));

    // add contents widget, resize to use extra space
    setWidget(contents);
    setWidgetResizable(true);
}

// This adds widgets to the page
void HeaderPage::addWidget(QWidget *widget)
{
    contents->layout()->addWidget(widget);
}

// This returns the page's title
QString HeaderPage::getTitle()
{
    return title;
}
