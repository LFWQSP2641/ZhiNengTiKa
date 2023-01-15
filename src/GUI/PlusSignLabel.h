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
#ifdef Q_OS_ANDROID
    void showAskDialog();
#endif // Q_OS_ANDROID
protected slots:
    void paste();
signals:
    void addPixmapLabel(const QPixmap &pixmap);
    void addPixmapLabels(const QList<QUrl> &urls);
};
