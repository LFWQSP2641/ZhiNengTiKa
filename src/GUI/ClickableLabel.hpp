#pragma once


class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget *parent = nullptr): QLabel{parent} {};
protected:
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if(event != Q_NULLPTR && event->button() == Qt::LeftButton)
        {
            emit clicked();
        }
    }

    void enterEvent(QEnterEvent*) override
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    void leaveEvent(QEvent*) override
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }
signals:
    void clicked();
};

