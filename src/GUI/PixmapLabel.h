#pragma once

class PixmapLabel : public QLabel
{
    Q_OBJECT
public:
    explicit PixmapLabel(QWidget *parent = nullptr);
    QString getUrl() const
    {
        return url;
    }

protected:
    QPixmap pixmap;
    QString url;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void enterEvent(QEnterEvent*) override
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    void leaveEvent(QEvent*) override
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }

public slots:
    void setPixmapFromNetwork(const QString &url);

signals:
    void pixmapRemoved(PixmapLabel *thisPoint);
    void pixmapSet();
};

