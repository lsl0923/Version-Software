#include "CameraTool.h"
#include <QDebug>
#include <QPluginLoader>
#include <memory>  // 引入智能指针的头文件
#include <opencv2/opencv.hpp>
CameraTool::CameraTool(const CameraType type)
{
    if(type == CameraType::OpenCV)
    {
        cameraManager_ = std::make_shared<CameraManager>(type);
    }
    /*
    QPluginLoader loader(path);
    QObject *plugin = loader.instance();
    if (!plugin)
    {
        qDebug() << "Failed to load plugin:" << loader.errorString();
        return;  // 插件加载失败时，返回
    }

    // 尝试转换插件为 CameraManager 类型
    CameraManager* cur = qobject_cast<CameraManager*>(plugin);
    cameraManager_ = std::shared_ptr<CameraManager>(cur);

    if (!cameraManager_)
    {
        qDebug() << "Failed to cast plugin to CameraManager";
        return;
    }

    // 加载插件并检测连接的相机
    cameraManager_->DetectConnectedCameras();
*/
    setName();  // 设置工具的名称
}

CameraTool::~CameraTool()
{
    // 这里可以做一些销毁或者清理操作，尽管 shared_ptr 会自动管理内存
}

void CameraTool::setName()
{
    name_ = "CameraTool";
}

std::vector<std::string> CameraTool::getInputsList()
{
    std::vector<std::string> vec;
    for(int i = 0;i<cameraManager_->GetCameraList().size();++i)
    {
        CameraID id = cameraManager_->GetCameraList()[i]->GetCameraID();
        vec.push_back(id.id);
    }
    return vec;
}

std::vector<std::string> CameraTool::getOutputsList()
{
    return { "image" }; // 输出为图片
}

std::string CameraTool::getInputType(const std::string& inputName)
{
    return ""; // 没有输入
}

std::string CameraTool::getOutputType(const std::string& outputName)
{
    return outputName == "image" ? "cv::Mat" : ""; // 输出类型为 OpenCV 图像
}

bool CameraTool::validateInputs()
{
    return true;
}
void CameraTool::setCameraFromManager(int cameraIndex)
{
    if (cameraManager_)
    {
        // 从 cameraManager_ 获取相机列表
        auto cameras = cameraManager_->GetCameraList();

        if (cameraIndex >= 0 && cameraIndex < cameras.size())
        {
            // 选择并设置 camera_
            camera_ = cameras[cameraIndex];
            qDebug() << "Camera set successfully.";
        } else
        {
            qDebug() << "Invalid camera index.";
        }
    } else
    {
        qDebug() << "CameraManager is not initialized.";
    }
}
void CameraTool::setCameraFromManager(CameraID cameraID)
{
    auto cameras = cameraManager_->GetCameraList();
    for (auto& camera : cameras)
    {
        if(camera->GetCameraID() == cameraID)
        {
            camera_ = camera;
            return;
        }
    }

}
int CameraTool::runSub()
{
    if(cameraManager_->GetCameraList().size() == 0)
    {
        return -1;
    }
    IOConfig config = this->getInputs();
    // 检查并设置默认值
    std::any exposureValue = config.getDataValue("Exposure");
    int exposure = 50;  // 默认曝光值

    // 如果 exposure 参数存在且类型匹配
    if (exposureValue.has_value() && exposureValue.type() == typeid(int))
    {
        exposure = std::any_cast<int>(exposureValue);
    }
    // 获取其他参数
    std::any snapValue = config.getDataValue("snap");
    bool snapMode = true;  // 默认 snap 设置
    std::any bs = config.getDataValue("snap");
    if (bs.has_value() && bs.type() == typeid(bool))
    {
        snapMode = std::any_cast<bool>(bs);
    }


    // 读取参数
    std::any cameraValue = config.getDataValue("Camera");
    std::string selectedCamera = "usb0";  // 默认相机

    if (cameraValue.has_value() && cameraValue.type() == typeid(std::string))
    {
        selectedCamera = std::any_cast<std::string>(cameraValue);
    }

    qDebug() << "Camera: " << QString::fromStdString(selectedCamera)
             << ", Exposure: " << exposure
             << ", Snap Mode: " << snapMode;
    CameraID id;
    id.id = selectedCamera;
    id.type = CameraType::OpenCV;
    setCameraFromManager(id);
    camera_->SetExposure(exposure);
    camera_->ConnectCamera();
    if (snapMode)
    {
        camera_->Snap();
        cv::Mat image = camera_->GetImage();

        if (image.empty()) {
            qDebug() << "Captured empty frame!";
            return -1;
        }
        outputs_.addData("image", image);
    }
    else
    {
        camera_->Snap();
        cv::Mat image = camera_->GetImage();

        if (image.empty()) {
            qDebug() << "Captured empty frame!";
            return -1;
        }
        outputs_.addData("image", image);
    }
    camera_->DisConnectCamera();

    return 1;
}


