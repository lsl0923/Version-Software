#ifndef OPENCVCAMERA_H
#define OPENCVCAMERA_H
#include <QThread>
#include "CameraBase.h"
#include <opencv2/opencv.hpp>

class CameraGrabThread;
class OpenCVCamera : public CameraBase
{
private:
    cv::VideoCapture cap_;
    int cameraIndex_;
    bool isConnected_;
    double exposure_;
    CameraGrabThread* grabThread_;

protected:
    void ConnectCameraSub() override;
    void DisConnectCameraSub() override;
    void StartGrabSub() override;
    void StopGrabSub() override;
    void SnapSub() override;
    bool IsConnectedSub() override;
    void SetExposureSub(double nExposure) override;
    void SetExternTrigerSub(bool bExternTriger) override;
    void GetExposureRangeSub(double& dbMin, double& dbMax) override;
    double GetExposureSub() override;

public:
    OpenCVCamera(int cameraIndex = 0);
};

class CameraGrabThread : public QThread
{
    Q_OBJECT
public:
    CameraGrabThread(OpenCVCamera* camera) : camera_(camera) {}

protected:
    void run() override
    {
        camera_->ConnectCamera();
        while (camera_->IsConnected())
        {
            camera_->Snap();
            emit frameReady(camera_->GetImage());
            msleep(5);  // 控制抓取频率
        }
    }
signals:
    void frameReady(cv::Mat frame);  // 用于更新主线程的图像信号
private:
    OpenCVCamera* camera_;
};
#endif // OPENCVCAMERA_H
