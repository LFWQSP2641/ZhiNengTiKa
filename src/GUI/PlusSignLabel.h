#pragma once

#include "ClickableLabel.hpp"

class PlusSignLabel : public ClickableLabel
{
    Q_OBJECT
public:
    explicit PlusSignLabel(QWidget *parent = nullptr);
protected:
    QMenu *menu;
    QAction *pasteAction;
    QAction *addAction;

    void contextMenuEvent(QContextMenuEvent *event) override;
public slots:
    void selectImageFile();
protected slots:
    void paste();
signals:
    void addPixmapLabel(const QPixmap &pixmap);
    void addPixmapLabels(const QList<QUrl> &urls);
};
