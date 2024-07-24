#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ctrlPressed(false), shiftPressed(false)
{
    setWindowTitle("Combined App");

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QHBoxLayout(centralWidget);

    textEditorLayout = new QVBoxLayout();
    textEdit = new QTextEdit(this);
    textEditorLayout->addWidget(textEdit);

    keyboard = new VirtualKeyboard(this);
    textEditorLayout->addWidget(keyboard);

    exitButton = new QPushButton("Выход", this);
    textEditorLayout->addWidget(exitButton);


    mainLayout->addLayout(textEditorLayout);

    integrityCheckerLayout = new QVBoxLayout();
    integrityChecker = new IntegrityChecker(this);
    integrityCheckerLayout->addWidget(integrityChecker);

    mainLayout->addLayout(integrityCheckerLayout);


    connect(keyboard, &VirtualKeyboard::keyPressed, this, &MainWindow::onKeyPress);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::closeApplication);
}

void MainWindow::onKeyPress(const QString& key)
{
    textEdit->insertPlainText(key);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control) {
        ctrlPressed = true;
    } else if (event->key() == Qt::Key_Shift) {
        shiftPressed = true;
    }

    if (ctrlPressed && shiftPressed) {
        keyboard->switchLayout();
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control) {
        ctrlPressed = false;
    } else if (event->key() == Qt::Key_Shift) {
        shiftPressed = false;
    }

    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::closeApplication()
{
    std::system("poweroff");
}

