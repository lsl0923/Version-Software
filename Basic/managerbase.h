#ifndef MANAGERBASE_H
#define MANAGERBASE_H
#include"Basic_global.h"
#include"ToolsBase.h"
#include<string>
#include <QObject>
struct BASIC_EXPORT ToolsInfo
{
    std::string name = "";      // 工具名称
    std::string library = "";   // 库的名称
    std::string type = "";      // 工具的类型
    std::string description = ""; // 工具的描述

    ToolsInfo(std::string n = "", std::string lib = "", std::string t = "", std::string desc = "")
        : name(std::move(n)), library(std::move(lib)), type(std::move(t)), description(std::move(desc)) {}

    bool operator==(const ToolsInfo& other) const
    {
        return name == other.name && library == other.library && type == other.type && description == other.description;
    }

    bool operator<(const ToolsInfo& other) const
    {
        return name < other.name;
    }
};

Q_DECLARE_METATYPE(ToolsInfo)
class BASIC_EXPORT ManagerBase: public QObject
{
    Q_OBJECT
public:
    ManagerBase();
    std::vector<ToolsInfo> getToolsList();
    virtual ToolsBase* createTools(const ToolsInfo& pluginName) = 0;
protected:
    std::vector<ToolsInfo> toolsInfo_;

};

#endif // MANAGERBASE_H
