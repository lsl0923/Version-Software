#ifndef OUTPUTSBASE_H
#define OUTPUTSBASE_H

#include <vector>
#include <any>

class OutPutsBase
{
public:
    // 默认构造函数
    OutPutsBase();

    // 带输出参数的构造函数
    OutPutsBase(std::vector<std::any> outputs);

    // 初始化函数
    void init();
    double getErrorCode() const;

    void setErrorCode(double errorCode);

    double getRunTime() const ;

    void setRunTime(double runTime);
    std::vector<std::any> outputs_;
protected:
    // 受保护的子类初始化接口
    virtual void initSub() = 0;
    double errorCode_;
    double runTime_;
    // 成员变量

};

#endif // OUTPUTSBASE_H
