#ifndef ROIOPTIONBOX_H
#define ROIOPTIONBOX_H

#include <QWidget>

class ROIOptionBox : public QWidget
{
    Q_OBJECT

public:
    explicit ROIOptionBox(QWidget *parent = nullptr);
    QSize sizeHint() const override { return QSize(220, 120); }

signals:
    void roiSelected(int roiIndex);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    int selectedROI;
};

#endif // ROIOPTIONBOX_H
