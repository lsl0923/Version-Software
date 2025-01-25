
#include <QDebug>
#include <QImage>
#include <QColor>
#include<QObject>
#include <chrono>
#include"ToolsBase.h"
#include <opencv2/opencv.hpp>
#include "BGR2Gray.h"
#include"Outputs.h"
// 构造函数
BGR2Gray::BGR2Gray() :  ToolsBase()
{
    // 初始化或配置任何所需的成员变量
    std::shared_ptr<OutPutsBase> out = std::make_shared<Outputs>();
    outputs = out;
    name_ = "BGR2Gray";
    toolId_ = name_+generateUniqueTimestamp();

}

// 析构函数
BGR2Gray::~BGR2Gray() {}

// 插件的具体功能：将图像从 BGR 转换为灰度
int BGR2Gray::runSub()
{
    auto start = std::chrono::high_resolution_clock::now();
    //0 输出图像，1运行结果代码，2时间
    outputs->init();
    qDebug() << "runSub BGR2Gray";
    cv::Mat img;

    if(inputs[0].type() == typeid(cv::Mat))
    {
        img = std::any_cast<cv::Mat>(inputs[0]);
    }
    else
    {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        outputs->setErrorCode(-1);
        outputs->setRunTime(elapsed.count());
        outputs->addOutput("Img",img);
        qDebug() << "type error: need Mat";

    }
    qDebug()<<img.channels();
    if (img.empty() || img.channels() != 3)
    {
        qWarning() << "Received an invalid QImage!";
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        outputs->setErrorCode(-1);
        outputs->setRunTime(elapsed.count());
        outputs->addOutput("Img",img);
        return -1;
    }

    // 2. 将 BGR 图像转换为灰度图像
    cv::Mat grayMat;
    cv::cvtColor(img, grayMat, cv::COLOR_BGR2GRAY);
       outputs->addOutput("Img",grayMat);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    outputs->setErrorCode(1);
    outputs->setRunTime(elapsed.count());
    return 1;
}

