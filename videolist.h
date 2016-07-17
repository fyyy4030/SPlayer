#ifndef VIDEOLIST_H
#define VIDEOLIST_H

#include <QDebug>
#include <List>
#include <QTextCodec>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>

class VideoList
{
public:
    VideoList();
    ~VideoList();

    //��ն���
    void clearList();

    //����������,�����ļ���
    QString addFile(QString);

    //��ȡ�ļ���
    QString getFileName(QString);

    //�Ӷ���ɾ��
    void deleteFile(int);

    //��һ��
    bool next();

    //��һ��
    bool last();


    //���ļ�����Ŀ¼
    void openDir(int);

    //����Ҫ���ŵ���
    void setCurrentItem(int);

    //��ȡҪ�������URL
    QString getCurrentItem();

    //��ȡ��row����ļ�
    QString getItem(int);

    //��ȡ��ǰ���ŵ����
    int getCurrentItemNum();

    //���ļ���ȡ��Ϣ
    void readFromFile();

    //д����Ϣ���ļ�
    void writeToFile();

    //���ز��Ŷ���
    QList<QString> getList();

private:
    //Ŀǰ���ŵ����
    int currentItem;

    //���Ŷ���
    QList<QString> LIST;
};

#endif // VIDEOLIST_H
