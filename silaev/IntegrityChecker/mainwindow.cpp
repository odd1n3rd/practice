#include "mainwindow.h"
#include <QFile>
#include <QCryptographicHash>
#include <QIODevice>
#include <QMessageBox>
#include <cstring>
#include <iostream>


int MainWindow::dataVariable = 0;
int MainWindow::textVariable = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    logOutput = new QTextEdit(this);
    logOutput->setReadOnly(true);

    checkButton = new QPushButton("Check Integrity", this);
    displayButton = new QPushButton("Display Known Hashes", this);
    displayCurrentButton = new QPushButton("Display Current Hashes", this);
    modifyDataButton = new QPushButton("Modify Data", this);
    modifyTextButton = new QPushButton("Modify Text", this);

    layout = new QVBoxLayout();
    layout->addWidget(checkButton);
    layout->addWidget(displayButton);
    layout->addWidget(displayCurrentButton);
    layout->addWidget(modifyDataButton);
    layout->addWidget(modifyTextButton);
    layout->addWidget(logOutput);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);


    knownHashes.insert("data", "df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119");
    knownHashes.insert("text", "df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119");

    connect(checkButton, &QPushButton::clicked, this, &MainWindow::checkIntegrity);
    connect(displayButton, &QPushButton::clicked, this, &MainWindow::displayKnownHashes);
    connect(displayCurrentButton, &QPushButton::clicked, this, &MainWindow::displayCurrentHashes);
    connect(modifyDataButton, &QPushButton::clicked, this, &MainWindow::modifyData);
    connect(modifyTextButton, &QPushButton::clicked, this, &MainWindow::modifyText);


    integrityCheckTimer = new QTimer(this);
    connect(integrityCheckTimer, &QTimer::timeout, this, &MainWindow::checkIntegrity);
    integrityCheckTimer->start(10000);
}

MainWindow::~MainWindow()
{

    delete logOutput;
    delete checkButton;
    delete displayButton;
    delete displayCurrentButton;
    delete modifyDataButton;
    delete modifyTextButton;
    delete layout;
    delete integrityCheckTimer;
}

void MainWindow::checkIntegrity()
{
    QStringList results;

    QString dataHash = calculateHash(reinterpret_cast<const char*>(&dataVariable), sizeof(dataVariable));
    if (dataHash == knownHashes.value("data")) {
        results.append("Data: OK");
    } else {
        results.append("Data: CORRUPTED");
    }

    QString textHash = calculateHash(reinterpret_cast<const char*>(&textVariable), sizeof(textVariable));
    if (textHash == knownHashes.value("text")) {
        results.append("Text: OK");
    } else {
        results.append("Text: CORRUPTED");
    }

    logOutput->clear();
    logOutput->append(results.join('\n'));
}

void MainWindow::displayKnownHashes()
{
    QStringList results;
    results.append("Known Hashes:");
    results.append(QString("Data: %1").arg(knownHashes.value("data")));
    results.append(QString("Text: %1").arg(knownHashes.value("text")));

    logOutput->clear();
    logOutput->append(results.join('\n'));
}

void MainWindow::displayCurrentHashes()
{
    QStringList results;
    results.append("Current Hashes:");
    results.append(QString("Data: %1").arg(calculateHash(reinterpret_cast<const char*>(&dataVariable), sizeof(dataVariable))));
    results.append(QString("Text: %1").arg(calculateHash(reinterpret_cast<const char*>(&textVariable), sizeof(textVariable))));

    logOutput->clear();
    logOutput->append(results.join('\n'));
}

void MainWindow::modifyData()
{
    dataVariable++;
    logOutput->append("Data modified");
}

void MainWindow::modifyText()
{
    textVariable++;
    logOutput->append("Text modified");
}

QString MainWindow::calculateHash(const char* data, size_t size)
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(data, size);
    return hash.result().toHex();
}
