#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QTranslator;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionEnglish_triggered();
    void on_actionChinese_triggered();
    void on_actionOpenMesh_triggered();
    void on_actionScale_triggered();

private:
    void initConnect();

private:
    Ui::MainWindow* ui;
};

#endif   // MAINWINDOW_H
