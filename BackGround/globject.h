#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QObject>

class GLObject : public QObject
{
    Q_OBJECT
public:
    explicit GLObject(QObject *parent = nullptr);

private:
    static bool init();

signals:

};

#endif // GLOBJECT_H
