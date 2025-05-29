#ifndef SPHEREFACE_RECOGNIZER_H
#define SPHEREFACE_RECOGNIZER_H

#include "FastMTCNN.h" // 你的 MTCNN 封装类
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>

class SphereFaceRecognizer {
public:
    SphereFaceRecognizer(const std::string& model_dir, const std::string& mtcnn_dir);

    // 提取图像中第一个人脸的特征向量
    bool extractFeature(const cv::Mat& image, cv::Mat& feature);

    // 比较两张图像中的人脸是否相同
    float compareFaces(const cv::Mat& img1, const cv::Mat& img2);
    bool extractRawFeature(const cv::Mat& alignedImg, cv::Mat& feature);
private:
    cv::Mat alignFace(const cv::Mat& image, const FaceInfo& faceInfo);

    cv::dnn::Net net_;
    MTCNN mtcnn_;
};

#endif // SPHEREFACE_RECOGNIZER_H
