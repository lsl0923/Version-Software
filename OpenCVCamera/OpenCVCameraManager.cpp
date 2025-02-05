#include "OpenCVCameraManager.h"
#include"OpenCVCamera.h"
#include<QDebug>
void OpenCVCameraManager::DetectConnectedCameras()
{
    qDebug() << "1";  // 进入函数
    cameras_.clear();

    for (int i = 0; i < 1; ++i)
    {
        qDebug() << "尝试打开摄像头: " << i;
        cv::VideoCapture cap;
        cap.open(i);

        if (!cap.isOpened()) // 如果无法打开摄像头，跳过
        {
            qDebug() << "摄像头 " << i << " 无法打开，跳过";
            continue;
        }

        qDebug() << "摄像头 " << i << " 成功打开";
        auto camera = std::make_shared<OpenCVCamera>(i);
        cameras_.push_back(camera);
        cap.release();
    }

    qDebug() << "1"; // 退出函数
}

