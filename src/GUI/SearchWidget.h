#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

class AnalysisWebRawData;

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = nullptr);

protected:
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

protected slots:
    void toPreviousPage();
    void toNextPageButton();
    void searchButtonPushed();
    void OKButtonPushed();
    void getTemplateCodeDataButtonPushed();
    void currentItemChanged(QListWidgetItem *item);

signals:
    void searchFinished(const AnalysisWebRawData &analysisWebRawData);
};

#endif // SEARCHDIALOG_H
