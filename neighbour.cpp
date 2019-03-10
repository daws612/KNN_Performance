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
