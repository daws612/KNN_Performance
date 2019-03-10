#ifndef NEIGHBOUR_H
#define NEIGHBOUR_H

#include <QObject>

class Neighbour : public QObject
{
    Q_OBJECT
public:
    explicit Neighbour();

    double getDistance() const;
    void setDistance(const double &distance);

    int getClassification() const;
    void setClassification(const int &classification);

private:
    double m_distance;
    int m_classification;

};

//static bool compare(const Neighbour& neighbor1, const Neighbour& neighbor2)
//{
//    if(neighbor1.getDistance() == neighbor2.getDistance())
//        return neighbor1 < neighbor2;
//    return neighbor1.getDistance() < neighbor2.getDistance();

//}

#endif // NEIGHBOUR_H
