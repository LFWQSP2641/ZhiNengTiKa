#pragma once

//大部分QAbstractButton.cpp中copy
class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget *parent = nullptr): QLabel{parent}, down(false) {};
    void setDown(bool down)
    {
        if (this->down == down)
            return;
        this->down = down;
        this->update();
    }
protected:
    bool down;

    bool hitLabel(const QPoint &pos) const
    {
        return this->rect().contains(pos);
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() != Qt::LeftButton)
        {
            event->ignore();
            return;
        }
        if (hitLabel(event->position().toPoint()))
        {
            setDown(true);
            emit pressed();
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() != Qt::LeftButton)
        {
            event->ignore();
            return;
        }

        if (!this->down)
        {
            // refresh is required by QMacStyle to resume the default button animation
            this->update();
            event->ignore();
            return;
        }

        if (hitLabel(event->position().toPoint()))
        {
            emit clicked();
            event->accept();
        }
        else
        {
            event->ignore();
        }
        setDown(false);
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
    void pressed();
    void clicked();
};

