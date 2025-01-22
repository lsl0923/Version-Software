#ifndef OUTPUTS_H
#define OUTPUTS_H
#include"OutPutsBase.h"
#include"BGR2Gray_global.h"


class BGR2GRAY_EXPORT Outputs : public OutPutsBase
{
public:
    Outputs();

protected:
    void initSub();
};

#endif // OUTPUTS_H
