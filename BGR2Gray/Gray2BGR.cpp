

#include <QDebug>
#include <QImage>
#include <QColor>
#include<QObject>

#include"ToolsBase.h"
#include <opencv2/opencv.hpp>
#include "Gray2BGR.h"
// 构造函数
Gray2BGR::Gray2BGR() :  ToolsBase()
{
    // 初始化或配置任何所需的成员变量
    //std::shared_ptr<OutPutsBase> out = std::make_shared<Outputs>();
    //outputs = out;
    setName();

}


void  Gray2BGR:: setName()
{
    name_ = "Gray2BGR";

}
// 析构函数
Gray2BGR::~Gray2BGR() {}

int Gray2BGR::runSub()
{
    if(!validateInputs())
    {
        qDebug() << "Input ERROR!";
        return -1;
    }
    auto start = std::chrono::high_resolution_clock::now();
    //0 输出图像，1运行结果代码，2时间
   // outputs->init();
    qDebug() << "runSub Gray";
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

    if (img.empty() || img.channels() != 1)
    {
        qWarning() << "Received an invalid QImage!";
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        outputs_.addData("image",img);
        outputs_.addData("errorCode",1);
        outputs_.addData("runTime",elapsed.count());

        return -1;
    }
    cv::Mat BGRMat;
    cv::cvtColor(img, BGRMat, cv::COLOR_GRAY2BGR);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    outputs_.addData("image",BGRMat);
    outputs_.addData("errorCode",1);
    outputs_.addData("runTime",elapsed.count());
    return 1;
}



bool Gray2BGR::validateInputs()
{
     std::vector<IOData>& data = inputs_.data;
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
std::vector<std::string> Gray2BGR::getInputsList()
{
    std::vector<std::string> vec;
    vec.push_back("image");
    vec.push_back("Test01");
    return vec;
}
std::vector<std::string> Gray2BGR::getOutputsList()
{
    std::vector<std::string> vec;
    vec.push_back("image");
    vec.push_back("Test01");
    return vec;
}
std::string Gray2BGR:: getInputType(const std::string& inputName)
{
    if(inputName == "image")
    {
        return "cv::Mat";
    }
    return "";
}
std::string Gray2BGR::getOutputType(const std::string& outputName)
{
    if(outputName == "image")
    {
        return "cv::Mat";
    }
    return "";
}
