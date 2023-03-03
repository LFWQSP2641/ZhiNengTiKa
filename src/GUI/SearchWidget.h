#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include "../Logic/TemplateSearcher.h"

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
    QListWidget *searchResultListWidget;
    QPushButton *OKButton;
    TemplateSearcher templateSearcher;

//    QHash<QListWidgetItem*, QString> hash;
    QStringList templateCodeList;

    void setSearchingState(bool searching);

    void closeEvent(QCloseEvent *event) override;

protected slots:
    void searchStartButtonPushed();
    void searchStopButtonPushed();
    void templateSearcherSearchStoped();
    void templateSearcherSearchFinished(bool success);
    void OKButtonPushed();
    void addToSearchResultListWidget(const QString &templateName, const QString &templateCode);

signals:
    void searchFinished(const QString &templateCode);
};

#endif // SEARCHWIDGET_H
