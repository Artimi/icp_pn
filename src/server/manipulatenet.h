#ifndef MANIPULATENET_H
#define MANIPULATENET_H
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>

class ManipulateNet
{
public:
    ManipulateNet();
    void saveNet();
    void loadNet(QString name);

private:
    int investigateVersion(QString name);
};

#endif // MANIPULATENET_H
