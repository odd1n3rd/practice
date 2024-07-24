#include "virtualkeyboard.h"
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QTextEdit>
#include <QRandomGenerator>
#include <QApplication>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QDrag>

DraggableButton::DraggableButton(const QString& text, QWidget* parent)
    : QPushButton(text, parent)
{
    setAcceptDrops(true);
}

void DraggableButton::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        dragStartPosition = event->pos();
    }
    QPushButton::mousePressEvent(event);
}

void DraggableButton::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }
    if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }

    QDrag* drag = new QDrag(this);
    QMimeData* mimeData = new QMimeData;
    mimeData->setText(text());
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec(Qt::MoveAction);
}

VirtualKeyboard::VirtualKeyboard(QWidget* parent) : QWidget(parent)
{
    setAcceptDrops(true);
    layout = new QVBoxLayout(this);
    keys["en"] = {
        {"`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "="},
        {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "\\"},
        {"a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'"},
        {"z", "x", "c", "v", "b", "n", "m", ",", ".", "/"},
        {"Space"}
    };
    keys["ru"] = {
        {"ё", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "="},
        {"й", "ц", "у", "к", "е", "н", "г", "ш", "щ", "з", "х", "ъ", "\\"},
        {"ф", "ы", "в", "а", "п", "р", "о", "л", "д", "ж", "э"},
        {"я", "ч", "с", "м", "и", "т", "ь", "б", "ю", "."},
        {"Пробел"}
    };
    currentLayout = "en";
    createButtons();
}

void VirtualKeyboard::createButtons()
{
    for (const auto& row : keys[currentLayout]) {
        QHBoxLayout* hLayout = new QHBoxLayout();
        for (const auto& key : row) {
            DraggableButton* button = new DraggableButton(key);
            connect(button, &DraggableButton::clicked, this, &VirtualKeyboard::onKeyPress);
            hLayout->addWidget(button);
        }
        layout->addLayout(hLayout);
    }
}

void VirtualKeyboard::onKeyPress()
{
    DraggableButton* button = qobject_cast<DraggableButton*>(sender());
    if (button) {
        QString key = button->text();
        if (key == "Space" || key == "Пробел") {
            emit keyPressed(" ");
        } else {
            emit keyPressed(key);
        }
        randomizeLayout();
    }
}

void VirtualKeyboard::randomizeLayout()
{
    for (auto& layout : keys) {
        // Horizontal shift
        for (auto& row : layout) {
            int shift = QRandomGenerator::global()->bounded(row.size());
            std::rotate(row.begin(), row.begin() + shift, row.end());
        }

        // Vertical shift
        int maxRowLength = 0;
        for (const auto& row : layout) {
            maxRowLength = qMax(maxRowLength, row.size());
        }
        for (int col = 0; col < maxRowLength; ++col) {
            QVector<QString> column;
            for (const auto& row : layout) {
                if (col < row.size()) {
                    column.push_back(row[col]);
                }
            }
            int shift = QRandomGenerator::global()->bounded(column.size());
            std::rotate(column.begin(), column.begin() + shift, column.end());
            int i = 0;
            for (auto& row : layout) {
                if (col < row.size()) {
                    row[col] = column[i++];
                }
            }
        }
    }
    refreshButtons();
}

void VirtualKeyboard::refreshButtons()
{
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    createButtons();
}



void VirtualKeyboard::switchLayout()
{
    currentLayout = (currentLayout == "en") ? "ru" : "en";
    refreshButtons();
}

void VirtualKeyboard::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    }
}

void VirtualKeyboard::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasText()) {
        QString droppedText = mimeData->text();
        DraggableButton* sourceButton = qobject_cast<DraggableButton*>(event->source());
        DraggableButton* targetButton = qobject_cast<DraggableButton*>(childAt(event->pos()));

        if (sourceButton && targetButton) {
            QString targetText = targetButton->text();
            targetButton->setText(droppedText);
            sourceButton->setText(targetText);

            // Обновляем структуру keys
            for (auto& row : keys[currentLayout]) {
                auto it1 = std::find(row.begin(), row.end(), droppedText);
                auto it2 = std::find(row.begin(), row.end(), targetText);
                if (it1 != row.end() && it2 != row.end()) {
                    std::iter_swap(it1, it2);
                    break;
                }
            }
        }
        event->acceptProposedAction();
    }
}
