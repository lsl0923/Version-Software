#include "OpenCVCamera.h"
#include <QThread>
#include"QDebug"

OpenCVCamera::OpenCVCamera(int cameraIndex)
    : cameraIndex_(cameraIndex), isConnected_(false), exposure_(10000) {}

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
    DisConnectCamera();
}

void OpenCVCamera::SnapSub()
{
    if (cap_.isOpened())
    {
        SetExposure(exposure_);
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
    if (cap_.isOpened())
    {
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

std::vector<std::shared_ptr<CameraBase>> OpenCVCamera::DetectConnectedCamerasSub()
{
    std::vector<std::shared_ptr<CameraBase>> cameras;
    for (int i = 0; i < 1; ++i)
    {
        cv::VideoCapture cap;
        cap.open(i);

        if (!cap.isOpened()) // 如果无法打开摄像头，跳过
        {
            continue;
        }

        qDebug() << "摄像头 " << i << " 成功打开";
        auto camera = std::make_shared<OpenCVCamera>(i);
        CameraID id("usb"+ std::to_string(i),CameraType::OpenCV);
        camera->SetCameraID(id);
        cameras.push_back(camera);
        cap.release();
    }
    return cameras;
}
