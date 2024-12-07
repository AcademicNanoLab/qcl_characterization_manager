#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QRegularExpression>

QMap<QString, QString> parseSassVariables(const QString &sassContent);
void setupStyle(QApplication *a);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setupStyle(&a);

    MainWindow w;
    w.show();
    return a.exec();
}

void setupStyle(QApplication *a)
{
    // get qss file with style definitions
    QFile qssFile(":/src/style/style.qss");
    qssFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(qssFile.readAll());
    qssFile.close();

    // get sass file with styling variables
    QFile sassFile(":/src/style/style.sass");
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
