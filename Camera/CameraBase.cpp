#include "CameraBase.h"
#include<QString>
CameraBase::CameraBase() : status_(CameraStatus::Disconnected) {}

CameraStatus CameraBase::GetStatus()
{
    return status_;
}

void CameraBase::SetCameraSNum(QString strID)
{
    cameraID_.id = strID.toStdString();
}

CameraID CameraBase::GetCameraID()
{
    return cameraID_;
}

void CameraBase::SetStatus(CameraStatus status)
{
    status_ = status;
}

void CameraBase::ConnectCamera()
{
    ConnectCameraSub();
    status_ = CameraStatus::Connected;
}

void CameraBase::DisConnectCamera()
{

    DisConnectCameraSub();
    status_ = CameraStatus::Disconnected;
}

bool CameraBase::IsConnected()
{
    return status_ == CameraStatus::Connected || status_ == CameraStatus::Grabbing;
}

void CameraBase::SetExposure(double nExposure)
{
    SetExposureSub(nExposure);
}

void CameraBase::GetExposureRange(double& dbMin, double& dbMax)
{
    GetExposureRangeSub(dbMin, dbMax);
}

double CameraBase::GetExposure()
{
    return GetExposureSub();
}

void CameraBase::Snap()
{
    SnapSub();
}

void CameraBase::StartGrab()
{
    StartGrabSub();
    status_ = CameraStatus::Grabbing;
}

void CameraBase::StopGrab() {
    StopGrabSub();
    status_ = CameraStatus::Connected;
}

cv::Mat CameraBase::GetImage()
{
    return imgCur_;
}

void CameraBase::OnImageCallBack(cv::Mat img)
{
    imgCur_ = img;
    emit newImageReceived(imgCur_);
    OnImageCallBackInnter(imgCur_);
}

void CameraBase::OnImageCallBackInnter(cv::Mat img)
{
    if(!img.empty())
    {
       // cv::imshow("1",imgCur_);
        //cv::waitKey(10);
    }
}

void CameraBase::StartWaitSnap()
{

    // 实现等待快照逻辑
}

void CameraBase::WaitSnap()
{
    // 实现等待快照逻辑
}


