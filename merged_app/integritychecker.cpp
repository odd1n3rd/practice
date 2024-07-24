#include "integritychecker.h"
#include <QVBoxLayout>
#include <QProcess>
#include <QCryptographicHash>
#include <QCoreApplication>
#include <QRegularExpression>
#include <QFile>
#include <QTemporaryFile>
#include <botan/hash.h>
#include <botan/gost_3411.h>
#include <sys/mman.h>
#include <unistd.h>
#include <QDateTime>

IntegrityChecker::IntegrityChecker(QWidget *parent)
    : QWidget(parent), statusLabel(nullptr), initialHashLabel(nullptr), currentHashLabel(nullptr)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    statusLabel = new QLabel("Integrity check running...", this);
    layout->addWidget(statusLabel);

    initialHashLabel = new QLabel(this);
    layout->addWidget(initialHashLabel);

    currentHashLabel = new QLabel(this);
    layout->addWidget(currentHashLabel);

    initialHash = calculateHash();
    initialHashLabel->setText("Initial Hash: " + initialHash.toHex());

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &IntegrityChecker::checkIntegrity);
    timer->start(1000); 

    QTimer::singleShot(0, this, &IntegrityChecker::checkIntegrity);
}

void IntegrityChecker::checkIntegrity()
{
    QByteArray currentHash = calculateHash();
    currentHashLabel->setText("Current Hash: " + currentHash.toHex());

    if (currentHash != initialHash)
    {
        statusLabel->setText("Обнаружено нарушение целостности");
    }
    else
    {
        statusLabel->setText("Целостность не нарушена");
    }
}


QByteArray IntegrityChecker::calculateHash()
{
    QFile mapsFile(QString("/proc/%1/maps").arg(QCoreApplication::applicationPid()));
    if (!mapsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open /proc/[pid]/maps";
        return QByteArray();
    }

    std::unique_ptr<Botan::HashFunction> hash(new Botan::GOST_34_11);

    while (!mapsFile.atEnd()) {
        QString line = mapsFile.readLine();
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);
        if (parts.size() < 6) continue;

        QStringList addressRange = parts[0].split("-");
        if (addressRange.size() != 2) continue;

        bool ok;
        qulonglong start = addressRange[0].toULongLong(&ok, 16);
        if (!ok) continue;
        qulonglong end = addressRange[1].toULongLong(&ok, 16);
        if (!ok) continue;

        if (parts[1][0] == 'r' && !parts[5].startsWith("[v")) {
            QFile memFile(QString("/proc/%1/mem").arg(QCoreApplication::applicationPid()));
            if (memFile.open(QIODevice::ReadOnly)) {
                memFile.seek(start);
                QByteArray buffer = memFile.read(end - start);
                hash->update(reinterpret_cast<const uint8_t*>(buffer.constData()), buffer.size());
                memFile.close();
            }
        }
    }

    mapsFile.close();

    Botan::secure_vector<uint8_t> result = hash->final();
    return QByteArray(reinterpret_cast<const char*>(result.data()), result.size());
}


