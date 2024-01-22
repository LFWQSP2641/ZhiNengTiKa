#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include "../Logic/TemplateSearcher.h"
#include "../Logic/TemplateListModel.h"
class TemplateListView;

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = nullptr);

protected:
    QVBoxLayout *mainLayout;
    QLineEdit *searchLineEdit;
    QLabel *stateShowLabel;
    QPushButton *searchStartButton;
    QPushButton *searchStopButton;
    QPushButton *OKButton;
    TemplateSearcher templateSearcher;

    TemplateListModel searchResultTemplateListModel;
    TemplateListView *searchResultTemplateListView;

    void setSearchingState(bool searching);

    void closeEvent(QCloseEvent *event) override;

protected slots:
    void searchStartButtonPush();
    void searchStopButtonPush();
    void templateSearcherSearchStop();
    void templateSearcherSearchFinish(bool success);
    void OKButtonPush();
    void addToSearchResultListWidget(const TemplateSummary &templateSummary);

signals:
    void searchFinished(const TemplateSummary &templateSummary);
};

#endif // SEARCHWIDGET_H
