#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "opencv2/imgproc/imgproc.hpp"
#include "networker.h"
#include "mainworker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_stopButton_clicked();
    void on_startServer_clicked();

    void on_sendButton_clicked();

    void on_mainWorkerButton_clicked();

private:
    Ui::MainWindow *ui;
    static QImage IplImage2QImage(const IplImage *iplImage);
    NetWorker *_server;
    MainWorker *_mainWorker;
};

#endif // MAINWINDOW_H
