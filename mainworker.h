#ifndef MAINWORKER_H
#define MAINWORKER_H

#include <QObject>
#include <networker.h>

#include "opencv2/highgui/highgui.hpp"

class MainWorker:  public QObject
{
    Q_OBJECT

public:
    MainWorker(NetWorker *networker);
    ~MainWorker();
    bool Initialize();
    void Start();
    void GrabFrame();
    void Stop();
    bool getIsWorking();


private:
    NetWorker *_systemHead;
    cv::VideoCapture* _capture;
    void setIsWorking(bool value);
    bool _initialized;
    bool _close;
    bool _started;

private slots:
    void On_StartWorking();
    void On_StopWorking();
    void On_GrabFrame();
};

#endif // MAINWORKER_H
