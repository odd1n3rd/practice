#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVector>
#include <QMap>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

class DraggableButton : public QPushButton
{
    Q_OBJECT

public:
    DraggableButton(const QString& text, QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QPoint dragStartPosition;
};

class VirtualKeyboard : public QWidget
{
    Q_OBJECT

public:
    VirtualKeyboard(QWidget* parent = nullptr);
    void switchLayout();

signals:
    void keyPressed(const QString& key);

private slots:
    void onKeyPress();
    void randomizeLayout();

private:
    void createButtons();
    void refreshButtons();

    QVBoxLayout* layout;
    QMap<QString, QVector<QVector<QString>>> keys;
    QString currentLayout;

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
};

class TextEditor : public QWidget
{
    Q_OBJECT

public:
    TextEditor(QWidget* parent = nullptr);

private slots:
    void onKeyPress(const QString& key);
    void closeApplication();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    QVBoxLayout* layout;
    QTextEdit* textEdit;
    VirtualKeyboard* keyboard;
    QPushButton* exitButton;
    bool ctrlPressed;
    bool shiftPressed;
};

#endif // MAIN_H
