#include "NavigationBarTabWidget.h"

NavigationBarTabWidget::NavigationBarTabWidget(QWidget *parent)
    : QTabWidget{parent}
{

}

int NavigationBarTabWidget::addTabWithScrollArea(QWidget *page, const QString &label)
{
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(page);
    scrollArea->setWidgetResizable(true);
    QScroller::grabGesture(scrollArea->viewport(), QScroller::TouchGesture);
    return QTabWidget::addTab(scrollArea, label);
}

int NavigationBarTabWidget::addTabWithScrollArea(QWidget *page, const QIcon &icon, const QString &label)
{
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(page);
    scrollArea->setWidgetResizable(true);
    QScroller::grabGesture(scrollArea->viewport(), QScroller::TouchGesture);
    return QTabWidget::addTab(scrollArea, icon, label);
}

void NavigationBarTabWidget::resizeEvent([[maybe_unused]]QResizeEvent *event)
{
    QTabWidget::resizeEvent(event);
    if(this->count() > 0)
    {
//        this->tabBar()->setStyleSheet(QStringLiteral("QTabBar::tab{height:25;width:%1}").arg(this->width() / this->count()));
        this->tabBar()->setFixedWidth(this->width());
    }
}

//bool NavigationBarTabWidget::eventFilter(QObject *obj, QEvent *event)
//{
//    if (event->type() == QEvent::Resize)
//    {
//        setStyleSheet(QString("QTabBar::tab{height:50;width:%1}").arg(this->width() / this->count()));
//    }
//    return QTabWidget::eventFilter(obj, event);
//}
