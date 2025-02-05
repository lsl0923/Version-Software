#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "CameraBase.h"
#include <memory>
#include <vector>
#include <QtPlugin>
#include<QObject>
#define CameraManager_iid "com.example.CameraManager/1.0"
class CAMERA_EXPORT CameraManager : public QObject
{
    Q_OBJECT
protected:
    std::vector<std::shared_ptr<CameraBase>> cameras_; // 相机列表

public:
    CameraManager() = default;
    virtual ~CameraManager() = default;

    virtual void DetectConnectedCameras() = 0; // 纯虚函数，由子类实现
    virtual void ConnectAll();
    virtual void DisconnectAll();
    virtual void SnapAll();

    void AddCamera(std::shared_ptr<CameraBase> camera);
    void RemoveCamera(std::shared_ptr<CameraBase> camera);
    std::shared_ptr<CameraBase> GetCamera(int index);
    std::vector<std::shared_ptr<CameraBase>> GetCameraList() const;
};

Q_DECLARE_INTERFACE(CameraManager, CameraManager_iid)
#endif // CAMERA_MANAGER_H
