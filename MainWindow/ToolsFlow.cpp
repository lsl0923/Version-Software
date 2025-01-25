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

        tool->setInputs(currentInput);

        // 执行工具
        if (tool->run() != 1)
        {
            qDebug() << "Tool execution failed!" ;
        }
        finalOutput_.push_back(tool->getOutputs()->getOutput("Img")); // 保存最终输出

    }

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

void ToolsFlow::setInput(const std::string& targetToolId, const std::string& sourceToolId, const std::string& sourceOutputId)
{
    inputConfigs_.emplace_back(targetToolId, sourceToolId, sourceOutputId);
}


bool ToolsFlow::setToolInputs(const std::shared_ptr<ToolsBase>& tool)
{
    std::vector<std::any> currentInputs;

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
                 auto outputValue = sourceTool->getOutputs()->getOutput(inputConfig.sourceOutputId);
                 currentInputs.push_back(outputValue);
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
std::shared_ptr<ToolsBase> ToolsFlow::findToolById(const std::string& toolId) const {
    for (const auto& tool : tools_) {
        if (tool && tool->getToolId() == toolId) {
            return tool;
        }
    }
    return nullptr;
}
