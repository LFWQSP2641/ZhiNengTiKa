#pragma once

#include "ClickableLabel.hpp"

class PixmapLabel : public ClickableLabel
{
    Q_OBJECT
public:
    explicit PixmapLabel(QWidget *parent = nullptr);
    explicit PixmapLabel(const QPixmap &pixmap, QWidget *parent = nullptr);
    explicit PixmapLabel(const QPixmap &pixmap, const QString &pixmapUrl, QWidget *parent = nullptr);

    void setPixmap(const QPixmap &pixmap);
    void setPixmap(const QPixmap &pixmap, const QString &pixmapUrl);
    QString getUrl() const
    {
        return url;
    }

    void remove()
    {
        emit removed(this);
    }
protected:
    QString url;
    QPixmap rawPixmap;

    QMenu *menu;
    QAction *copyAction;
    QAction *removeAction;
    QAction *previewAction;

    void uploadPixmap();
    void uploadPixmap(const QPixmap &pixmap);

    void contextMenuEvent(QContextMenuEvent *event) override;
protected slots:
    void copyPixmap();
    void removePixmap();
    void previewPixmap();
    void showTipBox();
signals:
    void removed(PixmapLabel *pixmapLabelPoint);
};

