#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QTcpServer>
#include "server.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "server_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    Server s;

    return a.exec();
}
