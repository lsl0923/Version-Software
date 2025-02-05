#include "CameraTool.h"
#include <QDebug>
#include <QPluginLoader>
#include <memory>  // 引入智能指针的头文件
#include <opencv2/opencv.hpp>

CameraTool::CameraTool(const QString path)
{
    QPluginLoader loader(path);
    QObject *plugin = loader.instance();
    if (!plugin) {
        qDebug() << "Failed to load plugin:" << loader.errorString();
        return;  // 插件加载失败时，返回
    }

    // 尝试转换插件为 CameraManager 类型
    CameraManager* cur = qobject_cast<CameraManager*>(plugin);
    cameraManager_ = std::shared_ptr<CameraManager>(cur);

    if (!cameraManager_) {
        qDebug() << "Failed to cast plugin to CameraManager";
        return;
    }

    // 加载插件并检测连接的相机
    cameraManager_->DetectConnectedCameras();
    setName();  // 设置工具的名称
}

CameraTool::~CameraTool() {
    // 这里可以做一些销毁或者清理操作，尽管 shared_ptr 会自动管理内存
}

void CameraTool::setName() {
    name_ = "Camera Capture";
}

std::vector<std::string> CameraTool::getInputsList() {
    return {}; // 相机工具没有输入
}

std::vector<std::string> CameraTool::getOutputsList() {
    return { "image" }; // 输出为图片
}

std::string CameraTool::getInputType(const std::string& inputName) {
    return ""; // 没有输入
}

std::string CameraTool::getOutputType(const std::string& outputName) {
    return outputName == "image" ? "cv::Mat" : ""; // 输出类型为 OpenCV 图像
}

bool CameraTool::validateInputs() {
    return true; // 由于没有输入，始终返回 true
}
void CameraTool::setCameraFromManager(int cameraIndex)
{
    if (cameraManager_) {
        // 从 cameraManager_ 获取相机列表
        auto cameras = cameraManager_->GetCameraList();

        if (cameraIndex >= 0 && cameraIndex < cameras.size()) {
            // 选择并设置 camera_
            camera_ = cameras[cameraIndex];
            qDebug() << "Camera set successfully.";
        } else {
            qDebug() << "Invalid camera index.";
        }
    } else {
        qDebug() << "CameraManager is not initialized.";
    }
}
int CameraTool::runSub()
{
    setCameraFromManager(0);
    camera_->ConnectCamera();
    if (!camera_ || !camera_->IsConnected())
    {
        qDebug() << "Camera is not connected!";
        return -1;  // 相机未连接，返回错误码
    }

    camera_->Snap();  // 捕获图像
    cv::Mat image = camera_->GetImage();  // 获取图像

    if (image.empty()) {
        qDebug() << "Captured empty frame!";
        return -1;  // 图像为空，返回错误码
    }
     qDebug() << "Captured OKkkkkkkkkkkkkkk!";
    outputs_.addData("image", image);  // 将图像数据添加到输出
     camera_->DisConnectCamera();
    return 1;  // 成功
}
