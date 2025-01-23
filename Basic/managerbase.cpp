#include "managerbase.h"

ManagerBase::ManagerBase() {}


std::vector<ToolsInfo>ManagerBase:: getToolsList()
{
    return toolsInfo_;
}
