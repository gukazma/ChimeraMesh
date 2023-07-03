#include "Widgets/MainWindow.h"
#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QTranslator>
#include <iostream>
int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    // initialize language
    QSettings settings("Soarscape", "UVAMVS");
    QString   language = "zh_CN";
    if (settings.contains("Language")) {
        language = settings.value("Language").toString();
    }
    else {
        settings.setValue("Language", "zh_CN");
    }

    QTranslator* translator = new QTranslator;
    translator->load(":/Translations/" + language + ".qm");
    app.installTranslator(translator);

    // initialize style
    QFile file(":/style.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);

    MainWindow* mainwindow = new MainWindow;
    mainwindow->showMaximized();

    return app.exec();
}
