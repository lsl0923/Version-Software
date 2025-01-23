
#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <opencv2/opencv.hpp>

class ImageViewer : public QWidget {
    Q_OBJECT

public:
    ImageViewer(QWidget *parent = nullptr);
    void setImage(const cv::Mat &mat);
    void centerImage();
     cv::Mat getImage() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void showEvent(QShowEvent *event) override;
private:
    cv::Mat image_;
    QPoint offset_;
    double scaleFactor_;
    QPoint lastPos_;
    int parent_width_;
    int parent_height_;
    QString coordinateText_;
};

#endif // IMAGEVIEWER_H
