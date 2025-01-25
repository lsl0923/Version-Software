#include "OutPutsBase.h"
#include <stdexcept>
// 默认构造函数
OutPutsBase::OutPutsBase()
{
}

// 初始化函数
void OutPutsBase::init()
{

    initSub();
}


double OutPutsBase::getErrorCode() const
{
    return errorCode_;
}

void OutPutsBase::setErrorCode(double errorCode)
{
    errorCode_ = errorCode;
}

// Getter and Setter for runTime_
double OutPutsBase::getRunTime() const
{
    return runTime_;
}

void OutPutsBase::setRunTime(double runTime)
{
    runTime_ = runTime;
}
void OutPutsBase::addOutput(const std::string& outputId, const std::any& value)
{
    outputs_[outputId] = value;  // 插入或更新输出
}

std::any OutPutsBase::getOutput(const std::string& outputId) const
{
    auto it = outputs_.find(outputId);
    if (it == outputs_.end())
    {
        throw std::runtime_error("Output ID not found: " + outputId);
    }
    return it->second;
}
