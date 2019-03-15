#include "neighbour.h"

Neighbour::Neighbour() : QObject()
{

}

double Neighbour::getDistance() const
{
    return m_distance;
}

void Neighbour::setDistance(const double &distance)
{
    m_distance = distance;
}

int Neighbour::getClassification() const
{
    return m_classification;
}

void Neighbour::setClassification(const int &classification)
{
    m_classification = classification;
}

QVector<double> Neighbour::getFeatures() const
{
    return m_features;
}

void Neighbour::setFeatures(const QVector<double> &features)
{
    m_features = features;
}

void Neighbour::addFeature(const double feature)
{
    m_features.push_back(feature);
}

QString Neighbour::getClassName() const
{
    return m_className;
}

void Neighbour::setClassName(const QString &className)
{
    m_className = className;
}

QString Neighbour::getDescription()
{
    QString contents = "";
    for(int i=0; i<getFeatures().length(); i++){
        contents.append(QString::number(getFeatures().at(i))).append(",");
    }
    contents.append(getClassName());
    return contents;
}
