#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include "../Logic/AnalysisWebRawData.h"

class SelectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelectWidget(QWidget *parent = nullptr);

public slots:
    void loadFromFile(const QString &dirPath);

protected:
    QVBoxLayout *mainLayout;
    QTabWidget *listWidgetTabWidget;
    QPushButton *OKButton;
    QPushButton *searchButton;
    QPushButton *previousPageButton;
    QPushButton *nextPageButton;
    QLineEdit *templateCodeLineEdit;
    QPushButton *scanQRCodeButton;

    QHash<QListWidgetItem*, QString> templateCodeFinder;

protected slots:
    void toPreviousPage();
    void toNextPageButton();
    void searchButtonPushed();
    void OKButtonPushed();
    AnalysisWebRawData getTemplateCodeData(const QString &templateCode);
    void itemSelectionChanged(QListWidgetItem *item);

signals:
    void searchFinished(const AnalysisWebRawData &analysisWebRawData);
};

#endif // SEARCHDIALOG_H
