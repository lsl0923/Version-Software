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

void ToolsFlow::setInitialInput(const  IOConfig& input)
{
    initialInput_ = input;
}

int ToolsFlow::executeFlow()
{
    if (tools_.empty())
    {
        qDebug()<< "No tools in the flow!" ;
        return -1;
    }

    for (const auto& tool : tools_)
    {
        if (!tool)
        {
            qDebug()<< "Invalid tool in the flow!" ;
            return -1;
        }

        setToolInputs(tool);
        // 执行工具
        if (tool->run() != 1)
        {
            qDebug() << "Tool execution failed!" ;
        }


        finalOutput_ = tool->getOutputs(); // 保存最终输出

    }

    return 1;
}

IOConfig ToolsFlow::getFinalOutput() const
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

void ToolsFlow::setInput(const std::string& targetToolId, const std::string& sourceToolId,const std::string& inputName, const std::string& sourceOutputId)
{
    inputConfigs_.emplace_back(targetToolId, sourceToolId,inputName, sourceOutputId);
}


bool ToolsFlow::setToolInputs(const std::shared_ptr<ToolsBase>& tool)
{

//
    IOConfig currentInputs = initialInput_;

    for (const auto& inputConfig : inputConfigs_)
    {
        // 检查当前工具是否需要该输入
        if (inputConfig.targetToolId == tool->getToolId())
        {
            // 查找来源工具及其输出
            auto sourceTool = findToolById(inputConfig.sourceToolId);
            if (!sourceTool)
            {
                qDebug() << "Source tool not found for input:" << QString::fromStdString(inputConfig.sourceToolId);
                return false;
            }

            // 查找来源工具的指定输出
            try
            {
                auto outputValue = sourceTool->getOutputs().getDataValue(inputConfig.sourceOutputId);
                currentInputs.addData(inputConfig.inputName,outputValue);
            }

            catch(const std::exception& e)
            {
                qDebug() << "Source tool output not found for ID:" << QString::fromStdString(inputConfig.sourceOutputId);
                return false;
            }
        }
    }

    tool->setInputs(currentInputs);

    return true;
}
std::shared_ptr<ToolsBase> ToolsFlow::findToolById(const std::string& toolId) const
{
    for (const auto& tool : tools_)
    {
        if (tool && tool->getToolId() == toolId)
        {
            return tool;
        }
    }
    return nullptr;
}
std::vector<std::shared_ptr<ToolsBase>> ToolsFlow::  getTools()
{
     return tools_;
}
bool ToolsFlow::removeToolById(const std::string& toolId)
{
    auto it = std::find_if(tools_.begin(), tools_.end(),
                           [&toolId](const std::shared_ptr<ToolsBase>& tool)
                           {
                               return tool && tool->getToolId() == toolId;
                           });

    if (it != tools_.end()) {
        tools_.erase(it); // 从工具列表中移除
        return true;
    }
    return false; // 未找到指定的工具
}
