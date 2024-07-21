#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCryptographicHash>
#include <QString>
#include <QMap>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void checkIntegrity();
    void displayKnownHashes();
    void displayCurrentHashes();
    void modifyData();
    void modifyText();
    QString calculateHash(const char* data, size_t size);

private:
    Ui::MainWindow *ui;
    QTextEdit *logOutput;
    QPushButton *checkButton;
    QPushButton *displayButton;
    QPushButton *displayCurrentButton;
    QPushButton *modifyDataButton;
    QPushButton *modifyTextButton;
    QVBoxLayout *layout;
    QTimer *integrityCheckTimer;

    QMap<QString, QString> knownHashes;

    static int dataVariable;
    static int textVariable;
};

#endif // MAINWINDOW_H
