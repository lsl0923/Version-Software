#include "SphereFaceRecognizer.h"

using namespace cv;
using namespace std;

SphereFaceRecognizer::SphereFaceRecognizer(const std::string& model_dir, const std::string& mtcnn_dir)
    : mtcnn_(mtcnn_dir)
{
    std::string modelTxt = model_dir + "/sphereface_deploy.prototxt";
    std::string modelBin = model_dir + "/sphereface_model.caffemodel";
    net_ = cv::dnn::readNetFromCaffe(modelTxt, modelBin);

    if (net_.empty()) {
        std::cerr << "Failed to load SphereFace model." << std::endl;
        exit(-1);
    }
}

cv::Mat SphereFaceRecognizer::alignFace(const cv::Mat& image, const FaceInfo& faceInfo) {
    // 以眼睛为基础做仿射对齐
    Point2f srcTri[5], dstTri[5];
    for (int i = 0; i < 5; ++i) {
        srcTri[i] = Point2f(faceInfo.landmark[2 * i], faceInfo.landmark[2 * i + 1]);
    }

    // 目标对齐位置（参考 SphereFace 使用）
    dstTri[0] = Point2f(30.2946f, 51.6963f);
    dstTri[1] = Point2f(65.5318f, 51.5014f);
    dstTri[2] = Point2f(48.0252f, 71.7366f);
    dstTri[3] = Point2f(33.5493f, 92.3655f);
    dstTri[4] = Point2f(62.7299f, 92.2041f);
    std::vector<cv::Point2f> srcVec(srcTri, srcTri + 5);
    std::vector<cv::Point2f> dstVec(dstTri, dstTri + 5);
    Mat trans = estimateAffinePartial2D(srcVec, dstVec);
    Mat aligned;
    warpAffine(image, aligned, trans, Size(96, 112));
    return aligned;
}
/*
bool SphereFaceRecognizer::extractFeature(const cv::Mat& image, cv::Mat& feature)
{
    std::vector<FaceInfo> faces = mtcnn_.Detect_mtcnn(image, 12, new float[3]{0.6f, 0.7f, 0.7f}, 0.709f, 3);
    if (faces.empty()) return false;

    Mat alignedFace = alignFace(image, faces[0]);
    cv::Mat inputBlob = cv::dnn::blobFromImage(image, 1.0 / 255, cv::Size(112, 96));
    net_.setInput(inputBlob, "data");
    Mat output = net_.forward("fc5");

    normalize(output, feature); // L2 归一化
    return true;


}
*/

bool SphereFaceRecognizer::extractRawFeature(const cv::Mat& alignedImg, cv::Mat& feature)
{
    // 如果输入不是 112x96，则手动 resize，保证一致性
    cv::Mat resized;
    cv::resize(alignedImg, resized, cv::Size(112, 96));

    cv::Mat rgb;
    cv::cvtColor(resized, rgb, cv::COLOR_BGR2RGB);

    cv::Mat inputBlob = cv::dnn::blobFromImage(rgb, 1.0 / 255, cv::Size(112, 96));
    net_.setInput(inputBlob, "data");
    cv::Mat output = net_.forward("fc5");

    normalize(output, feature); // L2 normalize
    return true;
}
bool SphereFaceRecognizer::extractFeature(const cv::Mat& image, cv::Mat& feature)
{
    std::vector<FaceInfo> faces = mtcnn_.Detect_mtcnn(image, 12, new float[3]{0.6f, 0.7f, 0.7f}, 0.709f, 3);
    if (faces.empty()) return false;

    // 1. 对齐人脸
    cv::Mat alignedFace = alignFace(image, faces[0]);

    // 2. 转换为 RGB（很多模型要求）
    cv::Mat rgb;
    cv::cvtColor(alignedFace, rgb, cv::COLOR_BGR2RGB);

    // 3. blob 输入模型
    cv::Mat inputBlob = cv::dnn::blobFromImage(rgb, 1.0 / 255, cv::Size(112, 96));

    net_.setInput(inputBlob, "data");
    cv::Mat output = net_.forward("fc5");

    normalize(output, feature); // L2 归一化
    return true;
}
float SphereFaceRecognizer::compareFaces(const cv::Mat& img1, const cv::Mat& img2)
{
    Mat feat1, feat2;
    if (!extractFeature(img1, feat1) || !extractFeature(img2, feat2))
    {
        return -1.0f; // 人脸提取失败
    }

    return feat1.dot(feat2); // 余弦相似度
}
