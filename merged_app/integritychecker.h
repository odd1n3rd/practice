#ifndef INTEGRITYCHECKER_H
#define INTEGRITYCHECKER_H

#include <QWidget>
#include <QTimer>
#include <QLabel>

class IntegrityChecker : public QWidget
{
    Q_OBJECT

public:
    IntegrityChecker(QWidget *parent = nullptr);

private slots:
    void checkIntegrity();


private:
    QByteArray calculateHash();
    QByteArray initialHash;
    QTimer *timer;
    QLabel *statusLabel;
    QLabel *initialHashLabel;
    QLabel *currentHashLabel;
};

#endif // INTEGRITYCHECKER_H
