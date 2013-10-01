#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "networker.h"

#include <QtTest/QTest>
#include "cv.h"
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "mainworker.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool _close = false;

void MainWindow::on_pushButton_clicked()
{
    ui->infoBox->clear();
    QString fn_haar = "haarcascade_frontalface_default.xml";
    int deviceId = 0;

    // These vectors hold the images and corresponding labels:
    vector<Mat> images;
    vector<int> labels;

    CascadeClassifier haar_cascade;
    //Application path
    QString folder = QCoreApplication::applicationDirPath();
    bool cascade_loaded = haar_cascade.load(folder.toStdString() + "/" + fn_haar.toStdString());

    //Is cascade loaded
    if(!cascade_loaded)
    {
        ui->infoBox->append("Can't load " + folder + "/" + fn_haar);
    }

    // Get a handle to the Video device:
    VideoCapture cap(deviceId);

    //won't qork without wainit on my pc
    QTest::qSleep(700);
    // Check if we can use this device at all:
    if(!cap.isOpened()) {
        ui->infoBox->append("Capture Device ID " + QString::number(deviceId) + "cannot be opened.\r\n" );
        //cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
        return;
    }


    // Holds the current frame from the Video device:
        Mat frame;
        for(;;) {
            cap >> frame;

            IplImage imm = frame;
            IplImage *im = &imm;

            ui->infoBox->clear();
            //ui->infoBox->append("Width: " + QString::number(im->width) + "\r\nHeight: " + QString::number(im->height));
            IplImage *nim = cvCreateImage(cvSize(320,240), im->depth, im->nChannels);
            cvResize(im, nim);
            frame = Mat(nim);

            ui->infoBox->append("\r\nWidth: " + QString::number(nim->width) + "\r\nHeight: " + QString::number(nim->height));

            // Clone the current frame:
            Mat original = frame.clone();
            // Convert the current frame to grayscale:
            Mat gray;
            cvtColor(original, gray, COLOR_BGR2GRAY);
            // Find the faces in the frame:
            vector< Rect_<int> > faces;

            haar_cascade.detectMultiScale(gray, faces);
            // At this point you have the position of the faces in
            // faces. Now we'll get the faces, make a prediction and
            // annotate it in the video. Cool or what?
            for(uint i = 0; i < faces.size(); i++) {
                // Process face by face:
                Rect face_i = faces[i];
                // Crop the face from the image. So simple with OpenCV C++:
                Mat face = gray(face_i);
                // Resizing the face is necessary for Eigenfaces and Fisherfaces. You can easily
                // verify this, by reading through the face recognition tutorial coming with OpenCV.
                // Resizing IS NOT NEEDED for Local Binary Patterns Histograms, so preparing the
                // input data really depends on the algorithm used.
                //
                // I strongly encourage you to play around with the algorithms. See which work best
                // in your scenario, LBPH should always be a contender for robust face recognition.
                //
                // Since I am showing the Fisherfaces algorithm here, I also show how to resize the
                // face you have just found:

                /*
                Mat face_resized;
                cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
                */

                // Now perform the prediction, see how easy that is:

                /*
                int prediction = model->predict(face_resized);
                */

                // And finally write all we've found out to the original image!
                // First of all draw a green rectangle around the detected face:
                rectangle(original, face_i, CV_RGB(0, 255,0), 1);

                // Create the text we will annotate the box with:

                /*
                string box_text = format("Prediction = %d", prediction);
                */

                // Calculate the position for annotated text (make sure we don't
                // put illegal values in there):
                int pos_x = std::max(face_i.tl().x - 10, 0);
                int pos_y = std::max(face_i.tl().y - 10, 0);
                // And now put it into the image:

                /*
                putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
                */

                putText(original, "Face", Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);

            }
            // Show the result:
            IplImage *m = new IplImage(original);
            QImage qim = MainWindow::IplImage2QImage(m);

            //imshow("face_recognizer", original);
            ui->label->setPixmap(QPixmap::fromImage(qim));
            // And display it:
            waitKey(20);
            //char key = (char) waitKey(20);
            // Exit this loop on escape:
            //if(key == 27)
            if(_close)
            {
                _close = false;
                break;
            }
        }
        frame.release();
        cap.release();
        //destroyWindow("face_recognizer");

}

QImage MainWindow::IplImage2QImage(const IplImage *iplImage)
{
       int height = iplImage->height;
       int width = iplImage->width;

       const uchar *qImageBuffer =(const uchar*)iplImage->imageData;
       QImage img(qImageBuffer, width, height, QImage::Format_RGB888);
       return img.rgbSwapped();
}

void MainWindow::on_stopButton_clicked()
{
    _close = true;
}

void MainWindow::on_startServer_clicked()
{
    //if(_server == NULL)
        _server =  new NetWorker();
    if(_mainWorker == NULL)
        _mainWorker = new MainWorker(_server);
    _mainWorker->Initialize();

    ui->infoBox->clear();
    _server->StartListen(2244);
    if(_server->IsListening())
        ui->infoBox->append("Listening");
    else
        ui->infoBox->append("Listen error");
}

void MainWindow::on_sendButton_clicked()
{
    QString str = "Hi";
    QByteArray data;
    data.append(str);
    _server->SendData(data);
}

void MainWindow::on_mainWorkerButton_clicked()
{

    MainWorker* w = new MainWorker(NULL);
    w->Initialize();
    w->Start();
    delete w;
}
