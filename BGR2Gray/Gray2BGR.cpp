

#include <QDebug>
#include <QImage>
#include <QColor>
#include<QObject>
#include"Outputs.h"
#include"ToolsBase.h"
#include <opencv2/opencv.hpp>
#include "Gray2BGR.h"
// 构造函数
Gray2BGR::Gray2BGR() :  ToolsBase(){
    // 初始化或配置任何所需的成员变量
    std::shared_ptr<OutPutsBase> out = std::make_shared<Outputs>();
    outputs = out;
}

// 析构函数
Gray2BGR::~Gray2BGR() {}

int Gray2BGR::runSub()
{
    auto start = std::chrono::high_resolution_clock::now();
    //0 输出图像，1运行结果代码，2时间
    outputs->init();
    qDebug() << "runSub Gray";
    cv::Mat img;

    if(inputs[0].type() == typeid(cv::Mat))
    {
        img = std::any_cast<cv::Mat>(inputs[0]);
    }
    else
    {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        outputs->outputs_[0] = img;
        outputs->setErrroCode_(-1);
        outputs->setRunTime(elapsed.count());
        qDebug() << "type error: need Mat";

    }
     qDebug()<<img.channels();
    if (img.empty() || img.channels() != 1)
    {
        qWarning() << "Received an invalid QImage!";
        outputs->outputs_[0] = img;
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        outputs->setErrroCode_(-1);
        outputs->setRunTime(elapsed.count());

        return -1;
    }
    cv::Mat BGRMat;
    cv::cvtColor(img, BGRMat, cv::COLOR_GRAY2BGR);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    outputs->setErrroCode_(1);
    outputs->setRunTime(elapsed.count());
    outputs->outputs_[0] = BGRMat;
    return 1;
}




