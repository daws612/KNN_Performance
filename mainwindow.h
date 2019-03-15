#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

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

    void displayColumnInfo();
    void splitTrainAndTestData();
    QString getPredictionOfKNeighbours(int k);
    void clearUI();
    void readAndCreateDataSet();
    void performPrediction();
private slots:
    void on_actionImport_Dataset_triggered();

    void on_cbClassColumn_currentIndexChanged(int index);

    void on_btnReClassify_clicked();

private:
    Ui::MainWindow *ui;
    QStringList columnNames;
    QList<QStringList> columnKeys;
    int classColumnIndex;
    QVector<Neighbour*> allDataPoints;
    QVector<Neighbour*> trainPoints;
    QVector<Neighbour*> testPoints;
    QString filename;

    void performKNN();
    double EuclideanDistance(QVector<double> observation, QVector<double> test);

    Neighbour *createNeighbourObject(QString observation);
};

#endif // MAINWINDOW_H
