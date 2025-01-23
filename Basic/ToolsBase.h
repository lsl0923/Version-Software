#ifndef TOOLSBASE_H
#define TOOLSBASE_H
#include<any>
#include "Basic_global.h"
#include<vector>
#include<memory>
#include"OutPutsBase.h"

class BASIC_EXPORT ToolsBase
{
public:
    ToolsBase();
    virtual ~ToolsBase() = default;
    int run();

    std::vector<std::any> inputs;
    std::shared_ptr<OutPutsBase> outputs;
protected:
    virtual int runSub() = 0;
};

#endif // TOOLSBASE_H
