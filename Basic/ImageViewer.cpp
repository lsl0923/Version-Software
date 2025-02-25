#include "ImageViewer.h"
#include <QPainter>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include<QEventLoop>
#include<QTimer>
ImageViewer::ImageViewer(QWidget *parent)
    : QWidget(parent), scaleFactor_(1.0), offset_(0, 0)
{
    setMouseTracking(true);
    QWidget *parentWidget = this->parentWidget();
    paintmode_ = PaintMode::PaintMode_None;
}


// 转换 OpenCV Mat 到 QImage
QImage MatToQImage(const cv::Mat& mat)
{
    switch (mat.type())
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image(mat.data,
                     mat.cols, mat.rows,
                     static_cast<int>(mat.step),
                     QImage::Format_ARGB32);

        return image;
    }

        // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image(mat.data,
                     mat.cols, mat.rows,
                     static_cast<int>(mat.step),
                     QImage::Format_RGB888);

        return image.rgbSwapped();
    }

        // 8-bit, 1 channel
    case CV_8UC1:
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        QImage image(mat.data,
                     mat.cols, mat.rows,
                     static_cast<int>(mat.step),
                     QImage::Format_Grayscale8);
#else
        static QVector<QRgb>  sColorTable;

        // only create our color table the first time
        if (sColorTable.isEmpty())
        {
            sColorTable.resize( 256 );

            for ( int i = 0; i < 256; ++i )
            {
                sColorTable[i] = qRgb( i, i, i );
            }
        }

        QImage image(mat.data,
                     mat.cols, mat.rows,
                     static_cast<int>(mat.step),
                     QImage::Format_Indexed8 );

        image.setColorTable(sColorTable);
#endif

        return image;
    }

        // wrong
    default:
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        break;
    }
    return QImage();
}

void ImageViewer::setImage(const cv::Mat &mat)
{


    image_ = mat.clone();
    overlay_ = cv::Mat(image_.rows, image_.cols, CV_8UC4, cv::Scalar(0, 0, 0,0));  // 初始化为透明图像

    // 根据窗口大小调整缩放比例
   /* if (parent_width_ > 0 && parent_height_ > 0)
    {
        double widthRatio = static_cast<double>(parent_width_) / image_.cols;
        double heightRatio = static_cast<double>(parent_height_) / image_.rows;
        scaleFactor_ = std::min(widthRatio, heightRatio); // 确保图片适应窗口，不超出范围
    }*/

    // 更新偏移量以居中
   // centerImage();
    update();
}

void mergeImg(cv::Mat& image , cv::Mat overlay)
{

}
void ImageViewer::paintEvent(QPaintEvent *event)
{

    QImage image_overlay = MatToQImage(overlay_);
    QImage image = MatToQImage(image_);
    QPainter painter(this);
    QRect targetRect(offset_, QSize(image.width() * scaleFactor_, image.height() * scaleFactor_));
    painter.drawImage(targetRect, image);
   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(targetRect, image_overlay);
    if (!coordinateText_.isEmpty()) {
        QFont font = painter.font();
        font.setPointSize(10); // 设置字体大小
        painter.setFont(font);

        int margin = 10; // 距离左下角的边距
        QPoint textPos(margin, height() - margin); // 坐标显示位置
        painter.setPen(Qt::black);                // 白色字体
        painter.drawText(textPos, coordinateText_); // 绘制坐标文本
    }
    if (drawing_ && paintmode_ == PaintMode_Line)
    {
        QPoint start = tempStartPos_ * scaleFactor_ + offset_;
        QPoint end = lastPos_ * scaleFactor_ + offset_;
        painter.setPen(QPen(Qt::yellow, 2, Qt::DashLine)); // 黄色虚线
        painter.drawLine(start, end);
    }
    else if (drawing_ && paintmode_ == PaintMode_Rectangle)
    {

        painter.setPen(QPen(Qt::yellow, 2, Qt::DashLine));
        QRect rect(tempStartPos_ * scaleFactor_ + offset_,
                   QSize((lastPos_.x() - tempStartPos_.x()) * scaleFactor_,
                         (lastPos_.y() - tempStartPos_.y()) * scaleFactor_));
        painter.setPen(QPen(Qt::yellow, 2, Qt::DashLine));
        painter.drawRect(rect);
    }
    else if(drawing_ && paintmode_ == PaintMode_Ellipse)
    {
        int x = std::min(tempStartPos_.x(), lastPos_.x()) * scaleFactor_ + offset_.x();
        int y = std::min(tempStartPos_.y(), lastPos_.y()) * scaleFactor_ + offset_.y();
        int width = std::abs(lastPos_.x() - tempStartPos_.x()) * scaleFactor_;
        int height = std::abs(lastPos_.y() - tempStartPos_.y()) * scaleFactor_;

        painter.setPen(QPen(Qt::yellow, 2, Qt::DashDotLine)); // 设置颜色为蓝色，线型为点划线，线宽为2
        painter.drawEllipse(x, y, width, height); // 绘制椭圆
    }
}

void ImageViewer::mousePressEvent(QMouseEvent *event)
{
    lastPos_ = (event->pos() - offset_) / scaleFactor_;
    tempStartPos_ = lastPos_;
    if (event->button() == Qt::LeftButton)
    {
        drawing_ = true;
    }
}
void ImageViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && drawing_)
    {
        if (paintmode_ == PaintMode_Line)
        {
            drawLine(tempStartPos_,lastPos_); // 只在释放时画到 `cv::Mat`
        }
        else if(paintmode_ == PaintMode_Rectangle)
        {
            drawRectangle(tempStartPos_, lastPos_);
        }
        else if(paintmode_ == PaintMode_Ellipse)
        {
            drawEllipse(tempStartPos_, lastPos_);
        }
        drawing_ = false;
        update();
    }
}
void ImageViewer::mouseMoveEvent(QMouseEvent *event)
{

    if (event->buttons() & Qt::MiddleButton)
    {
        lastPos_ = lastPos_ * scaleFactor_ + offset_;
        // 仅在中键按下时进行拖动操作
        offset_ += event->pos() - lastPos_;
        lastPos_ = (event->pos() - offset_) / scaleFactor_;
    }
    else if (event->buttons() & Qt::LeftButton)
    {
        if (drawing_)
        {
            if(paintmode_ == PaintMode_FreeDraw)
            {
                drawFreeLine(event->pos());
            }
            else if (paintmode_ == PaintMode_Erase)
            {
                erase(event->pos(), 20);  // 这里10是擦除区域的大小，你可以根据需要调整
            }
            lastPos_ = (event->pos() - offset_) / scaleFactor_;
        }
    }
    // 始终更新鼠标坐标信息
    QPointF imageCoords = (event->pos() - offset_) / scaleFactor_;
    coordinateText_ = QString("X: %1, Y: %2")
                          .arg(static_cast<int>(imageCoords.x()))
                          .arg(static_cast<int>(imageCoords.y()));

    update(); // 刷新界面以显示更新后的坐标
}


void ImageViewer::wheelEvent(QWheelEvent *event) {
    QPoint mousePos = event->pos();

    // 缩放前的图像在控件中的坐标
    QPointF beforeScalePoint = (mousePos - offset_) / scaleFactor_;
    if (event->angleDelta().y() > 0 && scaleFactor_<10)
    {
        scaleFactor_ *= 1.15;

    }
    else if(event->angleDelta().y() <= 0 && scaleFactor_>0.1)
    {
        scaleFactor_ /= 1.15;
    }
    QPointF afterScalePoint = beforeScalePoint * scaleFactor_;
    offset_ = (mousePos - afterScalePoint.toPoint());
    update();
}

void ImageViewer::centerImage()
{

    // 获取父控件的尺寸（QFrame），如果已经在 showEvent 中获取并保存了父控件尺寸
    int parentWidth = parent_width_;   // 从 showEvent 中保存的父控件宽度
    int parentHeight = parent_height_; // 从 showEvent 中保存的父控件高度

    if (parentWidth <= 0 || parentHeight <= 0)
    {
        qDebug() << "Invalid parent widget size.";
        return;
    }
    normalize();
    // 计算缩放后的图像宽度和高度
    int imageWidth = image_.cols * scaleFactor_;
    int imageHeight = image_.rows * scaleFactor_;

    // 计算偏移量，使图像居中
    int offsetX = (parentWidth - imageWidth) / 2;
    int offsetY = (parentHeight - imageHeight) / 2;

    offset_ = QPoint(offsetX, offsetY);

    // 刷新界面，触发重新绘制
    update();
}


cv::Mat ImageViewer::getImage() const
{
    return image_;
}

void ImageViewer::resizeEvent(QResizeEvent  *event)
{
    QWidget::resizeEvent(event); // 调用基类的showEvent

    // 获取父控件的尺寸（QFrame）
    QWidget *parentWidget = this->parentWidget();
    if (!parentWidget || parentWidget->width() <= 0 || parentWidget->height() <= 0)
    {
        qDebug() << "Waiting for parent widget to be ready...";
        QEventLoop loop;
        QTimer timer;
        connect(&timer, &QTimer::timeout, [&]() {
            if (parentWidget && parentWidget->width() > 0 && parentWidget->height() > 0)
            {
                parent_width_ = parentWidget->width();
                parent_height_ = parentWidget->height();
                loop.quit();
            }

        });
        timer.start(50);  // 每 50ms 检查一次
        loop.exec();  // 阻塞直到 parentWidget() 尺寸有效
    }
}






void ImageViewer::updateImageDisplay()
{
    if (currentImageIndex_ >= 0 && currentImageIndex_ < images_.size())
    {
        const cv::Mat &mat = images_.at(currentImageIndex_);
        setImage(mat);
        centerImage();

    }
}
void ImageViewer::loadImages(const QStringList &filePaths)
{
    images_.clear();             // 清空之前的图像
    currentImageIndex_ = -1;     // 重置索引


    for (const QString &filePath : filePaths)
    {
        cv::Mat img = cv::imread(filePath.toStdString());
        if (!img.empty())
        {
            images_.push_back(img); // 加载成功则保存
        }
        else
        {
            qDebug() << "Failed to load image:" << filePath;
        }
    }

    if (!images_.empty())
    {
        currentImageIndex_ = 0;  // 默认显示第一张图像
        updateImageDisplay();
    }
}
void ImageViewer::nextImage()
{
    if (currentImageIndex_ >= 0 && currentImageIndex_ < images_.size() - 1)
    {
        currentImageIndex_++;
        updateImageDisplay();
    }
    else
    {
        currentImageIndex_ = 0;
         updateImageDisplay();
    }
}

void ImageViewer::prevImage()
{
    if (currentImageIndex_ > 0)
    {
        currentImageIndex_--;
        updateImageDisplay();
    }
    else
    {
        currentImageIndex_ =images_.size()-1;
        updateImageDisplay();
    }
}
void ImageViewer::normalize()
{
    if (parent_width_ > 0 && parent_height_ > 0)
    {
        double widthRatio = static_cast<double>(parent_width_) / image_.cols;
        double heightRatio = static_cast<double>(parent_height_) / image_.rows;
        scaleFactor_ = std::min(widthRatio, heightRatio); // 确保图片适应窗口，不超出范围
    }
}

void ImageViewer::setMode(PaintMode mode)
{
    paintmode_ = mode;
}
void ImageViewer::drawFreeLine(QPoint point)
{
    if (lastPos_.isNull()) {
        lastPos_ = point;
        return;
    }
    point = (point - offset_) / scaleFactor_;
    cv::line(overlay_, cv::Point(lastPos_.x(), lastPos_.y()),
             cv::Point(point.x(), point.y()), cv::Scalar(255, 255, 0,255), 2, cv::LINE_AA);

    lastPos_ = point;
    //updateImageDisplay();
    update();
}


void ImageViewer::drawRectangle(QPoint start, QPoint end)
{

    cv::rectangle(overlay_, cv::Rect(cv::Point(start.x(),start.y()), cv::Point(end.x(),end.y())), cv::Scalar(255, 255, 0,255), 2);
    //updateImageDisplay();
    update();
}


void ImageViewer::drawEllipse(QPoint start, QPoint end)
{
    // 将 QPoint 转换为 cv::Point
    cv::Point pt1(start.x(), start.y());
    cv::Point pt2(end.x(), end.y());

    // 计算椭圆的中心和轴长
    cv::Point center((pt1.x + pt2.x) / 2, (pt1.y + pt2.y) / 2);
    int axisX = std::abs(pt1.x - pt2.x) / 2;
    int axisY = std::abs(pt1.y - pt2.y) / 2;

    // 绘制椭圆
    int thickness = 2; // 设置线条粗细
    cv::ellipse(overlay_, center, cv::Size(axisX, axisY), 0, 0, 360, cv::Scalar(255, 255, 0,255), thickness);

    // 更新图像显示
    //updateImageDisplay();
    update(); // 重新绘制窗口
}



void ImageViewer::drawLine(QPoint start, QPoint end)
{
    cv::line(overlay_, cv::Point(start.x(), start.y()),
             cv::Point(end.x(), end.y()), cv::Scalar(255, 255, 0,255), 2, cv::LINE_AA);
    //updateImageDisplay();

    update();
}

void ImageViewer::erase(QPoint pos, int size)
{
    pos = (pos - offset_) / scaleFactor_;
    cv::circle(overlay_, cv::Point(pos.x(), pos.y()), size, cv::Scalar(255, 255, 255,0), -1);  // 用白色（255, 255, 255）模拟擦除
    update();  // 更新视图
    //updateImageDisplay();
    update();
}
