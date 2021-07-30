#include "connector.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "WiFiConnector_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    Connector w;

    //w.setFixedSize(190,100); // setting the window size
    w.setWindowTitle("LED Control"); // setting window title
    w.show();

    w.show();
    return a.exec();
}
