#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "processor.h"
#include "contentgeneratordialog.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QtConcurrent>
#include <QStatusBar>
#include <QProgressBar>
#include <QTimer>

CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMainWindow)
{
    ui->setupUi(this);

    statusLabel = new QLabel();
    ui->statusbar->addPermanentWidget(statusLabel);
    Reset();
}

CMainWindow::~CMainWindow()
{
    delete ui;
}

void CMainWindow::OnProcessFinished(int emojiCount, const QHash<QString, int> &word2Count)
{
    // set emoji count
    ui->labelEmojiCount->setText(QString("%1").arg(emojiCount));

    // set words count
    ui->tableWidgetTop10Words->setRowCount(word2Count.size());
    int currentRow{0};
    foreach (auto key, word2Count.keys()) {
        auto value = word2Count[key];
        auto newWordCell = new QTableWidgetItem(key);
        auto newValueCell = new QTableWidgetItem();
        newValueCell->setData(Qt::DisplayRole, value);
        ui->tableWidgetTop10Words->setItem(currentRow, 0, newWordCell);
        ui->tableWidgetTop10Words->setItem(currentRow, 1, newValueCell);
        currentRow++;
    }
    ui->tableWidgetTop10Words->sortItems(1, Qt::DescendingOrder);
    ui->tableWidgetTop10Words->setRowCount(10);
}

void CMainWindow::OnStateChanged(const QString &state)
{
    statusLabel->setText(state);
}

void CMainWindow::OnFileLoaded(const QString &content)
{
    ui->plainTextEditContent->setPlainText(content);
    EnableContentView();
}

void CMainWindow::OnError(const QString &err)
{
    EnableContentView(false);
    QMessageBox::critical(this, "Error", err);
    OnStateChanged(QString("Error: %1").arg(err));
}

void CMainWindow::EnableContentView(bool yea)
{
    ui->plainTextEditContent->setEnabled(yea);
    ui->pushButtonAnalyze->setEnabled(yea);
    ui->tableWidgetTop10Words->setEnabled(yea);

    ui->actionRun->setEnabled(yea);
    ui->actionClose->setEnabled(yea);
}

void CMainWindow::Reset()
{
    EnableContentView(false);
    setWindowTitle("No file selected");
    ui->lineEditFilePath->clear();
    ui->plainTextEditContent->clear();
    ui->tableWidgetTop10Words->setRowCount(0);
    statusLabel->setText("Ready");
    ui->labelEmojiCount->setText("0");
}

void CMainWindow::LoadFile(const QString &filename)
{
    statusLabel->setText("Loading");
    auto proc = new CProcessor(filename);
    connect(proc, &QThread::finished, proc, &QObject::deleteLater);
    connect(proc, &CProcessor::StateChanged, this, &CMainWindow::OnStateChanged);
    connect(proc, &CProcessor::FileLoaded, this, &CMainWindow::OnFileLoaded);
    connect(proc, &CProcessor::Finished, this, &CMainWindow::OnProcessFinished);
    connect(proc, &CProcessor::Error, this, &CMainWindow::OnError);
    proc->start();
}

void CMainWindow::on_actionOpen_triggered()
{
    auto filename = QFileDialog::getOpenFileName(this, "Load file", "");
    if(!filename.isEmpty()){
        Reset();
        setWindowTitle(filename);
        ui->lineEditFilePath->setText(filename);
        ui->pushButtonAnalyze->setEnabled(true);
        ui->actionRun->setEnabled(true);
    }
}

void CMainWindow::on_actionRun_triggered()
{
    auto filename = ui->lineEditFilePath->text();
    LoadFile(filename);
}

void CMainWindow::on_actionClose_triggered()
{
    Reset();
}

void CMainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About","Talha Orak <talha.orak@gmail.com>");
}

void CMainWindow::on_lineEditFilePath_textEdited(const QString &arg1)
{
    ui->pushButtonAnalyze->setEnabled(!arg1.isEmpty());
    ui->actionRun->setEnabled(!arg1.isEmpty());
}

void CMainWindow::on_actionContent_Generator_triggered()
{
    CContentGeneratorDialog dlg;
    dlg.exec();
}
