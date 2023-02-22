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
    QPushButton *OKButton;
    QPushButton *searchButton;
    QPushButton *previousPageButton;
    QPushButton *nextPageButton;
    QLineEdit *searchLineEdit;
    QStringList templateNameList;
    QStringList templateCodeList;
    QLineEdit *templateCodeLineEdit;
    QPushButton *getTemplateCodeDataButton;
    QPushButton *scanQRCodeButton;

    QByteArray webRawData;
    QString templateName;
    QString templateCode;

private slots:
    void toPreviousPage();
    void toNextPageButton();
    void searchButtonPushed();
    void OKButtonPushed();
    void getTemplateCodeDataButtonPushed();
    void currentItemChanged(QListWidgetItem *item);
};

#endif // SEARCHDIALOG_H
