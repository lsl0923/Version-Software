#ifndef FORM_H
#define FORM_H
#include<map>
#include <QWidget>
#include<QMainWindow>
#include"ImageViewer.h"

#include"ToolsFlow.h"
#include"QListWidget"
#include"managerbase.h"
#include"toolitem.h"
#include"CameraManager.h"
namespace Ui
{
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();
    void loadPlugin(const QString& path);
    void loadPluginC(const QString& path);
    void onActionOpenUSB();
    void onActionOpenImg();
    void onActionSaveImg();
    void showMessage(const QString &text);

public slots:
    void updateImage(cv::Mat img);
private slots:

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2__clicked();

    void on_RunWidget_itemChanged(QListWidgetItem *item);

    void setInputMenu(const std::string& toolId, QMenu* inputMenu);
    void on_ToolsWight_doubleClicked(const QModelIndex &index);
    void onRightClick(const QPoint& pos);
    void on_LoadImage_clicked();

    void on_nextImage_clicked();
    void showInputConfigDialog(ToolItem* toolItem);

   // void on_pushButton_6_clicked();

    void on_pen_clicked();

    void on_mouse_clicked();

    void on_rect_clicked();

    void on_ellips_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

protected:
    //void dropEvent(QDropEvent *event) override;
private:
    Ui::Form *ui;
    ImageViewer* imageViewer_; // 图像查看控件

    std::map<ToolsInfo, ManagerBase*> toolsMap_;


    std::vector<ToolsFlow> toolsFlows_;
    std::shared_ptr<CameraManager>  cameraManager_;


    bool isExistsInToolsWight(const std::string toolName);
};

#endif // FORM_H
