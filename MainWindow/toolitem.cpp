#include "toolitem.h"



ToolItem::ToolItem(const ToolsInfo& info, std::shared_ptr<ToolsBase> toolInstance)
    : QListWidgetItem(QString::fromStdString(info.name)), toolInfo_(info), toolInstance_(toolInstance)
{
  //  setText(QString::fromStdString(info.name));
}
// 获取工具实例
std::shared_ptr<ToolsBase> ToolItem::getToolInstance() const
{
    return toolInstance_;
}

// 获取 ToolsInfo 结构体
ToolsInfo ToolItem::getToolInfo() const
{
    return toolInfo_;
}

void ToolItem::setToolInstance(std::shared_ptr<ToolsBase> toolInstance)
{
    this->toolInstance_ =  (toolInstance);;
}
