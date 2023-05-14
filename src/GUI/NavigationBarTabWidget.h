#pragma once

class NavigationBarTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit NavigationBarTabWidget(QWidget *parent = nullptr);

    int addTabWithScrollArea(QWidget *page, const QString &label);
    int addTabWithScrollArea(QWidget *page, const QIcon &icon, const QString &label);

protected:
    QScrollArea *embedIntoScrollArea(QWidget *page);
    void resizeEvent(QResizeEvent *event) override;
//    bool eventFilter(QObject *obj, QEvent *event) override;
};

