#include "OpenCVCamera.h"
#include <QThread>


OpenCVCamera::OpenCVCamera(int cameraIndex)
    : cameraIndex_(cameraIndex), isConnected_(false), exposure_(0) {}

void OpenCVCamera::ConnectCameraSub()
{
    cap_.open(cameraIndex_);
    isConnected_ = cap_.isOpened();
}

void OpenCVCamera::DisConnectCameraSub()
{
    if (cap_.isOpened())
    {
        cap_.release();
    }
    isConnected_ = false;
}

void OpenCVCamera::StartGrabSub()
{
    CameraGrabThread* grabThread = new CameraGrabThread(this);
    grabThread->start();
}

void OpenCVCamera::StopGrabSub()
{
    // OpenCV VideoCapture 没有显式停止抓取的方法
    DisConnectCamera();
}

void OpenCVCamera::SnapSub()
{
    ConnectCameraSub();
    if (cap_.isOpened())
    {
        cap_ >> imgCur_;
        if (!imgCur_.empty())
        {
            OnImageCallBack(imgCur_);
        }
    }
}

bool OpenCVCamera::IsConnectedSub()
{
    return isConnected_;
}

void OpenCVCamera::SetExposureSub(double nExposure)
{
    exposure_ = nExposure;
    if (cap_.isOpened()) {
        cap_.set(cv::CAP_PROP_EXPOSURE, exposure_);
    }
}

void OpenCVCamera::SetExternTrigerSub(bool bExternTriger)
{
    // OpenCV VideoCapture 不支持外部触发，这里可留空
}

void OpenCVCamera::GetExposureRangeSub(double& dbMin, double& dbMax)
{
    dbMin = -10; // OpenCV 并不提供标准的曝光范围，需要根据具体相机设置
    dbMax = 10;
}

double OpenCVCamera::GetExposureSub()
{
    return exposure_;
}


