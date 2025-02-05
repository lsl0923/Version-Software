#include "CameraManager.h"
#include <algorithm>


void CameraManager::ConnectAll() {
    for (auto& camera : cameras_) {
        camera->ConnectCamera();
    }
}

void CameraManager::DisconnectAll() {
    for (auto& camera : cameras_) {
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

void CameraManager::AddCamera(std::shared_ptr<CameraBase> camera) {
    cameras_.push_back(camera);
}

void CameraManager::RemoveCamera(std::shared_ptr<CameraBase> camera) {
    cameras_.erase(std::remove(cameras_.begin(), cameras_.end(), camera), cameras_.end());
}

std::shared_ptr<CameraBase> CameraManager::GetCamera(int index) {
    if (index >= 0 && index < static_cast<int>(cameras_.size())) {
        return cameras_[index];
    }
    return nullptr;
}

std::vector<std::shared_ptr<CameraBase>> CameraManager::GetCameraList() const {
    return cameras_;
}
