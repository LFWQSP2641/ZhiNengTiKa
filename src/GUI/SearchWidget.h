#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H


class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainLayout;
    QTabWidget *allListWidget;
    QListWidget *listWidget;
    QPushButton *resetButton;
    QPushButton *OKButton;
    QPushButton *searchButton;
    QPushButton *previousPageButton;
    QPushButton *nextPageButton;
    QLineEdit *searchLineEdit;
    QStringList templateNameList;
    QStringList templateCodeList;
    QLineEdit *templateCodeLineEdit;
    QPushButton *selectTemplateIdButton;

    QByteArray webRawData;
    QString templateName;
    QString templateCode;

public slots:
    void reset();

private slots:
    void toPreviousPage();
    void toNextPageButton();
    void searchButtonPushed();
    void cancleButtonPushed();
    void OKButtonPushed();
    void selectTemplateIdButtonPushed();
    void currentItemChanged(QListWidgetItem *item);

signals:
    void searchFinished(const QByteArray &webRawData, const QString &templateName, const QString &templateCode);
};

#endif // SEARCHDIALOG_H
