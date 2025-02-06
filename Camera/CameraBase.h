#ifndef CAMERABASE_H
#define CAMERABASE_H

#include "Camera_global.h"
#include<opencv2/opencv.hpp>
#include<QObject>


enum class CameraStatus
{
    Disconnected,
    Connected,
    Grabbing,
    Snap
};

enum class CameraType
{
    OpenCV,
    Other
};

struct CameraID {
    std::string id;  // 相机的唯一标识符（如序列号）
    CameraType type; // 相机类型

    CameraID() : id(""), type(CameraType::OpenCV) {} // 默认是 OpenCV 类型

    CameraID(std::string id, CameraType type) : id(std::move(id)), type(type) {}

    // 重载 == 运算符
    bool operator==(const CameraID& other) const
    {
        return (id == other.id) && (type == other.type);
    }
};

class CAMERA_EXPORT CameraBase :public QObject
{
    Q_OBJECT
protected:

    cv::Mat imgCur_;

protected:
    virtual void ConnectCameraSub() = 0;
    virtual void DisConnectCameraSub() = 0; //连接相机
    virtual void StartGrabSub() = 0;
    virtual void StopGrabSub() = 0;
    virtual void SnapSub() = 0;
    virtual bool IsConnectedSub() = 0;
    virtual  void SetExposureSub(double nExposure)=0;
    virtual void SetExternTrigerSub(bool bExternTriger) = 0;
    virtual void GetExposureRangeSub(double& dbMin, double& dbMax) = 0;
    virtual double GetExposureSub() = 0;
    void OnImageCallBack(cv::Mat img);
signals:
    void newImageReceived(cv::Mat image);
private:
    void OnImageCallBackInnter(cv::Mat img);
    void StartWaitSnap();
    void WaitSnap();

public:
    CameraBase();
public :
    CameraStatus GetStatus(); //获取相机当前状态
    void SetCameraSNum(QString strID); // 相机厂商定义的SN
    CameraID GetCameraID();//相机的唯一ID号
    void SetCameraID(CameraID id);//相机的唯一ID号
    void SetStatus(CameraStatus status);//相机当前状态 连续、单帧或者空闲
    void ConnectCamera();//连接相机
    void DisConnectCamera(); //断开连接
    bool IsConnected();  //是否连接
    void SetExposure(double nExposure); //设置曝光

    void GetExposureRange(double& dbMin, double& dbMax); //获取曝光范围
    double GetExposure(); //获取曝光
    void Snap() ;//单帧采集一张图片
    void StartGrab() ;
    void StopGrab();
    cv::Mat GetImage();


private:
    CameraID cameraID_;
    CameraStatus status_;
};

#endif // CAMERABASE_H
