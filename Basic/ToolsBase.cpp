#include "ToolsBase.h"
#include<string>
#include<QDebug>




ToolsBase::ToolsBase()
{
    toolId_ = generateUniqueTimestamp();
      qDebug()<<"~To1111111olsBase11111111111111111";
}
ToolsBase:: ~ToolsBase()
{
    qDebug()<<"~To1111111olsBase11111111111111111";
    qDebug()<<"~To1111111olsBase11111111111111111";
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


bool ToolsBase::isMatchType( const  std::string& type_1,const std::string& type_2)
{
    if (type_1 == type_2)
        return true;


    if ((type_1 == "int" && (type_2 == "float" || type_2 == "double")) ||
        (type_1 == "float" && (type_2 == "int" || type_2 == "double")) ||
        (type_1 == "double" && (type_2 == "int" || type_2 == "float")))
    {
        return true;
    }

    return false;
}
