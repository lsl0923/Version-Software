
#ifndef Gray2BGR_H
#define Gray2BGR_H
#include <opencv2/opencv.hpp>
#include <QWidget>
#include <QObject>
#include "ToolsBase.h"
#include "BGR2Gray_global.h"


class BGR2GRAY_EXPORT Gray2BGR :public ToolsBase
{


public:

    Gray2BGR();
    virtual ~Gray2BGR() override;

    // 重写 runSub 方法
    int runSub() override;
    bool validateInputs() override;
    std::vector<std::string> getInputsList() override;
    std::vector<std::string> getOutputsList() override;
    virtual std::string getInputType(const std::string& inputName) override;  // 获取某个输入的类型
    virtual std::string getOutputType(const std::string& outputName) override; // 获取某个输出的类型
    void setName() override;
};


#endif // Gray2BGR_H
