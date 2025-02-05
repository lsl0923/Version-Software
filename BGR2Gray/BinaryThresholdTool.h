#ifndef BINARYTHRESHOLDTOOL_H
#define BINARYTHRESHOLDTOOL_H

#include "ToolsBase.h"
#include <opencv2/opencv.hpp>

class BinaryThresholdTool : public ToolsBase
{
public:
    BinaryThresholdTool();
    ~BinaryThresholdTool() = default;

    // 获取输入/输出的名称列表
    std::vector<std::string> getInputsList() override;
    std::vector<std::string> getOutputsList() override;
    virtual std::string getInputType(const std::string& inputName) override;  // 获取某个输入的类型
    virtual std::string getOutputType(const std::string& outputName) override; // 获取某个输出的类型

protected:
    int runSub() override; // 实现二值化逻辑
    void setName() override; // 设置插件名称
    bool validateInputs() override; // 验证输入数据
};

#endif // BINARYTHRESHOLDTOOL_H
