#include "ToolsBase.h"
#include<string>
#include<QDebug>







ToolsBase::ToolsBase()
{
    toolId_ = generateUniqueTimestamp();
}
ToolsBase:: ~ToolsBase()
{
    qDebug()<<"~ToolsBase";
}
int ToolsBase::run()
{
    return runSub();
}

const IOConfig& ToolsBase::getInputs() const
{
    return inputs_;
}

// 设置输入
void ToolsBase::setInputs(const IOConfig& newInputs)
{
    inputs_ = newInputs;
}

// 获取输出
const IOConfig& ToolsBase::getOutputs() const
{
    return outputs_;
}

// 设置输出
void ToolsBase::setOutputs(const IOConfig& newOutputs)
{
    outputs_ = newOutputs;
}
std::string ToolsBase::getName()
{
    return name_;
}
 std::string ToolsBase:: getToolId()
{
    return name_ + toolId_;
}
std::string ToolsBase::generateUniqueTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return std::to_string(duration.count());
}
