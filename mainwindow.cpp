#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QFile>
#include <QtAlgorithms>
#include "neighbour.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionImport_Dataset_triggered()
{
    clearUI();
    columnNames.clear();
    columnKeys.clear();

    filename = QFileDialog::getOpenFileName(this, "Dataset file");

    if(filename.length() < 1) {
        QMessageBox::warning(this, "Import dataset error", "No file was selected");
        return;
    }

    QFile datasetFile(filename);

    if(datasetFile.open(QIODevice::ReadOnly)) {

        QTextStream in(&datasetFile);

        //Assumed comma is used and first row is column headers
        QString header = in.readLine();
        columnNames = header.split(",");

        //create list for keys of each column
        for(int i=0; i<columnNames.length(); i++) {
            columnKeys.append(QStringList());
        }

        ui->cbClassColumn->clear();
        ui->cbClassColumn->addItem("Please Select");
        ui->cbClassColumn->addItems(columnNames);
        ui->cbClassColumn->setEnabled(true);
        ui->cbClassColumn->setCurrentIndex(ui->cbClassColumn->count() - 1);

        datasetFile.close();

    } else {
        QMessageBox::warning(this, "Import dataset error", "Unable to open selected file");
        return;
    }
}

void MainWindow::performKNN()
{

    splitTrainAndTestData();

    performPrediction();
}

void MainWindow::performPrediction()
{

    int errors = 0;

    for(int t = 0; t < testPoints.length(); t++) {

        Neighbour *testPoint = testPoints.at(t);

        // Fill distances of all points from p
        for (int i = 0; i < trainPoints.length(); i++) {

            Neighbour *observation = trainPoints.at(i);
            double testDistance = EuclideanDistance(observation->getFeatures(), testPoint->getFeatures());

            observation->setDistance(testDistance);

        }

        QString prediction = getPredictionOfKNeighbours(3);

        ui->txtTestData->append(testPoint->getDescription().append(" Classified as: ").append(prediction));
        if(prediction != testPoint->getClassName())
            errors++;
    }
    ui->txtTestData->append("Total Errors: " + QString::number(errors));
}

double MainWindow::EuclideanDistance(QVector<double> observation, QVector<double> test)
{
    // The sum of the squared
    // differences of the elements
    double S = 0;

    for (int i = 0; i<observation.length(); i++)
    {
        S += pow(observation.at(i) - test.at(i), 2);
    }

    // The square root of the sum
    return sqrt(S);
}

Neighbour *MainWindow::createNeighbourObject(QString observation)
{
    Neighbour *point = new Neighbour();

    QStringList lineItem = observation.split(",");

    // by default class is assumed to be at the last column
    classColumnIndex = ui->cbClassColumn->currentIndex() - 1;

    for(int i=0; i<lineItem.length(); i++) {

        bool ok;
        QString(lineItem.at(i)).toInt(&ok);
        if(!ok)
            QString(lineItem.at(i)).toDouble(&ok);

        QStringList list = columnKeys.at(i);
        if(!ok){
            //Check if column at i contains unique item in the line being read at i
            if(!list.contains(lineItem.at(i))){
                list.append(lineItem.at(i));
                //replace to contain the list with updated unique column item
                columnKeys.replace(i, list);
            }
        }

        if(i == classColumnIndex) {
            point->setClassName(lineItem.at(i));
        } else{
            point->addFeature(list.length() > 0 ? list.indexOf(lineItem.at(i)) : QString(lineItem.at(i)).toDouble());
        }
    }
    return point;
}

void MainWindow::displayColumnInfo()
{
    //Display normalized data and keys
    for(int i=0; i<columnNames.length(); i++) {
        ui->txtColumnText->append(columnNames.at(i) + ":");
        QStringList line = columnKeys.at(i);
        if(line.length() == 0){
            ui->txtColumnText->append("\tNumerical Column");
            // Get the index of the value to disable
            QModelIndex index = ui->cbClassColumn->model()->index(i+1, 0);

            // This is the effective 'disable' flag
            QVariant v(0);

            // the magic
            ui->cbClassColumn->model()->setData(index, v, Qt::UserRole - 1);
        } else {
            for(int j=0; j<line.length(); j++)
                ui->txtColumnText->append("\t" + QString::number(j) + " - " + line.at(j));
        }
    }
}

void MainWindow::splitTrainAndTestData()
{
    //Shuffle dataset
    std::random_shuffle(allDataPoints.begin(), allDataPoints.end());

    int observationCount = allDataPoints.length();

    int testDataCountIndex = observationCount * 0.7;

    testPoints = allDataPoints;
    testPoints.remove(0, testDataCountIndex);

    trainPoints = allDataPoints;
    trainPoints.remove(testDataCountIndex, trainPoints.count() - testDataCountIndex);
    ui->lblDataCount->setText(QString::number(trainPoints.length()) + " observations");
}

bool compareNeighbours(const Neighbour *neighbor1, const Neighbour *neighbor2)
{
    if(neighbor1->getDistance() == neighbor2->getDistance())
        return neighbor1 < neighbor2;
    return neighbor1->getDistance() < neighbor2->getDistance();
}

QString MainWindow::getPredictionOfKNeighbours(int k)
{
    // Sort the Points by distance from p CHECK ME
    std::sort(trainPoints.begin(), trainPoints.end(), compareNeighbours);

    QString classNames = "";
    for(int j=0; j<k; j++) {
        classNames.append(trainPoints.at(j)->getClassName());
    }

    QStringList classifications = columnKeys.at(classColumnIndex);
    QString prediction = "";
    int max = 0;
    for(int c=0; c<classifications.length(); c++) {
        if(classNames.count(classifications.at(c)) > max){
            prediction = classifications.at(c);
            max = classNames.count(classifications.at(c));
        }
    }

    return prediction;
}

void MainWindow::clearUI()
{
    ui->txtColumnText->setText("");
    ui->txtNormalizedData->setText("");
    ui->txtTestData->setText("");

    allDataPoints.clear();
}

void MainWindow::on_cbClassColumn_currentIndexChanged(int index)
{
    if(index < 1)
        return;

    readAndCreateDataSet();
    performKNN();
}

void MainWindow::readAndCreateDataSet()
{
    clearUI();
    QFile datasetFile(filename);

    if(datasetFile.open(QIODevice::ReadOnly)) {

        QTextStream in(&datasetFile);
        in.readLine();

        while(!in.atEnd()) {
            QString line = in.readLine();
            Neighbour *point = createNeighbourObject(line);
            ui->txtNormalizedData->append(point->getDescription());
            allDataPoints.append(point);
        }

        displayColumnInfo();

        datasetFile.close();

    } else {
        QMessageBox::warning(this, "Import dataset error", "Unable to open selected file");
        return;
    }
}

void MainWindow::on_btnReClassify_clicked()
{
    if(classColumnIndex < 0)
        return;
    performKNN();
}
