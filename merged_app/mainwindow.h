#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QTextEdit>
#include "virtualkeyboard.h"
#include "integritychecker.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onKeyPress(const QString& key);
    void closeApplication();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    QWidget* centralWidget;
    QHBoxLayout* mainLayout;
    QVBoxLayout* textEditorLayout;
    QVBoxLayout* integrityCheckerLayout;
    QTextEdit* textEdit;
    VirtualKeyboard* keyboard;
    QPushButton* exitButton;
    QPushButton* violateButton;
    IntegrityChecker* integrityChecker;
    bool ctrlPressed;
    bool shiftPressed;
};

#endif
