/**
 * @file WizardFilePage.cpp
 * @brief Implementation of WizardFilePage managing file inputs and experimental notes in a wizard interface.
 * 
 * This class extends WizardPage and includes a file selection widget and a notes text edit,
 * arranged in a vertical splitter with configurable stretch factors.
 * Provides methods for setting/getting key base and serializing data into a QVariantMap.
 * 
 * @author Aleksandar Demic <A.Demic@leeds.ac.uk>
 */

#include "WizardFilePage.h"
#include "ui/components/wizard/WizardFileFieldWidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QSplitter>
#include <QWidget>
#include <QRegularExpression>  

/**
 * @brief Constructs a WizardFilePage with file field and experimental notes section.
 * 
 * @param title The page title displayed on the wizard page.
 * @param fileVariableName The variable name associated with the file field widget.
 * @param parent Optional parent widget.
 * 
 * Sets up UI components including:
 * - A WizardFileFieldWidget for file management - allows you to select multiple files
 * - A QTextEdit for optional experimental notes with placeholder text.
 * - A vertical QSplitter to layout the components with proportional sizes.
 */
WizardFilePage::WizardFilePage(const QString &title, const QString &fileVariableName, QWidget *parent)
    : WizardPage(title, parent)
{
    auto splitter = new QSplitter(Qt::Vertical, this);
    splitter->setHandleWidth(1);

    // File widget container with extra top margin
    auto fileContainer = new QWidget(this);
    auto fileLayout = new QVBoxLayout(fileContainer);
    fileLayout->setContentsMargins(10, 20, 10, 10);
    fileLayout->addWidget(new WizardFileFieldWidget(this, title, fileVariableName));
    splitter->addWidget(fileContainer);

    // Notes container with top margin before label
    auto notesContainer = new QWidget(this);
    auto notesLayout = new QVBoxLayout(notesContainer);
    notesLayout->setContentsMargins(10, 15, 10, 10);
    notesLayout->addWidget(new QLabel("Experimental Notes (max ~200 words):", this));

    notesEdit = new QTextEdit(this);
    notesEdit->setPlaceholderText("OPTIONAL. Describe the experimental setup, conditions, or observations here, e.g. details of lock-in sensitivity, time constant, any problems during measurements (e.g. no purge box)");
    notesEdit->setMinimumHeight(notesEdit->sizeHint().height() * 0.2);  // Reduce height by 60%
    notesLayout->addWidget(notesEdit);
    splitter->addWidget(notesContainer);

	splitter->setStretchFactor(0, 7.5);
	splitter->setStretchFactor(1, 2.5);


    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(splitter);
    setLayout(mainLayout);
}

/**
 * @brief Sets the key base string used as a prefix for keys in maps.
 * 
 * @param key The key base string to set.
 */
void WizardFilePage::setKeyBase(const QString &key)
{
    keyBase = key;
}

/**
 * @brief Returns the key base string if set, otherwise generates one based on the window title.
 * 
 * The generated key is lowercased, spaces and hyphens replaced with underscores,
 * and non-alphanumeric characters removed.
 * 
 * @return QString The key base string.
 */
QString WizardFilePage::getKeyBase() const
{
    if (!keyBase.isEmpty()) {
        return keyBase;
    }

    QString key = windowTitle().toLower();
    key.replace(QRegularExpression("[\\s\\-]+"), "_");
    key.remove(QRegularExpression("[^a-z0-9_]"));

    return key;
}

/**
 * @brief Adds the page's data to the provided QVariantMap.
 * 
 * Calls the base WizardPage addToMap method, then adds the trimmed experimental notes
 * under a key constructed from the prefix and key base.
 * 
 * @param map The QVariantMap to add data to.
 * @param keyPrefix The prefix to prepend to keys.
 */
void WizardFilePage::addToMap(QVariantMap &map, const QString &keyPrefix)
{
    WizardPage::addToMap(map);

    if (notesEdit) {
        QString key = keyPrefix + getKeyBase() + "_experimental_notes";
        map[key] = notesEdit->toPlainText().trimmed();
    }
}
