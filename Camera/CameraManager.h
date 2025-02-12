#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "CameraBase.h"
#include <memory>
#include <vector>
#include <QtPlugin>
#include<QObject>


#define CameraManager_iid "com.CameraManager"
class CAMERA_EXPORT CameraManager : public QObject
{
    Q_OBJECT
protected:
    std::vector<std::shared_ptr<CameraBase>> cameras_; // 相机列表

public:
    CameraManager(CameraType type);
     ~CameraManager() = default;
    //void DetectConnectedCameras();
    //virtual void DetectConnectedCamerasSub() = 0; // 纯虚函数，由子类实现
     void ConnectAll();
     void DisconnectAll();
     void SnapAll();

    void AddCamera(std::shared_ptr<CameraBase> camera);
    void RemoveCamera(std::shared_ptr<CameraBase> camera);
    std::shared_ptr<CameraBase> GetCamera(int index);
    std::vector<std::shared_ptr<CameraBase>> GetCameraList() const;
};

//Q_DECLARE_INTERFACE(CameraManager, CameraManager_iid)
#endif // CAMERA_MANAGER_H
