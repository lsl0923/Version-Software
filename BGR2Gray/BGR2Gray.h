
#ifndef BGR2GRAY_H
#define BGR2GRAY_H
#include <opencv2/opencv.hpp>
#include <QWidget>
#include <QObject>
#include "ToolsBase.h"
#include "BGR2Gray_global.h"

class BGR2GRAY_EXPORT BGR2Gray :public ToolsBase
{


public:

    BGR2Gray();
    virtual ~BGR2Gray() override;

    // 重写 runSub 方法
    int runSub() override;
    bool  validateInputs(const std::vector<IOData>& data) override;
    std::vector<std::string> getInputsList() override;
    std::vector<std::string> getOutputsList() override;
    void setName() override;
};


#endif // BGR2GRAY_H
