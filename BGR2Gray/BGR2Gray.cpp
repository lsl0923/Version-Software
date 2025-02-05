
#include <QDebug>
#include <QImage>
#include <QColor>
#include<QObject>
#include <chrono>
#include"ToolsBase.h"
#include <opencv2/opencv.hpp>
#include "BGR2Gray.h"

// 构造函数
BGR2Gray::BGR2Gray() :  ToolsBase()
{
    // 初始化或配置任何所需的成员变量
    //std::shared_ptr<OutPutsBase> out = std::make_shared<Outputs>();
    //outputs = out;
    setName();
}

// 析构函数
BGR2Gray::~BGR2Gray() {}

// 插件的具体功能：将图像从 BGR 转换为灰度
int BGR2Gray::runSub()
{
    if(!validateInputs())
    {
        qDebug() << "Input ERROR!";
        return -1;
    }
    auto start = std::chrono::high_resolution_clock::now();
    //0 输出图像，1运行结果代码，2时间
    //outputs->init();
    qDebug() << "runSub BGR2Gray";
    cv::Mat img;

    if(inputs_.getDataValue("image").type() == typeid(cv::Mat))
    {
        img = std::any_cast<cv::Mat>(inputs_.getDataValue("image"));
    }
    else
    {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        outputs_.addData("image",img);
        outputs_.addData("errorCode",1);
        outputs_.addData("runTime",elapsed.count());
        qDebug() << "type error: need Mat";

    }

    if (img.empty() || img.channels() != 3)
    {
        qWarning() << "Received an invalid QImage!";
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        outputs_.addData("image",img);
        outputs_.addData("errorCode",1);
        outputs_.addData("runTime",elapsed.count());
        return -1;
    }

    // 2. 将 BGR 图像转换为灰度图像
    cv::Mat grayMat;
    cv::cvtColor(img, grayMat, cv::COLOR_BGR2GRAY);  
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;


    outputs_.addData("image",grayMat);
    outputs_.addData("errorCode",1);
    outputs_.addData("runTime",elapsed.count());
    return 1;
}


bool BGR2Gray::validateInputs()
{
    const std::vector<IOData>& data = inputs_.data;
    // 查找是否存在名为 "image" 的输入
    auto it = std::find_if(data.begin(), data.end(), [](const IOData& io){
        return io.name == "image";
    });

    if (it == data.end()) {
        qDebug() << "Error: Missing required input 'image'";
        return false;
    }

    // 检查 "image" 是否为 cv::Mat 类型
    try
    {
        const auto& imageInput = std::any_cast<cv::Mat>(it->value);
    }
    catch (const std::bad_any_cast&) {
        qDebug() << "Error: Input 'image' must be of type cv::Mat";
        return false;
    }

    // 验证通过
    return true;
}

std::vector<std::string> BGR2Gray::getInputsList()
{
    std::vector<std::string> vec;
    vec.push_back("image");
     return vec;
}
void  BGR2Gray:: setName()
{
    name_ = "BGR2Gray";

}

std::vector<std::string> BGR2Gray::getOutputsList()
{
    std::vector<std::string> vec;
    vec.push_back("image");
    return vec;
}

std::string BGR2Gray:: getInputType(const std::string& inputName)
{
    if(inputName == "image")
    {
        return "cv::Mat";
    }
    return "";
}
std::string BGR2Gray::getOutputType(const std::string& outputName)
{
    if(outputName == "image")
    {
        return "cv::Mat";
    }
    return "";
}
