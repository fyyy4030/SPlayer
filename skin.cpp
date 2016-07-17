#include "skin.h"
#include "ui_skin.h"
#include <QFile>

skin::skin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::skin)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowModality(Qt::ApplicationModal);
}

skin::~skin()
{
    delete ui;
}

void skin::on_toolButton_clicked()
{
    QFile file(":/qss/my.qss");
    // ֻ����ʽ�򿪸��ļ�
    file.open(QFile::ReadOnly);
    // ��ȡ�ļ�ȫ�����ݣ�ʹ��tr()��������ת��ΪQString����
    QString styleSheet = tr(file.readAll());
    // ΪQApplication������ʽ��
    qApp->setStyleSheet(styleSheet);
}

void skin::on_toolButton_2_clicked()
{
    QFile file(":/qss/my1.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = tr(file.readAll());
    qApp->setStyleSheet(styleSheet);
}

void skin::on_toolButton_3_clicked()
{
    QFile file(":/qss/my2.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = tr(file.readAll());
    qApp->setStyleSheet(styleSheet);
}

void skin::on_toolButton_4_clicked()
{
    QFile file(":/qss/my3.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = tr(file.readAll());
    qApp->setStyleSheet(styleSheet);
}

void skin::on_toolButton_5_clicked()
{
    QFile file(":/qss/my4.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = tr(file.readAll());
    qApp->setStyleSheet(styleSheet);
}

void skin::on_toolButton_6_clicked()
{
    QFile file(":/qss/my5.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = tr(file.readAll());
    qApp->setStyleSheet(styleSheet);
}
