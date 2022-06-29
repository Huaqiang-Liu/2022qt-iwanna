#include "exitwindow.h"
#include "ui_exitwindow.h"
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
Exitwindow::Exitwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Exitwindow)
{
    ui->setupUi(this);

    // 把传过来的父窗口类的指针强制类型转换
    m_parent = static_cast<MainWindow*>(parent);//目前不知为啥这么干
    //m_parent->ui->exit_button->hide();
}

Exitwindow::~Exitwindow()
{

    delete ui;
}

void Exitwindow::on_pushButton_clicked()
{
    extern bool paint_what;
    paint_what = 1;
    update();


    m_parent->ui->help->setChecked(0);
    m_parent->ui->help->setVisible(1);


    m_parent->ui->chapter1->setChecked(0);
    m_parent->ui->chapter1->setVisible(1);
    m_parent->ui->chapter2->setChecked(0);
    m_parent->ui->chapter2->setVisible(1);
    m_parent->ui->chapter3->setChecked(0);
    m_parent->ui->chapter3->setVisible(1);
    m_parent->ui->chapter4->setChecked(0);
    m_parent->ui->chapter4->setVisible(1);
    m_parent->ui->chapter5->setChecked(0);
    m_parent->ui->chapter5->setVisible(1);//让被隐藏的按钮回来
    m_parent->ui->exit_button->hide();
    m_parent->ui->label->hide();
    close();
}


void Exitwindow::on_pushButton_2_clicked()
{
    paint_what = 0;
    m_parent->ui->exit_button->setVisible(1);
    close();
}

