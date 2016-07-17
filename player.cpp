#include "player.h"
#include "ui_player.h"
#include "skin.h"
#include "gif.h"
#include <QFile>
#include <QDesktopWidget>
player::player(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::player)
{
    ui->setupUi(this);
    QFile file(":/qss/my1.qss");
    // ֻ����ʽ�򿪸��ļ�
    file.open(QFile::ReadOnly);
    // ��ȡ�ļ�ȫ�����ݣ�ʹ��tr()��������ת��ΪQString����
    QString styleSheet = tr(file.readAll());
    // ΪQApplication������ʽ��
    qApp->setStyleSheet(styleSheet);

    // ���ô���
    setting = new Setting();
    connect(setting, SIGNAL(settingHaveChanged(int,int,int,int,int,int)), this, SLOT(setSetting(int,int,int,int,int,int)));
    step = 5;
    numGif = 10;

    //����Ϊ���Խ��������¼�
    setAcceptDrops(true);

    //����Ϊ֧�ִ����϶��¼�
    setMouseTracking(true);

    // ���ñ���
    QPixmap pixmap = QPixmap(":/pic/bg.PNG").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(palette);

    // �������ý��
    media = new Media(this);
    // ע���¼�������
    media->videoWidget->installEventFilter(this);
    //mediaObject = new Phonon::MediaObject();
    //videoWidget = new Phonon::VideoWidget();
    //videoWidget->setWindowFlags(Qt::SubWindow);
    //Phonon::createPath(media->mediaObject, media->videoWidget);
    //audioOutput = new Phonon::AudioOutput(Phonon::VideoCategory);
    //Phonon::createPath(media->mediaObject, media->audioOutput);
    connect(media->mediaObject, SIGNAL(aboutToFinish()), this, SLOT(playContine()));
    connect(media->mediaObject, SIGNAL(totalTimeChanged(qint64)), this, SLOT(setTotalTime(qint64)));
    connect(media->mediaObject, SIGNAL(tick(qint64)), this, SLOT(setCurrentTime(qint64)));
    connect(media->mediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)), this, SLOT(setBackgroundList()));

    // �������Ž��Ȼ���
    Phonon::SeekSlider *seekSlider = new Phonon::SeekSlider(media->mediaObject);

    // �����������ڻ���
    Phonon::VolumeSlider *volumeSlider = new Phonon::VolumeSlider(media->audioOutput);

    // ������Ƶʱ����
    currentTime = new QLabel(tr("00:00"));
    QLabel *separator = new QLabel(tr("/"));
    totalTime = new QLabel(tr("00:00"));

    // ��������б�
    //QTabWidget *tabW = new QTabWidget();
    videoList = new QListWidget();
    //tabW->addTab(videoList,tr("���ڲ���"));
    videoList->setContextMenuPolicy( Qt::CustomContextMenu );
    connect(videoList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_videoList_customContextMenuRequested(QPoint)));
    initList();

    // �����label
    QLabel *label = new QLabel(tr("�����б�"));

    // ������ť
    QIcon icon_play;
    icon_play.addFile(":/image/img/play.png");
    buttonPlay = new QPushButton();
    buttonPlay->setStyleSheet("QPushButton{border:0px;}");
    buttonPlay->setIcon(icon_play);
    connect(buttonPlay, SIGNAL(clicked(bool)), this, SLOT(onClickedButtonPlay()));
    setPlayState(true);

    QIcon icon_stop;
    icon_stop.addFile(":/image/img/stop.png");
    buttonStop = new QPushButton();
    buttonStop->setStyleSheet("QPushButton{border:0px;}");
    buttonStop->setIcon(icon_stop);
    connect(buttonStop, SIGNAL(clicked()), this, SLOT(onClickedButtonStop()));

    QIcon icon_go_farword;
    icon_go_farword.addFile(":/image/img/goforward.png");
    buttonGoFarword = new QPushButton();
    buttonGoFarword->setStyleSheet("QPushButton{border:0px;}");
    buttonGoFarword->setIcon(icon_go_farword);
    connect(buttonGoFarword, SIGNAL(clicked()), this, SLOT(onClickedButtonGoFarword()));

    QIcon icon_draw_back;
    icon_draw_back.addFile(":/image/img/drawback.png");
    buttonDrawBack = new QPushButton();
    buttonDrawBack->setStyleSheet("QPushButton{border:0px;}");
    buttonDrawBack->setIcon(icon_draw_back);
    connect(buttonDrawBack, SIGNAL(clicked()), this, SLOT(onClickedButtonDrawBack()));

    QIcon icon_next;
    icon_next.addFile(":/image/img/next.png");
    buttonNext = new QPushButton();
    buttonNext->setStyleSheet("QPushButton{border:0px;}");
    buttonNext->setIcon(icon_next);
    connect(buttonNext, SIGNAL(clicked()), this, SLOT(onClickedButtonNext()));

    QIcon icon_last;
    icon_last.addFile(":/image/img/last.png");
    buttonLast = new QPushButton();
    buttonLast->setStyleSheet("QPushButton{border:0px;}");
    buttonLast->setIcon(icon_last);
    connect(buttonLast, SIGNAL(clicked()), this, SLOT(onClickedButtonLast()));

    QIcon icon_full;
    icon_full.addFile(":/image/img/screen.png");
    buttonFullScreen = new QPushButton();
    buttonFullScreen->setStyleSheet("QPushButton{border:0px;}");
    buttonFullScreen->setIcon(icon_full);
    connect(buttonFullScreen, SIGNAL(clicked()), this, SLOT(onClickedButtonScreen()));
    setScreenState(true);

    QIcon icon_hide;
    icon_hide.addFile(":/image/img/maximize.png");
    buttonHide = new QPushButton();
    buttonHide->setStyleSheet("QPushButton{border:0px;}");
    buttonHide->setIcon(icon_hide);
    //connect(buttonHide,SIGNAL(clicked()),this,SLOT(onClickedButtonHide()));
    setHideState(true);

    QIcon icon_shot;
    icon_shot.addFile(":/image/img/shot.png");
    buttonShot = new QPushButton();
    buttonShot->setStyleSheet("QPushButton{border:0px;}");
    buttonShot->setIcon(icon_shot);
    connect(buttonShot, SIGNAL(clicked()), this, SLOT(onClickedButtonShot()));

    QIcon icon_gif;
    icon_gif.addFile(":/image/img/gif.png");
    buttonGif = new QPushButton();
    buttonGif->setStyleSheet("QPushButton{border:0px;}");
    buttonGif->setIcon(icon_gif);
    connect(buttonGif, SIGNAL(clicked()), this, SLOT(onClickedButtonGif()));

    QIcon icon_close;
    icon_close.addFile(":/image/img/close.png");
    buttonClose = new QPushButton();
    buttonClose->setStyleSheet("QPushButton{border:0px;}");
    buttonClose->setIcon(icon_close);
    connect(buttonClose,SIGNAL(clicked()),this,SLOT(onClickedButtonClose()));

    QIcon icon_maximize;
    icon_maximize.addFile(":/image/img/maximize.png");
    buttonMaximize = new QPushButton();
    buttonMaximize->setStyleSheet("QPushButton{border:0px;}");
    buttonMaximize->setIcon(icon_maximize);
    connect(buttonMaximize,SIGNAL(clicked()),this,SLOT(onClickedButtonMaximize()));

    QIcon icon_minimize;
    icon_minimize.addFile(":/image/img/minimize.png");
    buttonMinimize = new QPushButton();
    buttonMinimize->setStyleSheet("QPushButton{border:0px;}");
    buttonMinimize->setIcon(icon_minimize);
    connect(buttonMinimize,SIGNAL(clicked()),this,SLOT(onClickedButtonMinimize()));

    QIcon icon_set;
    icon_set.addFile(":/image/img/set.png");
    buttonSet = new QPushButton();
    buttonSet->setStyleSheet("QPushButton{border:0px;}");
    buttonSet->setIcon(icon_set);
    connect(buttonSet,SIGNAL(clicked()),this,SLOT(onClickedButtonSet()));

    QIcon icon_file;
    icon_file.addFile(":/image/img/file.png");
    buttonFile = new QPushButton();
    buttonFile->setStyleSheet("QPushButton{border:0px;}");
    buttonFile->setIcon(icon_file);
    connect(buttonFile,SIGNAL(clicked()),this,SLOT(onClickedButtonFile()));

    QIcon icon_skin;
    icon_skin.addFile(":/image/img/skin.png");
    buttonSkin = new QPushButton();
    buttonSkin->setStyleSheet("QPushButton{border:0px;}");
    buttonSkin->setIcon(icon_skin);
    connect(buttonSkin,SIGNAL(clicked()),this,SLOT(onClickedButtonSkin()));


    // ���ò��ֲ���
    media->videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    volumeSlider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    seekSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    videoList->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    buttonPlay->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonStop->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonGoFarword->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonDrawBack->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonFullScreen->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonShot->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    currentTime->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    totalTime->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    separator->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    buttonClose->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonMaximize->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonMinimize->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonSet->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonFile->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonSkin->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonNext->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonLast->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonHide->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonGif->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // ȥ������
    media->videoWidget->setFocusPolicy(Qt::NoFocus);
    volumeSlider->setFocusPolicy(Qt::NoFocus);
    seekSlider->setFocusPolicy(Qt::NoFocus);
    videoList->setFocusPolicy(Qt::NoFocus);
    buttonPlay->setFocusPolicy(Qt::NoFocus);
    buttonStop->setFocusPolicy(Qt::NoFocus);
    buttonGoFarword->setFocusPolicy(Qt::NoFocus);
    buttonDrawBack->setFocusPolicy(Qt::NoFocus);
    buttonFullScreen->setFocusPolicy(Qt::NoFocus);
    buttonShot->setFocusPolicy(Qt::NoFocus);
    currentTime->setFocusPolicy(Qt::NoFocus);
    totalTime->setFocusPolicy(Qt::NoFocus);
    separator->setFocusPolicy(Qt::NoFocus);
    buttonClose->setFocusPolicy(Qt::NoFocus);
    buttonMaximize->setFocusPolicy(Qt::NoFocus);
    buttonMinimize->setFocusPolicy(Qt::NoFocus);
    buttonSet->setFocusPolicy(Qt::NoFocus);
    buttonFile->setFocusPolicy(Qt::NoFocus);
    buttonSkin->setFocusPolicy(Qt::NoFocus);
    buttonNext->setFocusPolicy(Qt::NoFocus);
    buttonLast->setFocusPolicy(Qt::NoFocus);
    buttonHide->setFocusPolicy(Qt::NoFocus);
    buttonGif->setFocusPolicy(Qt::NoFocus);
    label->setFocusPolicy(Qt::NoFocus);

    // ��Ӳ��ֹ�����
    QGridLayout *mLayout = new QGridLayout();
    QHBoxLayout *timeLayout = new QHBoxLayout();

    timeLayout->addWidget(currentTime);
    timeLayout->addWidget(separator);
    timeLayout->addWidget(totalTime);


    mLayout->addWidget(buttonFile,0,28,1,1);
    mLayout->addWidget(buttonSkin,0,29,1,1);
    mLayout->addWidget(buttonSet,0,30,1,1);
    mLayout->addWidget(buttonMinimize,0,31,1,1);
    mLayout->addWidget(buttonMaximize,0,32,1,1);
    mLayout->addWidget(buttonClose,0,33,1,1);

    mLayout->addWidget(media->videoWidget,1,0,16,34);
    mLayout->addWidget(buttonDrawBack,17,0,1,1);
    mLayout->addWidget(seekSlider,17,1,1,29);
    mLayout->addWidget(buttonGoFarword,17,30,1,1);
    mLayout->addWidget(buttonPlay,18,0,1,1);
    mLayout->addWidget(buttonStop,18,1,1,1);
    mLayout->addWidget(buttonLast,18,2,1,1);
    mLayout->addWidget(buttonNext,18,3,1,1);
    mLayout->addWidget(buttonFullScreen,18,33,1,1);
    mLayout->addWidget(buttonShot,18,4,1,1);
    mLayout->addWidget(buttonGif,18,5,1,1);
    mLayout->addWidget(volumeSlider,18,6,1,14);
    //mLayout->addWidget(buttonHide,18,33,1,1);
    //mLayout->addWidget(label,1,34,1,1);
    //mLayout->addWidget(videoList,2,34,16,6);
    mLayout->addLayout(timeLayout,17,31,1,3);

    //���岼�֣��۵������
    resize(800, 600);
    splitter = new QSplitter(Qt::Horizontal,this);
    splitter->setHandleWidth(1);
    splitter->setStyleSheet("QSplitter::handle{background:#FFFFFF}");

    contentFrame = new QWidget(splitter);
    contentFrame->setLayout(mLayout);
    contentFrame->resize(600, height());
    contentFrame->setMinimumWidth(600);

    QGridLayout *listLayout=new QGridLayout();
    listLayout->addWidget(label,1,0,1,1);
    listLayout->addWidget(videoList,2,0,16,6);
    listFrame = new QWidget(splitter);
    listFrame->setLayout(listLayout);
    listFrame->resize(200, height());
    listFrame->setMaximumWidth(200);

    connect(splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(slotSplitterMoved(int,int)));

    leftIcon = QIcon(":/image/img/hide.png");
    rightIcon = QIcon(":/image/img/show.png");

    pushButton = new QPushButton(this);
    pushButton->setFocusPolicy(Qt::NoFocus);
    pushButton->hide();
    pushButton->setFixedSize(40, 40);
    pushButton->setIconSize(pushButton->size());
    pushButton->setStyleSheet("border:none;");
    pushButton->setIcon(rightIcon);
    pushButton->move(contentFrame->width() - pushButton->width(), (contentFrame->height() - pushButton->height())/2);
    connect(pushButton,SIGNAL(clicked()),this,SLOT(slotClickedBtn()));


    contentFrame->setMouseTracking(true);
    listFrame->setMouseTracking(true);
    contentFrame->installEventFilter(this);
    listFrame->installEventFilter(this);



    //setLayout(mLayout);
}

player::~player()
{
    //media->playlist->~VideoList();
    /*
    delete buttonPlay;
    delete buttonStop;
    delete buttonGoFarword;
    delete buttonDrawBack;
    delete buttonFullScreen;
    delete buttonHide;
    delete buttonShot;
    delete buttonClose;
    delete buttonMaximize;
    delete buttonMinimize;
    delete buttonSet;
    delete buttonFile;
    delete buttonSkin;
    delete buttonNext;
    delete buttonLast;
    delete buttonGif;
    delete pushButton;
    */

    //delete currentTime;
    //delete totalTime;

    //delete splitter;
    //delete contentFrame;
    //delete listFrame;



    //delete videoList;


    media->playlist->~VideoList();
    //delete media;
    delete ui;
}

void player::onClickedButtonClose()
{
    media->playlist->~VideoList();
    if (!this->close()) qDebug() << "close error"; qDebug() << "close succssed";
    //this->~player();
}

void player::onClickedButtonFile()
{
    QIcon icon_play;
    QString fileNamePath = QFileDialog::getOpenFileName(
                this,
                tr("���ļ�"),
                "..",
                tr("Movie(*.avi *.rmvb *.flv *.wmv *mp4);;Music(*.mp3 *.wmv)"));
    //media->mediaObject->setCurrentSource(Phonon::MediaSource(fileNamepath));
    //media->mediaObject->play();
    /*
    if (fileNamePath.length() != 0)
    {
        //�������б�Ͷ�������
        videoList->addItem(media->playlist->addFile(fileNamePath));
        //���õ�ǰ���ŵ���Ƶ
        media->playlist->setCurrentItem(videoList->count() - 1);
        media->mediaObject->setCurrentSource(Phonon::MediaSource(media->playlist->getCurrentItem()));
        media->mediaObject->play();
        //setTotalTime();
        icon_play.addFile(":/image/img/pause.png");
        buttonPlay->setStyleSheet("QPushButton{border:0px;}");
        buttonPlay->setIcon(icon_play);
        setPlayState(false);
    }*/
    if (fileNamePath.length() != 0)
    {
        //�������б�Ͷ�������
        int index = media->playlist->getList().indexOf(fileNamePath);
        if ( index == -1)
        {
            videoList->addItem(media->playlist->addFile(fileNamePath));
            media->playlist->setCurrentItem(videoList->count() - 1);
        }
        else
        {
            media->playlist->setCurrentItem(index);
        }
        //���õ�ǰ���ŵ���Ƶ
        media->mediaObject->setCurrentSource(Phonon::MediaSource(media->playlist->getCurrentItem()));
        media->mediaObject->play();
        //setTotalTime();
        icon_play.addFile(":/image/img/pause.png");
        buttonPlay->setStyleSheet("QPushButton{border:0px;}");
        buttonPlay->setIcon(icon_play);
        setPlayState(false);
    }
}

void player::onClickedButtonMaximize()
{
    /*
    Qt::WindowNoState           0x00000000	The window has no state set (in normal state).
    Qt::WindowMinimized         0x00000001	The window is minimized (i.e. iconified).
    Qt::WindowMaximized         0x00000002	The window is maximized with a frame around it.
    Qt::WindowFullScreen	0x00000004	The window fills the entire screen without any frame around it.
    Qt::WindowActive            0x00000008	The window is the active window, i.e. it has keyboard focus.
    */
    if(windowState()!= 0x00000002)
    {
        setWindowState(Qt::WindowMaximized);
    }
    else
    {
        //setWindowState(Qt::WindowActive);
        this->showNormal();
    }
}

//////////////////////////////////////////////////////�б�����/////////////////////////////////////////////////////////
void player::playContine()
{
    //next
    //current+
    //�������

    if (media->playlist->next())
    {
        media->mediaObject->enqueue(Phonon::MediaSource(media->playlist->getCurrentItem()));
    }

    //�������һ��
    //next
    //mediaObject->setcurrentSource
    //��������
    //mediaObject->play();
}

void player::initList()
{
    QList<QString> LIST = media->playlist->getList();
    for (int i = 0; i < LIST.size(); i++)
    {
         videoList->addItem(media->playlist->getFileName(LIST.at(i)));
    }
}

void player::playList()
{
    QIcon icon_play;
    media->playlist->setCurrentItem(videoList->currentRow());
    media->mediaObject->setCurrentSource(Phonon::MediaSource(media->playlist->getCurrentItem()));
    media->mediaObject->play();
    icon_play.addFile(":/image/img/pause.png");
    buttonPlay->setStyleSheet("QPushButton{border:0px;}");
    buttonPlay->setIcon(icon_play);
    setPlayState(false);
}

void player::addList()
{
    QString fileNamePath = QFileDialog::getOpenFileName(
                this,
                tr("���ļ�"),
                "..",
                tr("Movie(*.avi *.rmvb *.flv *.wmv *mp4);;Music(*.mp3 *.wmv)"));
    if (fileNamePath.length() != 0 && media->playlist->getList().indexOf(fileNamePath) == -1)
    {
        videoList->addItem(media->playlist->addFile(fileNamePath));
    }
}

void player::deleteList()
{
    //��Ҫ�жϺ͵�ǰ���ŵ��ǲ���һ��
    if (media->mediaObject->hasVideo() && (media->playlist->getCurrentItemNum() == videoList->currentRow()))
    {
        //��������
        onClickedButtonStop();
    }
    //qDebug() << "source" << media->playlist->getItem(videoList->currentRow());
    //qDebug() << "source" << media->mediaObject->currentSource();

    media->playlist->deleteFile(videoList->currentRow());
    videoList->takeItem(videoList->currentRow());
}

void player::insertList()
{
    /*
    QString path = QFileDialog::getOpenFileName(this, tr("Open Video"),".",tr("Movie(*.avi *.rmvb *.flv *.wmv *mp4);;"));
    if (path.length() != 0)
    {
        ui->videoWidget->insertItem(ui->videoWidget->currentRow() + 1, path);
    }
    */
}

void player::openDirList()
{
    media->playlist->openDir(videoList->currentRow());
}

void player::setBackgroundList()
{
    for (int i = 0; i < videoList->count(); i++)
    {
        videoList->item(i)->setBackground(QBrush(QColor(0, 0, 0, 0)));
    }
    videoList->item(media->playlist->getCurrentItemNum())->setBackground(QBrush(QColor(255, 255, 0, 180)));
}

void player::clearList()
{
    //��������
    onClickedButtonStop();
    videoList->clear();
    media->playlist->clearList();
}

void player::on_videoList_customContextMenuRequested(const QPoint &pos)
{
    QMenu *popMenu =new QMenu(this);//����һ���Ҽ������˵�

    QAction *playfileAction = new QAction(tr("&����ѡ����"), this);
    popMenu->addAction(playfileAction);
    connect(playfileAction, SIGNAL(triggered()), this, SLOT(playList()));

    QAction *addfileAction = new QAction(tr("&����ļ����б�"), this);
    popMenu->addAction(addfileAction);
    connect(addfileAction, SIGNAL(triggered()), this, SLOT(addList()));

    QAction *deletefileAction = new QAction(tr("&�Ӳ����б�ɾ��"), this);
    popMenu->addAction(deletefileAction);
    connect(deletefileAction, SIGNAL(triggered()), this, SLOT(deleteList()));

    QAction *clearlistAction = new QAction(tr("&��ղ����б�"), this);
    popMenu->addAction(clearlistAction);
    connect(clearlistAction, SIGNAL(triggered()), this, SLOT(clearList()));

    QAction *opendirAction = new QAction(tr("&���ļ�Ŀ¼"), this);
    popMenu->addAction(opendirAction);
    connect(opendirAction, SIGNAL(triggered()), this, SLOT(openDirList()));

    QListWidgetItem* curItem=videoList->itemAt(pos);

    if(curItem==NULL)
    {
        //������Ч
        playfileAction->setEnabled(false);
        deletefileAction->setEnabled(false);
        opendirAction->setEnabled(false);
    }
    popMenu->exec(QCursor::pos());//�����Ҽ��˵����˵�λ��Ϊ���λ��
}

/////////////////////////////////////////////�б�����///////////////////////////////////////////////////////////////////

/////////////////////////////////////////////ʱ�䴦����///////////////////////////////////////////////////////////////////
void player::setTotalTime()
{
    setTotalTime(media->mediaObject->totalTime());
}

void player::setTotalTime(qint64 time)
{
    int secT,minT;
    secT = time/1000;
    minT = secT/60;
    secT = secT%60;
    totalTime->setText(tr(QString::number(minT).sprintf("%02d",minT).toLocal8Bit().data())
                       +tr(":")
                       +tr(QString::number(secT).sprintf("%02d",secT).toLocal8Bit().data()));

}

void player::setCurrentTime(qint64 time)
{
    int secC,minC;
    //secC = media->mediaObject->currentTime()/1000;
    secC = time/1000;
    minC = secC/60;
    secC = secC%60;
    currentTime->setText(tr(QString::number(minC).sprintf("%02d",minC).toLocal8Bit().data())
                         +tr(":")
                         +tr(QString::number(secC).sprintf("%02d",secC).toLocal8Bit().data()));
}
////////////////////////////////////////////ʱ�䴦����///////////////////////////////////////////////////////////////////

void player::setSetting( int contrast,
                         int saturation,
                         int brightness,
                         int hue,
                         int step,
                         int numGif)
{
    media->videoWidget->setContrast(contrast / 10.0);
    media->videoWidget->setSaturation(saturation / 10.0);
    media->videoWidget->setBrightness(brightness / 10.0);
    media->videoWidget->setHue(hue / 10.0);
    this->numGif = numGif;
    this->step = step;
}

void player::onClickedButtonMinimize()
{

    setWindowState(Qt::WindowMinimized);
}

void player::onClickedButtonPlay()
{
    QIcon icon_play;
    if(getPlayState() == true)
    {
        if (media->mediaObject->hasVideo() == true)
        {
            media->mediaObject->play();
            icon_play.addFile(":/image/img/pause.png");
            buttonPlay->setStyleSheet("QPushButton{border:0px;}");
            buttonPlay->setIcon(icon_play);
            setPlayState(false);
        }
        else
        {
            if (media->playlist->getCurrentItemNum() < media->playlist->getList().size())
            {
                media->mediaObject->setCurrentSource(Phonon::MediaSource(media->playlist->getCurrentItem()));
                media->mediaObject->play();
                icon_play.addFile(":/image/img/pause.png");
                buttonPlay->setStyleSheet("QPushButton{border:0px;}");
                buttonPlay->setIcon(icon_play);
                setPlayState(false);
            }
        }
    }
    else
    {
        media->mediaObject->pause();
        icon_play.addFile(":/image/img/play.png");
        buttonPlay->setStyleSheet("QPushButton{border:0px;}");
        buttonPlay->setIcon(icon_play);
        setPlayState(true);
    }
}

void player::onClickedButtonNext()
{
    QIcon icon_play;
    if (media->playlist->next())
    {
        media->mediaObject->setCurrentSource(Phonon::MediaSource(media->playlist->getCurrentItem()));
        media->mediaObject->play();

        icon_play.addFile(":/image/img/pause.png");
        buttonPlay->setStyleSheet("QPushButton{border:0px;}");
        buttonPlay->setIcon(icon_play);
        setPlayState(false);
    }
}


void player::onClickedButtonLast()
{
    QIcon icon_play;
    if (media->playlist->last())
    {
        media->mediaObject->setCurrentSource(Phonon::MediaSource(media->playlist->getCurrentItem()));
        media->mediaObject->play();

        icon_play.addFile(":/image/img/pause.png");
        buttonPlay->setStyleSheet("QPushButton{border:0px;}");
        buttonPlay->setIcon(icon_play);
        setPlayState(false);
    }
}

void player::onClickedButtonSkin()
{
    skin *skin1 = new skin(this);
    skin1->setWindowTitle(tr("Ƥ��"));
    skin1->setWindowFlags(skin1->windowFlags() & ~Qt::WindowMaximizeButtonHint);
    skin1->showNormal();
}

void player::onClickedButtonStop()
{
    QIcon icon_play;
    setTotalTime(0);
    media->mediaObject->stop();
    media->mediaObject->clear();

    icon_play.addFile(":/image/img/pause.png");
    buttonPlay->setStyleSheet("QPushButton{border:0px;}");
    buttonPlay->setIcon(icon_play);
}

void player::onClickedButtonScreen()
{
    QIcon  icon_full;
    if(getScreenState() == true)
    {
        media->videoWidget->enterFullScreen();
        media->videoWidget->setFocus();
        setScreenState(false);
        icon_full.addFile(":/image/img/full_screen.png");
        buttonFullScreen = new QPushButton();
        buttonFullScreen->setStyleSheet("QPushButton{border:0px;}");
        buttonFullScreen->setIcon(icon_full);
    }
    else
    {
        media->videoWidget->exitFullScreen();
        setScreenState(true);
        icon_full.addFile(":/image/img/exit_full_screen.png");
        buttonFullScreen = new QPushButton();
        buttonFullScreen->setStyleSheet("QPushButton{border:0px;}");
        buttonFullScreen->setIcon(icon_full);
    }
}

void player::onClickedButtonGoFarword()
{
    media->goFoward(step * 1000);
    //mediaObject->seek(mediaObject->currentTime()+10000);
}

void player::onClickedButtonDrawBack()
{
    media->goBack(step * 1000);
    //mediaObject->seek(mediaObject->currentTime()-10000);
}

void player::onClickedButtonShot()
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"); //������ʾ��ʽ
    //д�ڽ���
    QString fileName = "snapshot";
    QDir filedir(QDir::currentPath());
    if(!filedir.exists(fileName))
    {
        if(!filedir.mkdir(fileName))
        {
            return;
        }
    }
    fileName.prepend("./");
    fileName.append("/");
    fileName.append(time);
    fileName.append(".bmp");
    qDebug() << "file" << fileName;

    /*
    QImage saveImage = media->videoWidget->snapshot();
    if(!saveImage.save(tr("./1.jpg")))
    {
        QMessageBox::warning(NULL, "warning", "Fail to shot", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else
    {
        QMessageBox::warning(NULL, "warning", "Success to shot", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    */
    QPixmap pixmap;
    media->mediaObject->pause();
    pixmap = pixmap.grabWindow(media->videoWidget->winId());
    if(!pixmap.save(fileName))
    {
        QMessageBox::warning(NULL, "warning", "Fail to shot", QMessageBox::Yes, QMessageBox::Yes);
    }
    else
    {
        QMessageBox::warning(NULL, "warning", "Success to shot", QMessageBox::Yes, QMessageBox::Yes);
    }
    media->mediaObject->play();
}

QImage player::gifscreenShot()
{
    QPixmap pixmap;
    pixmap = pixmap.grabWindow(media->videoWidget->winId());
    return pixmap.toImage();
}

void player::onClickedButtonSet()
{
    //Setting *setting = new Setting();
    setting->setWindowTitle(tr("����"));
    setting->setWindowFlags(setting->windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setting->showNormal();
}

void player::onClickedButtonGif()
//void player::onClickedButtonShot()
{
    ///////gifͼƬ//////
    if (media->mediaObject->state() == Phonon::PlayingState || media->mediaObject->state() == Phonon::PausedState)
    {
        bool isPause = false;
        if (media->mediaObject->state() == Phonon::PausedState) isPause = true;
        GifWriter *Gif = new GifWriter;
        media->mediaObject->pause();
        ////��ȡ10��ͼƬ 1000msһ��
        qint64 beginTime = media->mediaObject->currentTime();
        qint64 value = 1000;
        ////ͼƬ��
        /*
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"); //������ʾ��ʽ
        QString fileName = "./snapshot/";
        fileName.append(time);
        fileName.append(".gif");
        */
        ////ͼƬ��
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"); //������ʾ��ʽ
        QString fileName = "snapshot";
        QDir filedir(QDir::currentPath());
        if(!filedir.exists(fileName))
        {
            if(!filedir.mkdir(fileName))
            {
                return;
            }
        }
        fileName.prepend("./");
        fileName.append("/");
        fileName.append(time);
        fileName.append(".gif");

        QImage img;
        img = gifscreenShot();
        if (!GifBegin(Gif, fileName.toStdString().c_str(), img.width(), img.height(), 20, 8, true)) qDebug() << "wr1"; else qDebug() << "su1";
        for (int i = 1; i < numGif; i++)
        {
            if (value > media->mediaObject->remainingTime()) break;
            media->mediaObject->seek(beginTime + i * value);
            img = gifscreenShot();
            if (!GifWriteFrame(Gif, img.bits(), img.width(), img.height(),20, 8, true)) qDebug() << "wr2" << i; else qDebug() << "su2" << i;
        }
        if (!GifEnd(Gif)) qDebug() << "wr3"; else qDebug() << "su3";

        delete Gif;
        media->mediaObject->seek(beginTime);
        if (!isPause) media->mediaObject->play();
    }
}

//void player::onClickedButtonHide()
//{
//    /*
//    if(getHideState() == true)
//   {
//       videoList->hide();
//       setHideState(false);
//   }
//   else if(getHideState() == false)
//   {
//       videoList->show();
//       setHideState(true);
//   }
//   */
//}

void player::keyPressEvent(QKeyEvent * event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        qDebug("exc");
        onClickedButtonScreen();
        break;
    case Qt::Key_1:
        qDebug("1");
        onClickedButtonScreen();
        break;
    case Qt::Key_Left:
        onClickedButtonDrawBack();
        break;
    case Qt::Key_Right:
        onClickedButtonGoFarword();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void player::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
    else event->ignore();
}

void player::dropEvent(QDropEvent *event)
{
    QIcon icon_play;
    const QMimeData *mimeData=event->mimeData();
    if(mimeData->hasUrls())
    {
        QList<QUrl> urlList=mimeData->urls();
        QString fileNamePath=urlList.at(0).toLocalFile();
        if(!fileNamePath.isEmpty())
        {
            QFile file(fileNamePath);
            if(!file.open(QIODevice::ReadOnly))
                return;
            //media->mediaObject->setCurrentSource(Phonon::MediaSource(fileName));
            //media->mediaObject->play();
            /*
            if (fileNamePath.length() != 0)
            {
                //�������б�Ͷ�������
                videoList->addItem(media->playlist->addFile(fileNamePath));
                //���õ�ǰ���ŵ���Ƶ
                media->playlist->setCurrentItem(videoList->count() - 1);
                media->mediaObject->setCurrentSource(Phonon::MediaSource(media->playlist->getCurrentItem()));
                media->mediaObject->play();
                //setTotalTime();
                icon_play.addFile(":/image/img/pause.png");
                buttonPlay->setStyleSheet("QPushButton{border:0px;}");
                buttonPlay->setIcon(icon_play);
            }
            */
            if (fileNamePath.length() != 0)
            {
                //�������б�Ͷ�������
                int index = media->playlist->getList().indexOf(fileNamePath);
                if ( index == -1)
                {
                    videoList->addItem(media->playlist->addFile(fileNamePath));
                    media->playlist->setCurrentItem(videoList->count() - 1);
                }
                else
                {
                    media->playlist->setCurrentItem(index);
                }
                //���õ�ǰ���ŵ���Ƶ

                media->mediaObject->setCurrentSource(Phonon::MediaSource(media->playlist->getCurrentItem()));
                media->mediaObject->play();
                //setTotalTime();
                icon_play.addFile(":/image/img/pause.png");
                buttonPlay->setStyleSheet("QPushButton{border:0px;}");
                buttonPlay->setIcon(icon_play);
            }
        }
    }
}

void player::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_Drag = true;
        m_DragPosition = event->globalPos() - this->pos();
        event->accept();
    }
}

void player::mouseMoveEvent(QMouseEvent *event)
{
    if (m_Drag && (event->buttons() && Qt::LeftButton)) {
        move(event->globalPos() - m_DragPosition);
        event->accept();
    }
}

void player::mouseReleaseEvent(QMouseEvent *)
{
    m_Drag = false;
}

bool player::getPlayState()
{
    return buttonPlayState;
}

bool player::getScreenState()
{
    return buttonScreenState;
}

void player::setPlayState(bool value)
{
    buttonPlayState = value;
}

void player::setScreenState(bool value)
{
    buttonScreenState = value;
}

bool player::getHideState()
{
    return buttonHideState;
}

void player::setHideState(bool value)
{
    buttonHideState = value;
}

bool player::eventFilter(QObject *obj,QEvent *event)
{
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent *mouseMove = static_cast<QMouseEvent*>(event);
        QRect rect = pushButton->frameGeometry();
        if (rect.contains(mouseMove->pos())) {
            pushButton->show();
        }
        else {
            pushButton->hide();
        }
    }


    if(obj == media->videoWidget)
    {
        if(event->type() == QEvent::KeyPress)
        {
            //qDebug()<<"eventFilter:true";

            switch ((static_cast<QKeyEvent*>(event))->key())
            {
            case Qt::Key_Escape:
                //qDebug("exc");
                onClickedButtonScreen();
                break;
            case Qt::Key_1:
                //qDebug("1");
                onClickedButtonScreen();
                break;
            case Qt::Key_Left:
                onClickedButtonDrawBack();
                break;
            case Qt::Key_Right:
                onClickedButtonGoFarword();
                break;
            case Qt::Key_Space:
                onClickedButtonPlay();
                break;
            }
            return true;
        }
        else
        {
            //qDebug()<<"eventFilter:false";
            return false;
        }
    }


    return QWidget::eventFilter(obj, event);
}


//�۵������
void player::resizeEvent(QResizeEvent *event)
{
    splitter->setGeometry(0, 0, width(), height());
    move((QApplication::desktop()->width() - width())/2,  (QApplication::desktop()->height() - height())/2);
    QWidget::resizeEvent(event);
}

//bool player::eventFilter(QObject *obj, QEvent *event)
//{
//    if (event->type() == QEvent::MouseMove) {
//        QMouseEvent *mouseMove = static_cast<QMouseEvent*>(event);
//        QRect rect = pushButton->frameGeometry();
//        if (rect.contains(mouseMove->pos())) {
//            pushButton->show();
//        }
//        else {
//            pushButton->hide();
//        }
//    }
//    return QWidget::eventFilter(obj, event);
//}

void player::setBtnPos()
{
    pushButton->move(contentFrame->width() - pushButton->width(), (contentFrame->height() -pushButton->height())/2);
}

void player::setBtnIcon()
{
    if (listFrame->width() != 0) {
        pushButton->setIcon(rightIcon);
    }
    else {
        pushButton->setIcon(leftIcon);
    }
}

void player::slotClickedBtn()
{
    QList <int> sizeList;
    sizeList.clear();
    if (listFrame->width() != 0) {
        sizeList.append(800);
        sizeList.append(0);
    }
    else {
        sizeList.append(600);
        sizeList.append(200);
    }
    splitter->setSizes(sizeList);

    setBtnIcon();
    setBtnPos();
    pushButton->hide();
}

void player::slotSplitterMoved(int pos, int index)
{
    Q_UNUSED(pos)
    Q_UNUSED(index)
    setBtnIcon();
    setBtnPos();
}
