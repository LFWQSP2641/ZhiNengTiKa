#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include "../Logic/TemplateAnalysis.h"
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
    QLineEdit *templateCodeLineEdit;
    QPushButton *scanQRCodeButton;

    TemplateAnalysis getTemplateAnalysis(const QString &templateCode);

protected slots:
    void searchButtonPush();
    void OKButtonPush();
    void showTemplateDetailWidget(const TemplateAnalysis &templateAnalysis);
    void scanQRCode();
};

#endif // SEARCHDIALOG_H
