#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <vector>
#include "ToolsBase.h"
#include "managerbase.h"





class Manager :public ManagerBase {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID  "com.ManagerBase")

public:
    explicit Manager();
    virtual  ToolsBase* createTools(const ToolsInfo& pluginName) override;

    // 获取插件工具


};

#endif // MANAGER_H
