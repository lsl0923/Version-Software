#include<QDebug>

#include "TemMatcher.h"



TemMatcher::TemMatcher()
{
    setName();
}

TemMatcher::~TemMatcher()
{
    // 确保有定义
}
void TemMatcher::setName()
{
    name_ = "TemMatcher";
}


int TemMatcher::runSub()
{
    return 0;
}

bool TemMatcher::validateInputs()
{
    const std::vector<IOData>& data = inputs_.data;
    // 查找是否存在名为 "image" 的输入
    auto it = std::find_if(data.begin(), data.end(), [](const IOData& io)
    {
        return io.name == "image";
    });

    if (it == data.end())
    {
        qDebug() << "Error: Missing required input 'image'";
        return false;
    }

    // 检查 "image" 是否为 cv::Mat 类型
    try
    {
        const auto& imageInput = std::any_cast<cv::Mat>(it->value);
    }
    catch (const std::bad_any_cast&)
    {
        qDebug() << "Error: Input 'image' must be of type cv::Mat";
        return false;
    }



    // 查找是否存在名为 "Template" 的输入
    it = std::find_if(data.begin(), data.end(), [](const IOData& io)
                           {
                               return io.name == "Template";
                           });

    if (it == data.end())
    {
        qDebug() << "Error: Missing required input 'Template'";
        return false;
    }

    // 检查 "Template" 是否为 cv::Mat 类型
    try
    {
        const auto& Template = std::any_cast<cv::Mat>(it->value);
    }
    catch (const std::bad_any_cast&)
    {
        qDebug() << "Error: Input 'Template' must be of type cv::Mat";
        return false;
    }
    // 验证通过
    return true;
}

std::vector<std::string> TemMatcher::getInputsList()
{
    std::vector<std::string> vec;
    vec.push_back("image");
    vec.push_back("Template");
    return vec;
}

std::vector<std::string> TemMatcher::getOutputsList()
{
    std::vector<std::string> vec;
    vec.push_back("image");
    return vec;
}

std::string TemMatcher::getInputType(const std::string& inputName)
{
    return "cv::Mat";
}

std::string TemMatcher::getOutputType(const std::string& outputName)
{
    return "cv::Mat";
}


