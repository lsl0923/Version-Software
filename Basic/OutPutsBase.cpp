#include "OutPutsBase.h"

// 默认构造函数
OutPutsBase::OutPutsBase()
{
}

// 带输出参数的构造函数
OutPutsBase::OutPutsBase(std::vector<std::any> outputs)
    : outputs_(std::move(outputs)) {
    init();
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

