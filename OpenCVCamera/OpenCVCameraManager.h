#ifndef OPENCV_CAMERA_MANAGER_H
#define OPENCV_CAMERA_MANAGER_H

#include "CameraManager.h"

#include <opencv2/opencv.hpp>
#include <QObject>
#include <QtPlugin>

class CAMERA_EXPORT OpenCVCameraManager : public CameraManager
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID CameraManager_iid)
    Q_INTERFACES(CameraManager)
public:
    OpenCVCameraManager() = default;
    ~OpenCVCameraManager() override = default;

    void DetectConnectedCameras() override;
};

#endif // OPENCV_CAMERA_MANAGER_H
