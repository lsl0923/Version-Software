#include "Outputs.h"
#include<QDebug>
#include <opencv2/opencv.hpp>
Outputs::Outputs()
{
     outputs_ = std::vector(10, std::any());
}



void Outputs:: initSub()
{
    cv::Mat mat;
    outputs_[0] = mat;
    qDebug()<<" initSub()";
}
