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
    QString filename = QFileDialog::getOpenFileName(this, "Dataset file");

    if(filename.length() < 1) {
        QMessageBox::warning(this, "Import dataset error", "No file was selected");
        return;
    }

    QFile datasetFile(filename);

    if(datasetFile.open(QIODevice::ReadOnly)) {

        ui->txtColumnText->setText("");
        ui->txtNormalizedData->setText("");
        ui->txtTestData->setText("");
        normalizedDataSet.clear();
        columnNames.clear();
        dataSet.clear();

        //Open a file to write the normalized data using numerical values
        QFile normalizedFile("normalizedDataSet");
        if(normalizedFile.open(QIODevice::ReadWrite)) {

            QTextStream in(&datasetFile);
            QTextStream out(&normalizedFile);

            QString header = in.readLine();
            columnNames = header.split(","); //Assumed , is used and first row is column headers


            for(int i=0; i<columnNames.length(); i++) {
                dataSet.append(QStringList());
            }

            while(!in.atEnd()) {
                QString normalizedLine = "";

                QString line = in.readLine();
                QStringList lineItem = line.split(",");
                for(int i=0; i<lineItem.length(); i++) {
                    QStringList list = dataSet.at(i);
                    if(!list.contains(lineItem.at(i))){
                        list.append(lineItem.at(i));
                        dataSet.replace(i, list);
                    }

                    //Insert row item in normalized state
                    normalizedLine.append(QString::number(list.indexOf(lineItem.at(i)))).append(",");
                }
                normalizedLine = normalizedLine.left(normalizedLine.length() - 1);
                out << normalizedLine;
                out << "\n";
                ui->txtNormalizedData->append(normalizedLine);
                normalizedDataSet.append(normalizedLine);
            }

            //Display normalized data and keys
            for(int i=0; i<columnNames.length(); i++) {
                ui->txtColumnText->append(columnNames.at(i) + ":");
                QStringList line = dataSet.at(i);

                for(int j=0; j<line.length(); j++)
                    ui->txtColumnText->append("\t" + QString::number(j) + " - " + line.at(j));
            }

            datasetFile.close();
            normalizedFile.close();

            performKNN();
        } else {
            QMessageBox::warning(this, "Create file error", "Unable to create file for normalized data");
            return;
        }

    } else {
        QMessageBox::warning(this, "Import dataset error", "Unable to open selected file");
        return;
    }
}

bool compareNeighbours(const Neighbour *neighbor1, const Neighbour *neighbor2)
{
    if(neighbor1->getDistance() == neighbor2->getDistance())
        return neighbor1 < neighbor2;
    return neighbor1->getDistance() < neighbor2->getDistance();
}

// This function finds classification of point p using
// k nearest neighbour algorithm. It assumes only two
// groups and returns 0 if p belongs to group 0, else
// 1 (belongs to group 1).
void MainWindow::performKNN()
{
    //Shuffle dataset
    std::random_shuffle(normalizedDataSet.begin(), normalizedDataSet.end());

    int observationCount = normalizedDataSet.length();
    ui->lblDataCount->setText(QString::number(normalizedDataSet.length()) + " observations");

    QList<Neighbour*> neighbours;
    int errors = 0;

    int testDataCountIndex = observationCount * 0.7;

    for(int t = testDataCountIndex; t < observationCount; t++) {

    QString test = normalizedDataSet.at(t);
    QStringList testItem = test.split(",");

    // Fill distances of all points from p
    for (int i = 0; i < observationCount * 0.7; i++){
        QString observation = normalizedDataSet.at(i);
        QStringList observationItem = observation.split(",");

        double testDistance = EuclideanDistance(observationItem, testItem);

        Neighbour *komsu = new Neighbour();
        komsu->setDistance(testDistance);
        komsu->setClassification(QString(observationItem.at(observationItem.length() - 1)).toInt());
        neighbours.append(komsu); //holds all distances of test point to all train points

    }

    // Sort the Points by distance from p
    std::sort(neighbours.begin(), neighbours.end(), compareNeighbours);

    std::vector<int> nearestK;
    int k = 3;
    for(int j=0; j<k; j++) {
        nearestK.push_back(neighbours.at(j)->getClassification());
    }

    int max = 0;
    QString gender = "";
    for(int i=0; i<columnNames.length(); i++) {
        if(columnNames.at(i) == "Gender"){
            QStringList line = dataSet.at(i);

            for(int j=0; j<line.length(); j++){
                int freq = std::count(nearestK.begin(), nearestK.end(), j);
                if(max < freq){
                    max = freq;
                    gender = line.at(j);
                }
               // ui->txtColumnText->append("\t" + QString::number(j) + " - " + line.at(j) + " occurs " + QString::number(freq) + " times");
            }
            if((gender == 'M' && testItem.at(testItem.length() - 1) == "0") || (gender == 'F' && testItem.at(testItem.length() - 1) == "1"))
                errors++;
            ui->txtTestData->append(test + " Classified as " + gender);
        }
    }

    }
    ui->txtTestData->append("Total Errors: " + QString::number(errors));
}


double MainWindow::EuclideanDistance(QStringList observation, QStringList test){
      
    // The sum of the squared  
    // differences of the elements 
    double S = 0;

    for (int i = 0; i<observation.length(); i++)
    {
        S += pow(QString(observation.at(i)).toDouble() - QString(test.at(i)).toDouble(), 2);
    }

    // The square root of the sum 
    return sqrt(S);
}


