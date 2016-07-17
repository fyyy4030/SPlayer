#include "media.h"

Media::Media(QWidget * parent = 0)
{
    //����ý��ͼ
    mediaObject = new Phonon::MediaObject(parent);
    audioOutput = new Phonon::AudioOutput(Phonon::VideoCategory,parent);
    videoWidget = new Phonon::VideoWidget(parent);

    playlist = new VideoList();

    //��������
    Phonon::createPath(mediaObject, videoWidget);
    Phonon::createPath(mediaObject, audioOutput);

    //aboutFinish
    //��widget����������
    //connect(mediaObject, SIGNAL(aboutToFinish()), this, SLOT(readyToPlay()));
}

Media::~Media()
{
    //delete mediaObject;
    //delete audioOutput;
    //delete videoWidget;
    //delete playlist;
}

void Media::setPath(QString fileName)
{
    mediaObject->setCurrentSource(Phonon::MediaSource(fileName));
}

void Media::readyToPlay()
{
    //next
    //current+
    //�������

    if (playlist->next())
    {
        mediaObject->enqueue(Phonon::MediaSource(playlist->getCurrentItem()));
    }

    //�������һ��
    //mediaObject->stop();
    //��������
    //mediaObject->play();
}


//��ͣ����
void Media::pause()
{
    mediaObject->pause();
}

//��ʼ����
void Media::play()
{
    mediaObject->play();
}

//ֹͣ����
void Media::stop()
{
    mediaObject->stop();
}

//�������valueʱ��
void Media::goFoward(qint64 value)
{
    if (value <= mediaObject->remainingTime())
    {
        mediaObject->seek(mediaObject->currentTime() + value);
    }
    else
    {
        mediaObject->seek(mediaObject->totalTime());
    }
}

//���˲���valueʱ��
void Media::goBack(qint64 value)
{
    if (value <= mediaObject->currentTime())
    {
        mediaObject->seek(mediaObject->currentTime() - value);
    }
    else
    {
        mediaObject->seek(0);
    }
}

//���ٶ�λ��valueλ�ò���
void Media::locate(qint64 value)
{
    mediaObject->seek(value);
}

//ȫ������,trueΪȫ��,falseΪ��ȫ��
void Media::setFullScreen(bool flag)
{
    videoWidget->setFullScreen(flag);
}

//�˳�ȫ��
void Media::exitFullScreen()
{
    videoWidget->exitFullScreen();
}

//���öԱȶ�
void Media::setContrast(qint8 value)
{
    videoWidget->setContrast(value / 10.0);
}

//���ñ��Ͷ�
void Media::setSaturation(qint8 value)
{
    videoWidget->setSaturation(value / 10.0);
}

//��������
void Media::setBrightness(qint8 value)
{
    videoWidget->setBrightness(value / 10.0);
}

//����ɫ��
void Media::setHue(qint8 value)
{
    videoWidget->setBrightness(value / 10.0);
}

//���÷ֱ���
void Media:: setAspectRatio(Phonon::VideoWidget::AspectRatio value)
{
    videoWidget->setAspectRatio(value);
}

//��ȡ��ǰʱ��
qint64 Media::currentTime()
{
    return mediaObject->currentTime();
}

