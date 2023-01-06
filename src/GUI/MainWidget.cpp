#include "../StaticClass/Global.h"
#include "../StaticClass/Login.h"
#include "../StaticClass/Setting.h"
#include "MainWidget.h"
#include "AnswerAndAnalysisWidget.h"
#include "SettingWidget.h"
#include "QuestionWidget.h"
#include "SearchWidget.h"
#include "UploadWidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    mainTabWidget = new QTabWidget(this);
    mainLayout = new QGridLayout(this);
    answerAndAnalysisWidget = new AnswerAndAnalysisWidget(analysisWebRawData, this);
    settingWidget = new SettingWidget(this);
    questionWidget = new QuestionWidget(analysisWebRawData, this);
    searchWidget = new SearchWidget(this);
    uploadWidget = new UploadWidget(analysisWebRawData, this);
    searchWidget->hide();
    searchWidget->setParent(this);
    searchWidget->setWindowFlags(Qt::Window);

    mainLayout->addWidget(mainTabWidget, 3, 0, 1, 2);

#ifdef Q_OS_ANDROID
    this->setFixedSize(this->size());
#endif

    mainTabWidget->addTab(searchWidget, QStringLiteral("题卡"));
    mainTabWidget->addTab(answerAndAnalysisWidget, QStringLiteral("答案及解析"));
    mainTabWidget->addTab(questionWidget, QStringLiteral("原题"));
    mainTabWidget->addTab(addScrollArea(uploadWidget), "上传");
    mainTabWidget->addTab(addScrollArea(settingWidget), QStringLiteral("设置"));

    connect(mainTabWidget, &QTabWidget::tabBarClicked, settingWidget, &SettingWidget::refreshTempSize);
    connect(mainTabWidget, &QTabWidget::currentChanged, [this](int index)
    {
        switch (index)
        {
        case 1:
            answerAndAnalysisWidget->setFocusPolicy(Qt::StrongFocus);
            break;
        case 2:
            questionWidget->setFocusPolicy(Qt::StrongFocus);
            break;
        case 3:
            if(!Setting::logined())
            {
                QMessageBox::warning(this, QStringLiteral("warning"), QStringLiteral("未登录"));
            }
        case 4:
            settingWidget->refreshTempSize();
            break;
        default:
            break;
        }
    });

    connect(searchWidget, &SearchWidget::searchFinished, [this]()
    {
        this->mainTabWidget->setCurrentIndex(1);
    });
    connect(searchWidget, &SearchWidget::searchFinished, &analysisWebRawData, &AnalysisWebRawData::analysis);
}

MainWidget::~MainWidget()
{
}

QWidget *MainWidget::addScrollArea(QWidget *Widget)
{
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(Widget);
    scrollArea->setWidgetResizable(true);
    QScroller::grabGesture(scrollArea->viewport(), QScroller::TouchGesture);
    return scrollArea;
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    Setting::saveToFile();
    event->accept();
}

#ifdef Q_OS_ANDROID
void MainWidget::resizeEvent(QResizeEvent *event)
{
    Global::mainWidgetSize = this->size();
    event->accept();
}
#endif
