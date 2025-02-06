#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <vector>
#include "ToolsBase.h"
#include "managerbase.h"

class Manager : public ManagerBase
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ManagerBase")

public:
    explicit Manager();
    ~Manager() override = default;

    ToolsBase* createTools(const ToolsInfo& pluginName) override;

};

#endif // MANAGER_H
