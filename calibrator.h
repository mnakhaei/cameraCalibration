#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <QObject>
#include <QThread>
#include <settings.h>



class calibrator : public QThread
{
    Q_OBJECT
public:
    explicit calibrator(QObject *parent = nullptr);
    void setSsttings(const Settings &value);

signals:
    void newImage(const cv::Mat&);
    void percentage(int p);

public slots:
    void calibrationStart();


    // QThread interface
protected:
    virtual void run();

    enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };
    Settings s;
    bool runCalibrationAndSave(Settings& s, Size imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                              vector<vector<Point2f> > imagePoints);

    static bool runCalibration( Settings& s, Size& imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                               vector<vector<Point2f> > imagePoints, vector<Mat>& rvecs, vector<Mat>& tvecs,
                               vector<float>& reprojErrs,  double& totalAvgErr);
    static double computeReprojectionErrors( const vector<vector<Point3f> >& objectPoints,
                                            const vector<vector<Point2f> >& imagePoints,
                                            const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                                            const Mat& cameraMatrix , const Mat& distCoeffs,
                                            vector<float>& perViewErrors, bool fisheye);

    static void calcBoardCornerPositions(Size boardSize, float squareSize, vector<Point3f>& corners,
                                        Settings::Pattern patternType /*= Settings::CHESSBOARD*/);

    static void saveCameraParams( Settings& s, Size& imageSize, Mat& cameraMatrix, Mat& distCoeffs,
                                 const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                                 const vector<float>& reprojErrs, const vector<vector<Point2f> >& imagePoints,
                                 double totalAvgErr );

    bool gg;
};

#endif // CALIBRATOR_H
