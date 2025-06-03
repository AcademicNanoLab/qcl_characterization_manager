/**
 * \file        main.cpp
 * \brief       Entry point for the application. Loads and applies styles, then launches the main window.
 * \author      Aleksandar Demic <A.Demic@leeds.ac.uk>
 */
 
#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QRegularExpression>

QMap<QString, QString> parseSassVariables(const QString &sassContent);
void setupStyle(QApplication *a);

/**
 * @brief The main entry point of the Qt application.
 *
 * Initializes the QApplication object, applies styling, and shows the main window.
 *
 * @param argc Argument count
 * @param argv Argument values
 * @return Exit status from the QApplication event loop
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setupStyle(&a);

    MainWindow w;
    w.show();
    return a.exec();
}

/**
 * @brief Applies the application's stylesheet using QSS with injected SASS variable values.
 *
 * This function loads a QSS file and a SASS file from resources. It parses SASS variables and replaces 
 * them in the QSS stylesheet before applying it to the application.
 *
 * @param a Pointer to the QApplication instance
 */
void setupStyle(QApplication *a)
{
    // get qss file with style definitions
    QFile qssFile(":/src/resources/style/style.qss");
    qssFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(qssFile.readAll());
    qssFile.close();

    // get sass file with styling variables
    QFile sassFile(":/src/resources/style/style.sass");
    sassFile.open(QFile::ReadOnly);
    QString sassContent = QLatin1String(sassFile.readAll());
    sassFile.close();

    // replace variables in the stylesheet
    QMap<QString, QString> variables = parseSassVariables(sassContent);
    QMapIterator<QString, QString> i(variables);
    while (i.hasNext()) {
        i.next();
        styleSheet.replace(i.key(), i.value());
    }
    a->setStyleSheet(styleSheet);
}

/**
 * @brief Parses SASS-style variable declarations from a given string.
 *
 * Extracts variable names and their corresponding values using a regular expression.
 * Supports declarations in the format: `@variable = "value";`
 *
 * @param sassContent The content of the SASS file as a QString
 * @return A map of variable names (without `$`) to their corresponding values
 */
QMap<QString, QString> parseSassVariables(const QString &sassContent)
{
    QMap<QString, QString> variables;

    // Regular expression to match SASS variables (e.g., $variable: value;)
    static QRegularExpression regex(R"(@([\w-]+)\s*=\s*\"([^\"]+)\"\s*;?)");
    QRegularExpressionMatchIterator it = regex.globalMatch(sassContent);

    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QString variableName = match.captured(1);
        QString variableValue = match.captured(2).trimmed();
        variables[variableName] = variableValue;
    }

    return variables;
}