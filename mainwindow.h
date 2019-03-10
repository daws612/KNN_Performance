#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Neighbour;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionImport_Dataset_triggered();

private:
    Ui::MainWindow *ui;
    QStringList normalizedDataSet;
    QStringList columnNames;
    QList<QStringList> dataSet;

    void performKNN();
    double EuclideanDistance(QStringList observation, QStringList test);
};

#endif // MAINWINDOW_H
