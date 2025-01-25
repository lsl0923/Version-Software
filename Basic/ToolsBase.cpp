#include "ToolsBase.h"
#include<string>
ToolsBase::ToolsBase()
{

}

int ToolsBase::run()
{
    return runSub();
}

const std::vector<std::any>& ToolsBase::getInputs() const
{
    return inputs;
}


void ToolsBase::setInputs(const std::vector<std::any>& newInputs)
{
    inputs = newInputs;
}


std::shared_ptr<OutPutsBase> ToolsBase::getOutputs() const
{
    return outputs;
}


void ToolsBase::setOutputs(const std::shared_ptr<OutPutsBase>& newOutputs)
{
    outputs = newOutputs;
}
std::string ToolsBase::getName()
{
    return name_;
}
 std::string ToolsBase:: getToolId()
{
     return toolId_;
}
std::string ToolsBase::generateUniqueTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return std::to_string(duration.count());
}
