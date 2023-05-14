#include "NavigationBarTabWidget.h"

NavigationBarTabWidget::NavigationBarTabWidget(QWidget *parent)
    : QTabWidget{parent}
{

}

int NavigationBarTabWidget::addTabWithScrollArea(QWidget *page, const QString &label)
{
    return QTabWidget::addTab(embedIntoScrollArea(page), label);
}

int NavigationBarTabWidget::addTabWithScrollArea(QWidget *page, const QIcon &icon, const QString &label)
{
    return QTabWidget::addTab(embedIntoScrollArea(page), icon, label);
}

QScrollArea *NavigationBarTabWidget::embedIntoScrollArea(QWidget *page)
{
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(page);
    scrollArea->setWidgetResizable(true);
    QScroller::grabGesture(scrollArea->viewport(), QScroller::TouchGesture);
    return scrollArea;
}

void NavigationBarTabWidget::resizeEvent(QResizeEvent *event)
{
    QTabWidget::resizeEvent(event);
    if(this->count() > 0)
    {
        this->tabBar()->setFixedWidth(this->width());
    }
    event->accept();
}

//bool NavigationBarTabWidget::eventFilter(QObject *obj, QEvent *event)
//{
//    if (event->type() == QEvent::Resize)
//    {
//        setStyleSheet(QString("QTabBar::tab{height:50;width:%1}").arg(this->width() / this->count()));
//    }
//    return QTabWidget::eventFilter(obj, event);
//}
