#ifndef NEIGHBOUR_H
#define NEIGHBOUR_H

#include <QObject>
#include <QVector>

class Neighbour : public QObject
{
    Q_OBJECT
public:
    explicit Neighbour();

    double getDistance() const;
    void setDistance(const double &distance);

    int getClassification() const;
    void setClassification(const int &classification);

    QVector<double> getFeatures() const;
    void setFeatures(const QVector<double> &features);
    void addFeature(const double feature);

    QString getClassName() const;
    void setClassName(const QString &className);

    QString getDescription();
private:
    double m_distance;
    int m_classification;
    QVector<double> m_features;
    QString m_className;

};

#endif // NEIGHBOUR_H
