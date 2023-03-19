#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include "../Logic/AnalysisWebRawData.h"
class MultipleSubjectsTemplateListView;

class SelectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelectWidget(QWidget *parent = nullptr);

protected:
    QVBoxLayout *mainLayout;
    MultipleSubjectsTemplateListView *multipleSubjectsTemplateListView;
    QPushButton *OKButton;
    QPushButton *searchButton;
//    QPushButton *previousPageButton;
//    QPushButton *nextPageButton;
    QLineEdit *templateCodeLineEdit;
    QPushButton *scanQRCodeButton;

protected slots:
//    void toPreviousPage();
//    void toNextPageButton();
    void searchButtonPushed();
    void OKButtonPushed();
    AnalysisWebRawData getTemplateCodeData(const QString &templateCode);
    void scanQRCode();

signals:
    void searchFinished(const AnalysisWebRawData &analysisWebRawData);
};

#endif // SEARCHDIALOG_H
