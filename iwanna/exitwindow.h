#ifndef EXITWINDOW_H
#define EXITWINDOW_H
extern bool paint_what;//0则画游戏界面背景，1则画菜单背景
extern int choice;//选择关卡，在下面的绘图事件中
#include <QDialog>

class MainWindow;//子窗口中操纵主窗口，要声明！！！！

namespace Ui {
class Exitwindow;
}

class Exitwindow : public QDialog
{
    Q_OBJECT

public:
    explicit Exitwindow(QWidget *parent = nullptr);
    ~Exitwindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Exitwindow *ui;
    MainWindow *m_parent;// 主窗口类的对象!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

};

#endif // EXITWINDOW_H
