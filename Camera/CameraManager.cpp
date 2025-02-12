#include "CameraManager.h"
#include <algorithm>
#include <QPluginLoader>
#include"CameraBase.h"
CameraManager::CameraManager(CameraType type)
{
    if(type == CameraType::OpenCV)
    {
        QString path = "/home/lsl/Code/OpenCVCamera/build/Desktop_Qt_5_15_2_GCC_64bit-Debug/libOpenCVCamera.so";
        QPluginLoader loader(path);
        QObject *plugin = loader.instance();
        if (!plugin)
        {
            qDebug() << "Failed to load plugin:" << loader.errorString();
            return;  // 插件加载失败时，返回
        }
        // 尝试转换插件为 CameraManager 类型
        CameraBase* cur = qobject_cast<CameraBase*>(plugin);
        cameras_ = cur->DetectConnectedCameras();
    }

}

void CameraManager::ConnectAll()
{
    for (auto& camera : cameras_)
    {
        camera->ConnectCamera();
    }
}

void CameraManager::DisconnectAll()
{
    for (auto& camera : cameras_)
    {
        camera->DisConnectCamera();
    }
}

void CameraManager::SnapAll()
{
    for (auto& camera : cameras_)
    {
        camera->Snap();
    }
}

void CameraManager::AddCamera(std::shared_ptr<CameraBase> camera)
{
    cameras_.push_back(camera);
}

void CameraManager::RemoveCamera(std::shared_ptr<CameraBase> camera)
{
    cameras_.erase(std::remove(cameras_.begin(), cameras_.end(), camera), cameras_.end());
}

std::shared_ptr<CameraBase> CameraManager::GetCamera(int index)
{
    if (index >= 0 && index < static_cast<int>(cameras_.size()))
    {
        return cameras_[index];
    }
    return nullptr;
}

std::vector<std::shared_ptr<CameraBase>> CameraManager::GetCameraList() const
{
    return cameras_;
}
/*
void  CameraManager::DetectConnectedCameras()
{
    DetectConnectedCamerasSub();
}
*/
