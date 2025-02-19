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
void ToolsFlow::setInput(InputConfig& input)
{
    inputConfigs_.push_back(input);
}

bool ToolsFlow::setToolInputs(const std::shared_ptr<ToolsBase>& tool)
{

//
    IOConfig currentInputs = tool->getInputs();
    for(auto input :initialInput_.data)
    {
        if(input.name == "image")
        {
            currentInputs.addData(input.name,input.value);
        }
        if(!currentInputs.getDataValue(input.name).has_value())
        {
            currentInputs.addData(input.name,input.value);
        }
    }

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

                // 检查 inputConfig.value 是否包含有效的值
                if (inputConfig.value.has_value())
                {
                    try
                    {
                        // 尝试将 std::any 转换为正确的类型
                        double value = std::any_cast<double>(inputConfig.value);
                        currentInputs.addData(inputConfig.inputName, value);
                        qDebug() << "Added input data for:" << QString::fromStdString(inputConfig.inputName);
                        continue;
                    }
                    catch (const std::bad_any_cast& e)
                    {
                        // 如果类型转换失败
                        qDebug() << "Failed to cast value for input:" << QString::fromStdString(inputConfig.inputName)
                                 << "Error:" << e.what();
                    }
                }
                else
                {
                    qDebug() << "Skipping empty input value for:" << QString::fromStdString(inputConfig.inputName);
                    continue;
                }
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
