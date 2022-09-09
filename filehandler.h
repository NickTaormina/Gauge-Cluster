#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>

class fileHandler : public QObject
{
    Q_OBJECT
public:
    explicit fileHandler(QObject *parent = nullptr);

signals:

};

#endif // FILEHANDLER_H
