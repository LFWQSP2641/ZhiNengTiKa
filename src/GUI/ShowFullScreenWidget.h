#pragma once


class ShowFullScreenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowFullScreenWidget(QWidget *widget, QWidget *parent = nullptr);

protected:
    QWidget *widget;

    void resizeEvent(QResizeEvent *event) override;

signals:

};

