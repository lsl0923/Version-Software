#ifndef FACEDETECT_H
#define FACEDETECT_H

#include <opencv2/opencv.hpp>
#include"BGR2Gray_global.h"
#include"ToolsBase.h"
#include"FaceDatabase.h"
#include"SphereFaceRecognizer.h"
#include<QMessageBox>
class BGR2GRAY_EXPORT FaceDetect: public ToolsBase
{
public:
    FaceDetect();
    virtual ~FaceDetect() override;
    void initInput();
    // 重写 runSub 方法
    int runSub() override;
    void addFace();
    bool validateInputs() override;
    std::vector<std::string> getInputsList() override;
    std::vector<std::string> getOutputsList() override;
    virtual std::string getInputType(const std::string& inputName) override;  // 获取某个输入的类型
    virtual std::string getOutputType(const std::string& outputName) override; // 获取某个输出的类型
    void setName() override;
    void faceDetect_Haar(cv::Mat& img, cv::Mat& dst, std::vector<cv::Rect>& faceRects);
    void faceDetect_FastMTCNN(cv::Mat& img, cv::Mat& dst, std::vector<cv::Rect>& faceRects);
    int finalizeOutput(const cv::Mat& img, int code,
                                   std::chrono::high_resolution_clock::time_point start);

private:
    SphereFaceRecognizer recognizer_;
    FaceDatabase db_;
};


#endif // FACEDETECT_H
