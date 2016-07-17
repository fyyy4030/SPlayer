#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>
namespace Ui {
    class Setting;
}

class Setting : public QWidget
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = 0);
    ~Setting();

private slots:
    void on_buttonOk_clicked();

    void on_buttonCancel_clicked();
signals:
    void settingHaveChanged(int, int, int, int, int, int);

private:
    Ui::Setting *ui;
    int setContrast; //�Աȶ�
    int setSaturation;//���Ͷ�
    int setBrightness;//��������
    int setHue; //����ɫ��
    int step;//������˲���
    int numGif;//gifͼƬ����
};

#endif // SETTING_H
