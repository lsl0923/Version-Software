#include<QDebug>


#include "Filter.h"


Filter::Filter()
{
    setName();
}

Filter::~Filter()
{
    // 确保有定义
}
void Filter::setName()
{
    name_ = "Filter";
}


int Filter::runSub()
{

    if(!validateInputs())
    {
        qDebug() << "Input ERROR!";
        return -1;
    }
    // 获取输入图像和阈值
    auto image = std::any_cast<cv::Mat>(inputs_.getDataValue("image"));
    auto type = std::any_cast<int>(inputs_.getDataValue("type"));
    auto size = std::any_cast<int>(inputs_.getDataValue("size"));
    if (image.empty())
    {
        qDebug() << "Input image is empty!";
        return -1;
    }

    cv::Mat outputImage;
    // 设置输出
    if(type == 0)
    {
        // 中值滤波
        cv::medianBlur(image, outputImage, size);
        outputs_.addData("image", outputImage);
    }
    else if(type == 1)
    {
        // 均值滤波
        cv::blur(image, outputImage, cv::Size(size, size));
        outputs_.addData("image", outputImage);
    }
    else if(type == 2)
    {
        // 高斯滤波，sigmaX 设置为 0 让 OpenCV 自动计算
        cv::GaussianBlur(image, outputImage, cv::Size(size, size), 0);
        outputs_.addData("image", outputImage);
    }
    else
    {
       qDebug() << "Unsupported filter type: " << type;
    }






    return 0;
}

bool Filter::validateInputs()
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

    it = std::find_if(data.begin(), data.end(), [](const IOData& io)
                           {
                               return io.name == "type";
                           });

    if (it == data.end())
    {
        qDebug() << "Error: Missing required input 'type'";
        return false;
    }

    try
    {
        const auto& imageInput = std::any_cast<int>(it->value);
    }
    catch (const std::bad_any_cast&)
    {
        qDebug() << "Error: Input 'image' must be of type type";
        return false;
    }



    it = std::find_if(data.begin(), data.end(), [](const IOData& io)
                      {
                          return io.name == "size";
                      });

    if (it == data.end())
    {
        qDebug() << "Error: Missing required input 'size'";
        return false;
    }

    try
    {
        const auto& imageInput = std::any_cast<int>(it->value);
        if(imageInput%2 == 0)
        {
            return false;
        }
    }
    catch (const std::bad_any_cast&)
    {
        qDebug() << "Error: Input 'size' must be of type int";
        return false;
    }

    return true;
}

std::vector<std::string> Filter::getInputsList()
{
    std::vector<std::string> vec;
    vec.push_back("image");
      vec.push_back("type");
      vec.push_back("size");
    return vec;
}

std::vector<std::string> Filter::getOutputsList()
{
    std::vector<std::string> vec;
    vec.push_back("image");
    return vec;
}

std::string Filter::getInputType(const std::string& inputName)
{
    if(inputName == "image")
    {
        return "cv::Mat";
    }
    else
    {
        return "int";
    }

}

std::string Filter::getOutputType(const std::string& outputName)
{
    if(outputName == "image")
    {
        return "cv::Mat";
    }
    else
    {
        return "int";
    }
}
