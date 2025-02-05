#include "BinaryThresholdTool.h"
#include <QDebug>

BinaryThresholdTool::BinaryThresholdTool()
{
    setName(); // 设置插件名称
}

void BinaryThresholdTool::setName()
{
    name_ = "BinaryThresholdTool";
}

std::vector<std::string> BinaryThresholdTool::getInputsList()
{
    return {"image", "ThresholdValue"};
}

std::vector<std::string> BinaryThresholdTool::getOutputsList()
{
    return {"image"};
}

bool BinaryThresholdTool::validateInputs()
{
    const std::vector<IOData>& data = inputs_.data;
    bool hasImage = false;
    bool hasThreshold = false;

    for (const auto& input : data)
    {
        if (input.name == "image" && input.value.type() == typeid(cv::Mat))
        {
            hasImage = true;
        }

        // 检查 ThresholdValue 是否存在以及其类型
        if (input.name == "ThresholdValue")
        {
            if (input.value.type() == typeid(int) ||
                input.value.type() == typeid(long) ||
                input.value.type() == typeid(double) ||
                input.value.type() == typeid(float))
            {
                hasThreshold = true;
            }
            else
            {
                qDebug() << "Invalid type for ThresholdValue!";
                return false;
            }
        }
    }

    if (!hasThreshold)
    {
        inputs_.addData("ThresholdValue", 127.0);
        hasThreshold = true;
    }

    // 打印调试信息
    if (!hasImage)
    {
        qDebug() << "image is missing!";
    }
    if (!hasThreshold)
    {
        qDebug() << "ThresholdValue is missing!";
    }

    return hasImage && hasThreshold;
}

int BinaryThresholdTool::runSub()
{
    if(!validateInputs())
    {
        qDebug() << "Input ERROR!";
        return -1;
    }
    // 获取输入图像和阈值
    auto image = std::any_cast<cv::Mat>(inputs_.getDataValue("image"));
    auto thresholdValue = std::any_cast<double>(inputs_.getDataValue("ThresholdValue"));

    if (image.empty())
    {
        qDebug() << "Input image is empty!";
        return -1;
    }

    cv::Mat outputImage;
    cv::threshold(image, outputImage, thresholdValue, 255, cv::THRESH_BINARY);

    // 设置输出
    outputs_.addData("image", outputImage);

    return 0; // 成功返回 0
}




std::string BinaryThresholdTool:: getInputType(const std::string& inputName)
{
    if(inputName == "image")
    {
        return "cv::Mat";
    }
    else if(inputName == "ThresholdValue")
    {
        return "double";
    }
    return "";
}
std::string BinaryThresholdTool::getOutputType(const std::string& outputName)
{
    if(outputName == "image")
    {
        return "cv::Mat";
    }
    return "";
}
