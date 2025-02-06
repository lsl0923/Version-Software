#include "OpenCVCameraManager.h"
#include"OpenCVCamera.h"
#include<QDebug>
void OpenCVCameraManager::DetectConnectedCamerasSub()
{

    cameras_.clear();

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
        cameras_.push_back(camera);
        cap.release();
    }
}

