#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class CMainWindow; }
QT_END_NAMESPACE

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = nullptr);
    ~CMainWindow();

public slots:
    void OnProcessFinished(int emojiCount, const QHash<QString, int> & word2Count);
    void OnStateChanged(const QString & state);
    void OnFileLoaded(const QString & content);
    void OnError(const QString & err);

private slots:
    void EnableContentView(bool yea=true);
    void Reset();
    void LoadFile(const QString & filename);

    void on_actionOpen_triggered();

    void on_actionRun_triggered();

    void on_actionClose_triggered();

    void on_actionAbout_triggered();

    void on_lineEditFilePath_textEdited(const QString &arg1);

    void on_actionContent_Generator_triggered();

private:
    Ui::CMainWindow *ui;
    QLabel *statusLabel;

};
#endif // CMAINWINDOW_H
