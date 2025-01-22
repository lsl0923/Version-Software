#ifndef TOOLITEM_H
#define TOOLITEM_H

#include <QListWidgetItem>
#include <QString>
#include <memory>
#include "ToolsBase.h"
#include "managerbase.h"

class ToolItem : public QListWidgetItem
{
public:

    ToolItem(const ToolsInfo& info = ToolsInfo(), std::shared_ptr<ToolsBase> toolInstance = nullptr);

    ToolsInfo getToolInfo() const;

    // 获取工具实例
    std::shared_ptr<ToolsBase> getToolInstance() const;
    void setToolInstance(std::shared_ptr<ToolsBase> toolInstance_);

private:
    ToolsInfo toolInfo_;  // 包含工具信息
    std::shared_ptr<ToolsBase> toolInstance_;

};
#endif // TOOLITEM_H
