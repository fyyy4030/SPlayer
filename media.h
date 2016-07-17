#ifndef MEDIA_H
#define MEDIA_H

#include <QWidget>
#include <QAction>
#include <phonon>
#include <QDebug>
#include "videolist.h"

class Media
{
public:
    Media(QWidget * parent);
    ~Media();

    //����·��
    void setPath(QString path);

    //�������valueʱ��
    void goFoward(qint64 value);

    //���˲���valueʱ��
    void goBack(qint64 value);

    //MediaObject AudioObject
     Phonon::MediaObject *mediaObject;
     Phonon::AudioOutput *audioOutput;
     Phonon::VideoWidget *videoWidget;
     VideoList * playlist;

 public slots:

     //�ۺ�����aboutfinish����
     void readyToPlay();

private:
    //ȫ������,trueΪȫ��,falseΪ��ȫ
    void setFullScreen(bool);

    //�˳�ȫ��
    void exitFullScreen();

    //��ȡ��ǰ����ʱ��
    qint64 currentTime();

    //��ͣ����
    void pause();

    //��ʼ����
    void play();

    //ֹͣ����
    void stop();

    //���ٶ�λ��valueλ�ò���
    void locate(qint64 value);

    //���öԱȶ�
    void setContrast(qint8 value);

    //���ñ��Ͷ�
    void setSaturation(qint8 value);

    //��������
    void setBrightness(qint8 value);

    //����ɫ��
    void setHue(qint8 value);

    //���ÿ�߱�
    void setAspectRatio(Phonon::VideoWidget::AspectRatio);
};

#endif // MEDIA_H
