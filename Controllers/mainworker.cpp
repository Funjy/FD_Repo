#include "mainworker.h"
#include <networker.h>
#include <clientserverinterface.h>

#include <QtTest/QTest>
#include "cv.h"
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

bool _isWorking;
//bool _close;

MainWorker::MainWorker(NetWorker *networker)
{
    _systemHead = networker;
    _isWorking = false;
    _initialized = false;
    _close = false;
}

MainWorker::~MainWorker()
{
    if(_systemHead == NULL)
        return;
    //_systemHead->~NetWorker();
    delete _systemHead;
    _systemHead = NULL;
}
bool MainWorker::getIsWorking()
{
    return _isWorking;
}

void MainWorker::setIsWorking(bool value)
{
    _isWorking = value;
}


bool MainWorker::Initialize()
{
    if(_initialized)
        return true;
    connect(_systemHead, SIGNAL(StartWorkingEvent()), this, SLOT(On_StartWorking()));
    connect(_systemHead, SIGNAL(StopWorkingEvent()), this, SLOT(On_StopWorking()));
    connect(_systemHead, SIGNAL(GrabFrameEvent()), this, SLOT(On_GrabFrame()));
    /*
    QString fn_haar = "haarcascade_frontalface_default.xml";
    CascadeClassifier haar_cascade;
    //Application path
    QString folder = QCoreApplication::applicationDirPath();
    bool cascade_loaded = haar_cascade.load(folder.toStdString() + "/" + fn_haar.toStdString());

    //Is cascade loaded
    if(!cascade_loaded)
    {
        //Cascade not loaded
        return false;
    }
    */

    /*
    int deviceId = 0;

    // Get a handle to the Video device:
    //VideoCapture cap(deviceId);
    _capture = new VideoCapture(deviceId);

    //won't qork without wainit on my pc
    QTest::qSleep(700);
    // Check if we can use this device at all:
    if(!_capture->isOpened()) {
        //Can't use device
        return false;
    }
    */
    _initialized = true;
    return true;

}

void MainWorker::Start()
{    
    if(!_initialized)
    {
        _systemHead->SendMessage(HeadCommands::NOT_INITIALIZED);
        return;
    }
    int deviceId = 0;
    _capture = new VideoCapture(deviceId);
    if(!_capture->isOpened()) {
        _systemHead->SendMessage("can't open capture");
        return;
    }
    _started = true;
    _systemHead->SendMessage(HeadCommands::START_SUCCESS_RESPONSE);
    /*namedWindow("onew");
    // Holds the current frame from the Video device:    
        Mat frame;
        //for(;;) {
            //_capture >> frame;
        bool read = _capture->read(frame);
        if(!read)
        {
            _systemHead->SendMessage("can't read");
            return;
        }


            // Clone the current frame:
            Mat original = frame.clone();
            // Convert the current frame to grayscale:
            Mat gray;
            cvtColor(original, gray, COLOR_BGR2GRAY);

            Mat mini;

            try
            {
                resize(gray, mini, Size(320,240), 1.0, 1.0, INTER_CUBIC);
                //resize(original, mini, Size(320,240), 1.0, 1.0);
            }
            catch(Exception& ex)
            {
                _systemHead->SendMessage(QString(ex.what()));
                return;
            }

            vector<int> params;
//            params.push_back(CV_IMWRITE_PNG_COMPRESSION);
//            params.push_back(3);
//            params.push_back(0);
            params.push_back(CV_IMWRITE_JPEG_QUALITY);
            params.push_back(85);
            params.push_back(0);
            vector<uchar> buf;
            //bool encoded = imencode(".PNG", gray, buf, params);
            bool encoded;
            try
            {
                //encoded = imencode(".PNG", mini, buf, params);
                encoded = imencode(".JPG", mini, buf, params);
            }
            catch(Exception& ex)
            {
                _systemHead->SendMessage(QString(ex.what()));
                return;
            }

            if(encoded)
            {
                QByteArray ba2send;
                uchar r;
                foreach (r, buf) {
                    ba2send.append((char)r);
                }

                //-----------------------------------------------
                //uchar* bfrom = (unsigned char*)(ba2send->data());
                //std::vector<uchar> buf1;
                //buf1.insert(buf1.end(), bfrom, bfrom + sizeof(uchar)*ba2send->length());

                //Mat newim = imdecode(buf1, 0);

                //_systemHead->SendData(ba2send);
                _systemHead->SendFrame(ba2send);
                Mat newim = imdecode(buf, CV_LOAD_IMAGE_ANYCOLOR);
                int pos_x = 20;
                int pos_y = 20;
                string st = QString::number(buf.size()).toStdString();
                putText(newim, st, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
                imshow("onew", newim);
            }
            else
            {
                imshow("onew", original);
            }*/

            //Send here
            //_systemHead->SendData(bytes);

            /*
            char key = (char) waitKey(20);
            // Exit this loop on escape:
            if(key == 27)
            {
                //gray.release();
                //original.release();
                return;
            }
            */

            /*
            //Check to close
            if(_close)
            {
                _close = false;
                break;
            }*/

        //}//for(;;) {
        //gray.release();
        //original.release();
        //frame.release();
        //_capture->release();
}

void MainWorker::GrabFrame()
{
    if(_capture == NULL || !_capture->isOpened())
    {
        _systemHead->SendMessage(HeadCommands::CAPTURE_CLOSED);
        return;
    }

    Mat frame;
    //for(;;) {
        //_capture >> frame;
    bool read = _capture->read(frame);
    if(!read)
    {
        _systemHead->SendMessage("can't read");
        return;
    }
        // Clone the current frame:
        Mat original = frame.clone();
        // Convert the current frame to grayscale:
        Mat gray;
        cvtColor(original, gray, COLOR_BGR2GRAY);

        Mat mini;
        resize(gray, mini, Size(320,240), 1.0, 1.0, INTER_CUBIC);
        //resize(original, mini, Size(320,240), 1.0, 1.0);

        vector<int> params;
//            params.push_back(CV_IMWRITE_PNG_COMPRESSION);
//            params.push_back(3);
//            params.push_back(0);
        params.push_back(CV_IMWRITE_JPEG_QUALITY);
        params.push_back(85);
        params.push_back(0);
        vector<uchar> buf;
        //bool encoded = imencode(".PNG", gray, buf, params);
        bool encoded;
        //encoded = imencode(".PNG", mini, buf, params);
        encoded = imencode(".JPG", mini, buf, params);

        if(encoded)
        {
            QByteArray ba2send;
            uchar r;
            foreach (r, buf) {
                ba2send.append((char)r);
            }

            //-----------------------------------------------
            //uchar* bfrom = (unsigned char*)(ba2send->data());
            //std::vector<uchar> buf1;
            //buf1.insert(buf1.end(), bfrom, bfrom + sizeof(uchar)*ba2send->length());

            //Mat newim = imdecode(buf1, 0);

            //_systemHead->SendData(ba2send);
            _systemHead->SendFrame(ba2send);
            Mat newim = imdecode(buf, CV_LOAD_IMAGE_ANYCOLOR);
            int pos_x = 20;
            int pos_y = 20;
            string st = QString::number(buf.size()).toStdString();
            putText(newim, st, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
            imshow("onew", newim);
        }
        else
        {
            imshow("onew", original);
        }
}



void MainWorker::Stop()
{
    if(!_started)
        return;
    _capture->release();
    delete _capture;
    _capture = NULL;
    _started = false;
    _systemHead->SendMessage(HeadCommands::STOP_SUCCESS_RESPONSE);
    //_initialized = false;
}

void MainWorker::On_StartWorking()
{
    Start();
}

void MainWorker::On_StopWorking()
{    
    Stop();
}

void MainWorker::On_GrabFrame()
{
    GrabFrame();
}
