#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMap>
#include <QVector>

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

#endif // VIRTUALKEYBOARD_H
