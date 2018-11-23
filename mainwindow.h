#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <settings.h>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <QCameraInfo>
#include <QtCore>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QLabel>

#include <calibrator.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Settings settings;
    string videoFileAddress;
    string outPutFile;
    QFileDialog *outPutFD,*videoFD;
    calibrator *c;
public slots:
    void setOutPutFile();
    void setVideoFile();
    void setImg(const cv::Mat &mat);
    void fillSettings();


};

#endif // MAINWINDOW_H
