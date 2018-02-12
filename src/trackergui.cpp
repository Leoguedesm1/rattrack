#include "trackergui.h"
#include "ui_trackergui.h"
#include "mainwindow.h"
#include "tracker.h"
#include "filesanddirectoriesconstants.h"
#include "fastmodeguiconstants.h"
#include "normalmodeguiconstants.h"
#include "settingsfileconstants.h"

TrackerGUI* TrackerGUI::instance = 0;
const string WINDOW_TITLE = "Rat-track";

TrackerGUI::TrackerGUI(QWidget *parent) :
    QWidget(parent), ui(new Ui::TrackerGUI) { }

TrackerGUI::TrackerGUI(QWidget *parent, int mode, vector<Video *> videos) :
    QWidget(parent), ui(new Ui::TrackerGUI) {

    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    this->setWindowTitle(QString::fromStdString(WINDOW_TITLE));

    vector<Video *> captureVideos = videos;
    this->countVideos = captureVideos.size();
    indexVideo = 0;

    this->mode = mode;
    this->loadSettings();

    //Making GUI for FastMode
    if(this->mode == FAST) {

        //QSignalMapper:
        mapper = new QSignalMapper(this);
        QObject::connect(mapper, SIGNAL(mapped(int)), this, SLOT(cancelTestButtonClicked(int)));

        //QTABLE:
        //Creating Table
        tableWidget = new QTableWidget((int)captureVideos.size(), TABLE_WIDGET_COLUMNS_NUMBER);

        //Disable Horizontal Scroll Bar
        tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        //Making Headers
        QStringList tableHeader;
        tableHeader << "  Teste  " << "  Status  " << "  Resultado  " << " Opcoes ";
        tableWidget->setHorizontalHeaderLabels(tableHeader);
        tableWidget->verticalHeader()->setVisible(false);

        //Adjusting Table Size
        this->resize(FAST_MODE_GUI_WINDOW_WIDTH, FAST_MODE_GUI_WINDOW_HEIGHT);
        tableWidget->setColumnWidth(0, TABLE_WIDGET_COLUMNS_WIDTH);
        tableWidget->setColumnWidth(1, TABLE_WIDGET_COLUMNS_WIDTH);
        tableWidget->setColumnWidth(2, TABLE_WIDGET_COLUMNS_WIDTH);
        tableWidget->setColumnWidth(3, TABLE_WIDGET_COLUMNS_WIDTH);
        tableWidget->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

        //Centering table
        int x = (this->width()-tableWidget->width()) / 2;
        int y = (this->height()-tableWidget->height()-60) / 2;
        tableWidget->move(x, y);

        //Setting the Table
        for(int i = 0; i < (int)captureVideos.size(); i++) {

            //Creating itens to input in table:
            QVBoxLayout *aux;
            QProgressBar *b;
            QLabel *lab;
            QPushButton *bt;
            //Test Column
            Video* cap = captureVideos.at(i);
            lbTests.push_back(new QLabel(cap->getFileName().section("/", -1, -1)));
            lab = lbTests.at(i);
            lab->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

            //Status Column
            barProgress.push_back(new QProgressBar());
            b = barProgress.at(i);
            b->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
            lbStatus.push_back(new QLabel(QString::fromStdString(FAST_MODE_GUI_STATUS_WAITING)));
            lab = lbStatus.at(i);
            lab->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
            statusVerticalLayout.push_back(new QVBoxLayout());
            aux = statusVerticalLayout.at(i);
            aux->addWidget(barProgress.at(i), 0, Qt::AlignCenter);
            aux->addWidget(lbStatus.at(i), 1, Qt::AlignCenter);

            //Results Column
            lbResults.push_back(new QLabel(QString::fromStdString(FAST_MODE_GUI_RESULTS_UNAVAILABLE)));
            lab = lbResults.at(i);
            lab->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

            //Options Column
            QIcon icon(":/cancel");
            btCancelTest.push_back(new QPushButton(icon, QString::fromStdString(FAST_MODE_GUI_CANCEL_ANALYSIS)));
            bt = btCancelTest.at(i);
            QObject::connect(bt, SIGNAL(clicked()), mapper, SLOT(map()));
            mapper->setMapping(bt, i);

            //Putting itens in columns
            QWidget *test = new QWidget(), *status = new QWidget(), *results = new QWidget(), *options = new QWidget();
            QHBoxLayout *testHorLayout = new QHBoxLayout(test), *statusHorLayout = new QHBoxLayout(status),
                    *resultsHorLayout = new QHBoxLayout(results), *optionsHorLayout = new QHBoxLayout(options);

            //Test Column
            testHorLayout->addWidget(lbTests.at(i));
            testHorLayout->setAlignment(Qt::AlignCenter);
            testHorLayout->setContentsMargins(5, 5, 5, 5);
            test->setLayout(testHorLayout);
            tableWidget->setCellWidget(i, 0, test);

            //Status Column
            statusHorLayout->addLayout(statusVerticalLayout.at(i));
            statusHorLayout->setAlignment(Qt::AlignCenter);
            statusHorLayout->setContentsMargins(5, 5, 5, 5);
            status->setLayout(statusHorLayout);
            tableWidget->setCellWidget(i, 1, status);

            //Results Column
            resultsHorLayout->addWidget(lbResults.at(i));
            resultsHorLayout->setAlignment(Qt::AlignCenter);
            resultsHorLayout->setContentsMargins(5, 5, 5, 5);
            results->setLayout(resultsHorLayout);
            tableWidget->setCellWidget(i, 2, results);

            //Options Column
            optionsHorLayout->addWidget(btCancelTest.at(i));
            optionsHorLayout->setAlignment(Qt::AlignCenter);
            optionsHorLayout->setContentsMargins(5, 5, 5, 5);
            options->setLayout(optionsHorLayout);
            tableWidget->setCellWidget(i, 3, options);
        }

        //QButtons:
        QIcon ButtonIcon;

        //Setting CancelButton
        ButtonIcon = QIcon(":/cancel.png");
        btCancel = new QPushButton(ButtonIcon, QString::fromStdString(FAST_MODE_GUI_CANCEL_ALL));
        btCancel->setIconSize(QSize(24,24));
        QObject::connect(btCancel, SIGNAL(clicked()), this, SLOT(cancelAllButtonClicked()));

        //Setting buttonLayout
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();
        buttonLayout->addWidget(btCancel);
        buttonLayout->addStretch();

        //Setting tableLayout
        QHBoxLayout *tableLayout = new QHBoxLayout();
        tableLayout->addWidget(tableWidget, 1);

        //SettingMainLayout
        QVBoxLayout *mainVerticalLayout = new QVBoxLayout(this);
        mainVerticalLayout->addLayout(tableLayout, 1);
        mainVerticalLayout->addLayout(buttonLayout, 1);

        //Making GUI for NormalMode
    }else{

        QFont f;

        //Status Label
        QLabel* lb1Status = new QLabel(QString::fromStdString(NORMAL_MODE_GUI_STATUS));
        f = QFont(QString::fromStdString(TEXT_FONT), FONT_SIZE, QFont::Bold);
        lb1Status->setFont(f);
        lb2Status = new QLabel(QString::fromStdString(NORMAL_MODE_GUI_STATUS_START));
        lb2Status->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        QHBoxLayout *layoutHorStatus = new QHBoxLayout();
        layoutHorStatus->addWidget(lb1Status);
        layoutHorStatus->addWidget(lb2Status);
        layoutHorStatus->addStretch();

        //Video Label
        lbFrame = new QLabel();
        lbFrame->setFixedSize(QSize(FRAME_WIDTH, FRAME_HEIGHT));
        lbFrame->setStyleSheet(QString::fromStdString(FRAME_BACKGROUND));
        QVBoxLayout *layoutVerStatus = new QVBoxLayout();
        layoutVerStatus->addWidget(lbFrame);
        layoutVerStatus->addLayout(layoutHorStatus);

        //Buttons
        QIcon icon;

        icon = QIcon(":/cancel");
        btCancel = new QPushButton(icon, QString::fromStdString(NORMAL_MODE_GUI_CANCEL));
        btCancel->setIconSize(QSize(24, 24));
        QObject::connect(btCancel, SIGNAL(clicked()), this, SLOT(btCancelTest_clicked()));

        icon = QIcon(":/pause");
        btPlayPause = new QPushButton(icon, QString::fromStdString(NORMAL_MODE_GUI_PAUSE));
        btPlayPause->setIconSize(QSize(24,24));
        QObject::connect(btPlayPause, SIGNAL(clicked()), this, SLOT(btPlayPause_clicked()));

        icon = QIcon(":/reset");
        btReset = new QPushButton(icon, QString::fromStdString(NORMAL_MODE_GUI_RESET));
        btReset->setIconSize(QSize(24,24));
        QObject::connect(btReset, SIGNAL(clicked()), this, SLOT(btReset_clicked()));

        icon = QIcon(":/snapshot");
        btSnapshot = new QPushButton(icon, QString::fromStdString(NORMAL_MODE_GUI_SNAPSHOT));
        btSnapshot->setIconSize(QSize(24,24));
        QObject::connect(btSnapshot, SIGNAL(clicked()), this, SLOT(btSnapshot_clicked()));

        QHBoxLayout *layoutHorButtons = new QHBoxLayout();
        layoutHorButtons->addWidget(btPlayPause, 0, Qt::AlignCenter);
        layoutHorButtons->addWidget(btReset, 0, Qt::AlignCenter);
        layoutHorButtons->addWidget(btSnapshot, 0, Qt::AlignCenter);

        icon = QIcon(":/config");
        btAdvancedSettings = new QPushButton(icon, "");
        btAdvancedSettings->setToolTip(QString::fromStdString(NORMAL_MODE_GUI_ADVANCED_SETTINGS));
        QObject::connect(btAdvancedSettings, SIGNAL(clicked()), this, SLOT(btAdvancedSettings_clicked()));

        QHBoxLayout *layoutHorConfig = new QHBoxLayout();
        layoutHorConfig->addStretch(1);
        layoutHorConfig->addWidget(btAdvancedSettings, 0, Qt::AlignCenter);

        //FrameTitle
        QLabel *lbFrameTitle = new QLabel(QString::fromStdString(NORMAL_MODE_GUI_TOOLS));
        f = QFont(QString::fromStdString(TEXT_FONT), FONT_SIZE_TITLE, QFont::Bold);
        lbFrameTitle->setFont(f);

        //ToolsFrame
        QHBoxLayout *layoutHorTitle = new QHBoxLayout();
        layoutHorTitle->addStretch(1);
        layoutHorTitle->addWidget(lbFrameTitle, 0);
        layoutHorTitle->addStretch(1);

        //Settings Widget
        this->createSettingsWindow();
        QHBoxLayout *settingsLayout = new QHBoxLayout();
        settingsLayout->addWidget(settingsWidget);

        QVBoxLayout *layoutVerTools = new QVBoxLayout();
        layoutVerTools->addSpacing(10);
        layoutVerTools->addLayout(layoutHorConfig);
        layoutVerTools->addLayout(layoutHorTitle);
        layoutVerTools->addSpacing(25);
        layoutVerTools->addLayout(layoutHorButtons);
        layoutVerTools->addStretch(1);
        layoutVerTools->addLayout(settingsLayout);
        layoutVerTools->addStretch(1);
        layoutVerTools->addWidget(btCancel);
        layoutVerTools->addStretch();

        QHBoxLayout *layoutHorTools = new QHBoxLayout();
        layoutHorTools->addLayout(layoutVerStatus);
        layoutHorTools->addLayout(layoutVerTools);

        //FileName
        QLabel* lb1File = new QLabel(QString::fromStdString(NORMAL_MODE_GUI_FILE));
        f = QFont(QString::fromStdString(TEXT_FONT), FONT_SIZE, QFont::Bold);
        lb1File->setFont(f);
        lb2File = new QLabel();
        lb2File->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        QHBoxLayout *layoutHorFile = new QHBoxLayout();
        layoutHorFile->addWidget(lb1File);
        layoutHorFile->addWidget(lb2File);
        layoutHorFile->addStretch();

        //MainLayout
        QVBoxLayout *mainVerticalLayout = new QVBoxLayout(this);
        mainVerticalLayout->addLayout(layoutHorFile);
        mainVerticalLayout->addLayout(layoutHorTools);

        this->startNormalModeSettings();
        this->resize(this->minimumSizeHint());
        settingsWidget->hide();
    }

    //Centering Window
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-this->width()) / 2;
    int y = (screenGeometry.height()-this->height()) / 2;
    this->move(x, y);

    //Remove Window's optionsButton
    this->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    this->setFixedSize(this->size());

    instance = this;

    rattrack = new Tracker(0, videos, this->mode);
}

TrackerGUI* TrackerGUI::getInstance() {
    return instance;
}

TrackerGUI::~TrackerGUI() {
    delete ui;
}

void TrackerGUI::restartProgram() {
    this->rattrack = NULL;
    close();
    MainWindow* mw = new MainWindow();
    mw->show();
}

int TrackerGUI::getIndexVideo() {
    return this->indexVideo;
}

void TrackerGUI::setIndexVideo(int index) {
    this->indexVideo = index;
}

void TrackerGUI::setStatus(QString status) {
    if(this->mode == NORMAL)
        this->lb2Status->setText(status);
    else{
        QLabel* lab = lbStatus.at(indexVideo);
        lab->setText(status);
    }
}

//NormalMode Functions
void TrackerGUI::createSettingsWindow() {

    QFont f(QString::fromStdString(TEXT_FONT), FONT_SIZE, QFont::Bold);
    QIcon icon;

    //Vision Mode Layout:
    //----Label
    lbVisionMode = new QLabel("Vision Mode: ");
    lbVisionMode->setFont(f);
    //-----Buttons
    icon = QIcon(":/perspective.png");
    btCameraVision = new QPushButton(icon, "Camera Vision");
    btCameraVision->setIconSize(QSize(24, 24));
    QObject::connect(btCameraVision, SIGNAL(clicked()), this, SLOT(btCameraVision_clicked()));
    icon = QIcon(":/original.png");
    btHomographyVision = new QPushButton(icon, "Homography Vision");
    btHomographyVision->setIconSize(QSize(24, 24));
    QObject::connect(btHomographyVision, SIGNAL(clicked()), this, SLOT(btHomographyVision_clicked()));
    //-----Layout
    QHBoxLayout *visionLayout = new QHBoxLayout();
    visionLayout->addWidget(lbVisionMode);
    visionLayout->addStretch();
    visionLayout->addWidget(btCameraVision);
    visionLayout->addWidget(btHomographyVision);

    //ThreshValue Layout:
    //----Label
    lbThreshValue = new QLabel("Thresh Value: ");
    lbThreshValue->setFont(f);
    lbThreshValue1 = new QLabel();
    lbThreshValue1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //-----Slider
    sliderThreshValue = new QSlider(Qt::Horizontal);
    sliderThreshValue->setRange(0, 255);
    QObject::connect(sliderThreshValue, SIGNAL(valueChanged(int)), this, SLOT(setThreshValue(int)));
    //-----Layout
    QHBoxLayout *threshValueLayout = new QHBoxLayout();
    threshValueLayout->addWidget(lbThreshValue);
    threshValueLayout->addWidget(sliderThreshValue);
    threshValueLayout->addWidget(lbThreshValue1);

    //MaxArea Layout:
    //----Label
    lbMaxArea = new QLabel("Max area: ");
    lbMaxArea->setFont(f);
    lbMaxArea1 = new QLabel();
    lbMaxArea1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //-----Slider
    sliderMaxArea = new QSlider(Qt::Horizontal);
    sliderMaxArea->setRange(1, 1000);
    QObject::connect(sliderMaxArea, SIGNAL(valueChanged(int)), this, SLOT(setMaxArea(int)));
    //-----Layout
    QHBoxLayout *maxAreaLayout = new QHBoxLayout();
    maxAreaLayout->addWidget(lbMaxArea);
    maxAreaLayout->addWidget(sliderMaxArea);
    maxAreaLayout->addWidget(lbMaxArea1);

    //MinArea Layout:
    //----Label
    lbMinArea = new QLabel("Min area: ");
    lbMinArea->setFont(f);
    lbMinArea1 = new QLabel();
    lbMinArea1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //-----Slider
    sliderMinArea = new QSlider(Qt::Horizontal);
    sliderMinArea->setRange(1, 1000);
    QObject::connect(sliderMinArea, SIGNAL(valueChanged(int)), this, SLOT(setMinArea(int)));
    //-----Layout
    QHBoxLayout *minAreaLayout = new QHBoxLayout();
    minAreaLayout->addWidget(lbMinArea);
    minAreaLayout->addWidget(sliderMinArea);
    minAreaLayout->addWidget(lbMinArea1);

    //ThreshThick Layout:
    //----Label
    lbThreshThick = new QLabel("Thresh Thick: ");
    lbThreshThick->setFont(f);
    lbThreshThick1 = new QLabel();
    lbThreshThick1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //-----Slider
    sliderThreshThick = new QSlider(Qt::Horizontal);
    sliderThreshThick->setRange(0, 5);
    QObject::connect(sliderThreshThick, SIGNAL(valueChanged(int)), this, SLOT(setThreshThick(int)));
    //-----Layout
    QHBoxLayout *threshThickLayout = new QHBoxLayout();
    threshThickLayout->addWidget(lbThreshThick);
    threshThickLayout->addWidget(sliderThreshThick);
    threshThickLayout->addWidget(lbThreshThick1);

    //Sliders Layout:
    QVBoxLayout *slidersLayout = new QVBoxLayout();
    slidersLayout->addLayout(threshValueLayout);
    slidersLayout->addLayout(maxAreaLayout);
    slidersLayout->addLayout(minAreaLayout);
    slidersLayout->addLayout(threshThickLayout);

    //MainLayout
    QVBoxLayout *main = new QVBoxLayout();
    main->addLayout(visionLayout);
    main->addStretch();
    main->addLayout(slidersLayout);

    //Widget
    settingsWidget = new QWidget();
    settingsWidget->setLayout(main);
}

void TrackerGUI::btCancelTest_clicked() {
    this->rattrack->cancelAnalysis();
}

void TrackerGUI::btPlayPause_clicked() {
    QTimer* timer = this->rattrack->getTimer();

    if(timer->isActive()) {
        this->rattrack->setTimer(false);
        this->btPlayPause->setText("Play");
        QIcon icon(":/play.png");
        this->btPlayPause->setIcon(icon);
        this->btPlayPause->setIconSize(QSize(24, 24));
    }else{
        this->rattrack->setTimer(true);
        this->btPlayPause->setText("Pausar");
        QIcon icon(":/pause.png");
        this->btPlayPause->setIcon(icon);
        this->btPlayPause->setIconSize(QSize(24, 24));
    }
}

void TrackerGUI::btReset_clicked() {
    this->rattrack->restartAnalysis();
}

void TrackerGUI::btAdvancedSettings_clicked() {
    if(settingsWidget->isHidden())
        settingsWidget->show();
    else
        settingsWidget->hide();
}

void TrackerGUI::btSnapshot_clicked() {
    this->rattrack->saveSnapshot();
}

void TrackerGUI::setFilename(QString name) {
    this->lb2File->setText(name);
}

void TrackerGUI::showImage(Mat showFrame) {
    QImage imageShow((const uchar *) showFrame.data, showFrame.cols, showFrame.rows, showFrame.step, QImage::Format_RGB888);
    lbFrame->setPixmap(QPixmap::fromImage(imageShow));
}

Mat TrackerGUI::getImage() {
    const QPixmap* pixmap = this->lbFrame->pixmap();
    QImage image(pixmap->toImage());
    return Mat(image.height(), image.width(), CV_8UC4 , (uchar*)image.bits(), image.bytesPerLine());
}

//------Settings Functions
void TrackerGUI::btCameraVision_clicked() {
    btCameraVision->setEnabled(false);
    btHomographyVision->setEnabled(true);
    this->screen = CAMERA;
}

void TrackerGUI::btHomographyVision_clicked() {
    btHomographyVision->setEnabled(false);
    btCameraVision->setEnabled(true);
    this->screen = HOMOGRAPHY;
}

void TrackerGUI::setThreshValue(int value) {
    this->sliderThreshValue->setValue(value);
    this->lbThreshValue1->setText(QString::number(value));
    thresholdValue = value;
}

int* TrackerGUI::getThreshValue() {
    return &thresholdValue;
}

void TrackerGUI::setMaxArea(int value) {
    this->sliderMaxArea->setValue(value);
    this->lbMaxArea1->setText(QString::number(value));
    maxArea = value;
}

int* TrackerGUI::getMaxArea() {
    return &maxArea;
}

void TrackerGUI::setMinArea(int value) {
    this->sliderMinArea->setValue(value);
    this->lbMinArea1->setText(QString::number(value));
    minArea = value;
}

int* TrackerGUI::getMinArea() {
    return &minArea;
}

void TrackerGUI::setThreshThick(int value) {
    this->sliderThreshThick->setValue(value);
    this->lbThreshThick1->setText(QString::number(value));
    thresholdThick = value;
}

int* TrackerGUI::getThreshThick() {
    return &thresholdThick;
}

int* TrackerGUI::getScreenMode() {
    return &screen;
}

void TrackerGUI::loadSettings() {
    this->readerSettings = new ReaderXML();
    this->readerSettings->readFile(SETTINGS_DIR_NAME + ADVANCED_SETTINGS_FILE_NAME);
    this->readerSettings->getInFile(THRESHOLD_VALUE_NAME.c_str(), &thresholdValue);
    this->readerSettings->getInFile(MINIMUM_AREA_NAME.c_str(), &minArea);
    this->readerSettings->getInFile(MAXIMUM_AREA_NAME.c_str(), &maxArea);
    this->readerSettings->getInFile(THRESHOLD_THICKNESS_NAME.c_str(), &thresholdThick);
    int screen;
    this->readerSettings->getInFile(SCREEN_MODE_NAME.c_str(), &screen);
    if(screen == 0) this->screen = CAMERA;
    else  this->screen = HOMOGRAPHY;
    this->readerSettings->closeFile();
}

void TrackerGUI::saveSettings() {
    this->writerSettings = new WriterXML();
    this->writerSettings->startFile(SETTINGS_DIR_NAME + ADVANCED_SETTINGS_FILE_NAME);
    this->writerSettings->write(THRESHOLD_VALUE_NAME.c_str()); this->writerSettings->write(thresholdValue);
    this->writerSettings->write(MINIMUM_AREA_NAME.c_str()); this->writerSettings->write(minArea);
    this->writerSettings->write(MAXIMUM_AREA_NAME.c_str()); this->writerSettings->write(maxArea);
    this->writerSettings->write(THRESHOLD_THICKNESS_NAME.c_str()); this->writerSettings->write(thresholdThick);
    this->writerSettings->write(SCREEN_MODE_NAME.c_str());
    if(this->screen == CAMERA) this->writerSettings->write(0);
    else this->writerSettings->write(1);
    this->writerSettings->closeFile();
}

void TrackerGUI::startNormalModeSettings() {

    this->sliderThreshValue->setValue(thresholdValue);
    this->lbThreshValue1->setText(QString::number(thresholdValue));
    this->sliderMaxArea->setValue(maxArea);
    this->lbMaxArea1->setText(QString::number(maxArea));
    this->sliderMinArea->setValue(minArea);
    this->lbMinArea1->setText(QString::number(minArea));
    this->sliderThreshThick->setValue(thresholdThick);
    this->lbThreshThick1->setText(QString::number(thresholdThick));

    if(screen == CAMERA)
        this->btCameraVision->setEnabled(false);
    else
        this->btHomographyVision->setEnabled(false);
}

//FastMode Functions
void TrackerGUI::cancelTestButtonClicked(int indexButton) {

    QLabel* status = lbStatus.at(indexButton);
    //Removing GUI elements
    //Test Column
    removeFromVector(&lbTests, indexButton);
    //Status Column
    removeFromVector(&lbStatus, indexButton);
    removeFromVector(&barProgress, indexButton);
    //Results Column
    removeFromVector(&lbResults, indexButton);
    //Options Column
    removeFromVector(&btCancelTest, indexButton);
    //Removing cell
    tableWidget->removeRow(indexButton);

    //Updating QSignal Mapper
    for(int i = indexButton; i < (int) btCancelTest.size(); i++) {
        QPushButton *bt = btCancelTest.at(i);
        mapper->removeMappings(bt);
        mapper->setMapping(bt, i);
    }

    if(status->text() == QString::fromStdString(FAST_MODE_GUI_STATUS_EVALUATING))
        this->rattrack->cancelAnalysis();
    else
        this->rattrack->removeFromQueue(indexButton);
}

void TrackerGUI::setResults(string path) {
    stringstream link;
    link << "Disponiveis <a href=file:///" << path << ">aqui</a>.";
    QLabel *lab = lbResults.at(indexVideo);
    lab->setText(QString::fromStdString(link.str()));
    lab->setOpenExternalLinks(true);
}

void TrackerGUI::blockCancelButton() {
    QPushButton *bt = btCancelTest.at(indexVideo);
    bt->setEnabled(false);
}

void TrackerGUI::setProgressBar() {
    QProgressBar* bar = this->barProgress.at(indexVideo);
    bar->setMinimum(0);
    bar->setMaximum(100);
    bar->setRange(0, 100);
}

void TrackerGUI::updateProgressBar(int value) {
    QProgressBar* bar = this->barProgress.at(indexVideo);
    bar->setValue(value);
    bar->update();
}

void TrackerGUI::cancelAllButtonClicked() {
    while(this->countVideos > 1) {
        this->rattrack->removeFromQueue(indexVideo+1);
        this->countVideos--;
    }
    this->rattrack->cancelAnalysis();
}

template<typename T>
void TrackerGUI::removeFromVector(vector<T *> *v, int index) {
    T *data = v->at(index);

    typename vector<T *>::iterator position = std::find(v->begin(), v->end(), data);

    if(position != v->end()) v->erase(position);
}
