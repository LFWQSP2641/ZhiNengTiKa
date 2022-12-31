#include "ShowFullScreenWidget.h"

ShowFullScreenWidget::ShowFullScreenWidget(QWidget *widget, QWidget *parent)
    : QWidget{parent}, widget(widget)
{
    widget->setParent(this);
}

void ShowFullScreenWidget::resizeEvent(QResizeEvent *event)
{
    this->widget->resize(event->size());
}
