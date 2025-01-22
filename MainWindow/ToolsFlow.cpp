#include "ToolsFlow.h"

#include<QDebug>
#include <any>

ToolsFlow::ToolsFlow()
{

}
ToolsFlow::~ToolsFlow()
{

}

void ToolsFlow::addTool(const std::shared_ptr<ToolsBase>& tool)
{
    tools_.push_back(tool);
}

void ToolsFlow::setInitialInput(const  std::vector<std::any>& input)
{
    initialInput_ = input;
}

bool ToolsFlow::executeFlow()
{
    if (tools_.empty())
    {
        qDebug()<< "No tools in the flow!" ;
        return false;
    }

    std::vector<std::any> currentInput = initialInput_;

    for (const auto& tool : tools_)
    {
        if (!tool)
        {
            qDebug()<< "Invalid tool in the flow!" ;
            return false;
        }

        tool->inputs = currentInput;

        // 执行工具
        if (tool->run() != 1)
        {
            qDebug() << "Tool execution failed!" ;
        }

        // 获取输出
        try
        {
            currentInput = tool->outputs->outputs_; // 假设工具输出的第一个是下一步输入


            qDebug() << "errorcode " <<tool->outputs->getErrroCode_() ;
        } catch (const std::bad_any_cast& e)
        {
            qDebug() << "Output type mismatch: " << e.what();
            return false;
        }
    }

    finalOutput_ = currentInput; // 保存最终输出
    return true;
}

std::vector<std::any> ToolsFlow::getFinalOutput() const
{
    return finalOutput_;
}

void ToolsFlow::setFlowID(std::string id)
{
    flowID_ = id;
}

std::string ToolsFlow::getFlowID()
{
    return flowID_;
}


