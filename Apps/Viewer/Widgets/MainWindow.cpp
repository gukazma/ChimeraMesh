#include "MainWindow.h"
#include "GlobalSignalEmiiter.h"
#include "QRibbon.h"
#include "Scene/SceneManager.h"
#include "ui_MainWindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QFileDialog>
#include <QProcess>
#include <QSettings>
#include <QTranslator>
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initConnect();
    QRibbon::install(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionChinese_triggered()
{
    QSettings settings("Soarscape", "UVAMVS");
    settings.setValue("Language", "zh_CN");
    QApplication::exit(0);
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void MainWindow::on_actionOpenMesh_triggered()
{
    auto result = QFileDialog::getOpenFileName(this, tr("Open mesh"), "", "ply (*.ply);;osgb (*.osgb)");
    if (!result.isEmpty()) {
        emit g_globalSignalEmitter.signal_openMesh(result.toLocal8Bit().constData());
    }
}

void MainWindow::on_actionScale_triggered()
{
    emit g_globalSignalEmitter.signal_scale();
}

void MainWindow::initConnect()
{
    connect(&g_globalSignalEmitter,
            SIGNAL(signal_openMesh(const boost::filesystem::path&)),
            &g_sceneManager,
            SLOT(slot_openMesh(const boost::filesystem::path&)));
    connect(&g_globalSignalEmitter, SIGNAL(signal_scale()), &g_sceneManager, SLOT(slot_scale()));
}

void MainWindow::on_actionEnglish_triggered()
{
    QSettings settings("Soarscape", "UVAMVS");
    settings.setValue("Language", "en_US");
    QApplication::exit(0);
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}
