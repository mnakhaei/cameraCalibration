#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<cv::Mat>("cv::Mat");


    outPutFD=new QFileDialog(this);
    outPutFD->setAcceptMode(QFileDialog::AcceptMode::AcceptSave);

    videoFD=new QFileDialog(this);
    videoFD->setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);


    auto cl = QCameraInfo::availableCameras();
    for(const auto&i:cl)
    {
        ui->comboBox_3->addItem(i.deviceName());
    }

    c = new calibrator(this);


    connect(ui->pushButton_3,SIGNAL(clicked()),this->outPutFD,SLOT(open()));
    connect(this->outPutFD,SIGNAL(accepted()),this,SLOT(setOutPutFile()));
    connect(ui->pushButton,SIGNAL(clicked()),this->videoFD,SLOT(open()));
    connect(this->videoFD,SIGNAL(accepted()),this,SLOT(setVideoFile()));
    connect(this->ui->pushButton_2,SIGNAL(clicked()),this,SLOT(fillSettings()));
    connect(this->c,SIGNAL(newImage(const cv::Mat&)),this,SLOT(setImg(const cv::Mat &)),Qt::ConnectionType::QueuedConnection);
    connect(this->c,SIGNAL(percentage(int)),this->ui->progressBar,SLOT(setValue(int)));
    connect(ui->pushButton_4,SIGNAL(clicked()),this->c,SLOT(calibrationStart()));



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillSettings()
{
    settings.goodInput=true;
    settings.boardSize.width=ui->spinBox->value();
    settings.boardSize.height=ui->spinBox_2->value();
    settings.squareSize=ui->spinBox_3->value();
    settings.aspectRatio=1;
    settings.writeExtrinsics=1;
    settings.writePoints=true;
    settings.outputFileName=outPutFile;
    settings.calibZeroTangentDist=1;
    settings.calibFixPrincipalPoint=1;
    settings.useFisheye=false;
    settings.flipVertical=false;
    settings.showUndistorsed=true;
    settings.fixK1=0;
    settings.fixK2=0;
    settings.fixK3=0;
    settings.fixK4=1;
    settings.fixK5=1;


    if (ui->comboBox->currentText()=="CHESSBOARD")
        settings.calibrationPattern=Settings::Pattern::CHESSBOARD;
    if (ui->comboBox->currentText()=="CIRCLES_GRID")
        settings.calibrationPattern=Settings::Pattern::CIRCLES_GRID;
    if (ui->comboBox->currentText()=="ASYMMETRIC_CIRCLES_GRID")
        settings.calibrationPattern=Settings::Pattern::ASYMMETRIC_CIRCLES_GRID;

    settings.nrFrames=ui->spinBox_5->value();
    if (ui->comboBox_2->currentText()=="video file")
    {
        settings.inputType=Settings::InputType::VIDEO_FILE;
        if(!settings.inputCapture.open(videoFileAddress))
        {
            qDebug() << QString::fromStdString(videoFileAddress);
            ui->statusBar->showMessage("can't open video file");
            settings.goodInput=false;
        }
    }
    if (ui->comboBox_2->currentText()=="camera")
    {
        settings.inputType=Settings::InputType::CAMERA;
        if(!settings.inputCapture.open(ui->comboBox_3->currentText().toStdString()))
        {
            ui->statusBar->showMessage("can't open camera",10);
            settings.goodInput=false;
        }
        settings.delay=ui->spinBox_4->value();
    }
    settings.flag=0;
    if(settings.calibFixPrincipalPoint) settings.flag |= CALIB_FIX_PRINCIPAL_POINT;
    if(settings.calibZeroTangentDist)   settings.flag |= CALIB_ZERO_TANGENT_DIST;
    if(static_cast<bool>( settings.aspectRatio))
                                        settings.flag |= CALIB_FIX_ASPECT_RATIO;
    if(settings.fixK1)                  settings.flag |= CALIB_FIX_K1;
    if(settings.fixK2)                  settings.flag |= CALIB_FIX_K2;
    if(settings.fixK3)                  settings.flag |= CALIB_FIX_K3;
    if(settings.fixK4)                  settings.flag |= CALIB_FIX_K4;
    if(settings.fixK5)                  settings.flag |= CALIB_FIX_K5;

    if (settings.useFisheye) {
        // the fisheye model has its own enum, so overwrite the flags
        settings.flag = fisheye::CALIB_FIX_SKEW | fisheye::CALIB_RECOMPUTE_EXTRINSIC;
        if(settings.fixK1)                   settings.flag |= fisheye::CALIB_FIX_K1;
        if(settings.fixK2)                   settings.flag |= fisheye::CALIB_FIX_K2;
        if(settings.fixK3)                   settings.flag |= fisheye::CALIB_FIX_K3;
        if(settings.fixK4)                   settings.flag |= fisheye::CALIB_FIX_K4;
        if (settings.calibFixPrincipalPoint) settings.flag |= fisheye::CALIB_FIX_PRINCIPAL_POINT;
    }

    settings.atImageList = 0;

    qDebug() << "start mishe alan";

    c->setSsttings(settings);
    c->start();


}

void MainWindow::setOutPutFile()
{
    ui->label_11->setText( outPutFD->selectedFiles()[0].split('/').at(outPutFD->selectedFiles()[0].split('/').size()-1));
    outPutFile=outPutFD->selectedFiles().at(0).toStdString();
}

void MainWindow::setVideoFile()
{
    ui->label_8->setText( videoFD->selectedFiles()[0].split('/').at(videoFD->selectedFiles()[0].split('/').size()-1));
    videoFileAddress=videoFD->selectedFiles().at(0).toStdString();
}

void MainWindow::setImg(const Mat &mat)
{
    Mat img = mat.clone();
    if (img.type() == 16)
       cv::cvtColor(img,img,CV_BGR2RGB);
    cv::resize(img, img, Size(), 0.5, 0.5);
    auto p = QPixmap::fromImage(QImage(static_cast<unsigned char*>( img.data), img.cols, img.rows, QImage::Format_RGB888));
    this->ui->label_2->setPixmap(p);
}
