#include "contentgeneratordialog.h"
#include "ui_contentgeneratordialog.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QRandomGenerator>
#include <algorithm>
#include <QDateTime>

CContentGeneratorDialog::CContentGeneratorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CContentGeneratorDialog)
{
    ui->setupUi(this);
}

CContentGeneratorDialog::~CContentGeneratorDialog()
{
    delete ui;
}

void CContentGeneratorDialog::on_pushButtonBrowse_clicked()
{
    auto filename = QFileDialog::getSaveFileName(this, "Save");
    if(!filename.isEmpty()){
        ui->lineEditFilename->setText(filename);
    }
}

void CContentGeneratorDialog::on_pushButtonAddRow_clicked()
{
    ui->tableWidgetWords->setRowCount(ui->tableWidgetWords->rowCount()+1);
}

void CContentGeneratorDialog::on_pushButtonDelRow_clicked()
{
    if(ui->tableWidgetWords->selectedItems().size()!=0){
        for(auto row: ui->tableWidgetWords->selectedItems()){
            ui->tableWidgetWords->removeRow(row->row());
        }
    }
}

void CContentGeneratorDialog::on_pushButtonRandomWord_clicked()
{
    auto len = ui->spinBoxRandomWordLen->value();
    QByteArray word;

    auto rc = ui->tableWidgetWords->rowCount();
    for(auto i=0; i<len; ++i){
        word.append(static_cast<char>(QRandomGenerator::global()->bounded('a','z'+1)));

    }
    ui->tableWidgetWords->setRowCount(rc + 1);

    ui->tableWidgetWords->setItem(rc, 0, new QTableWidgetItem(word));
    ui->tableWidgetWords->setItem(rc, 1, new QTableWidgetItem("1"));
}

void CContentGeneratorDialog::on_pushButtonGenerate_clicked()
{
    auto r = QRandomGenerator::global();
    QList<QString> allEmojis{":)",":/",":(",":-)",":-O", ";)", ";-)",":))", "=)",":D", ":-D", "xD", "xDD"};
    QFile f{ui->lineEditFilename->text()};
    QTextStream stream{&f};
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::critical(this, "Error", "Error opening the file");
        return;
    }
    QList<QString> words;
    auto emojiCount = ui->spinBoxSmileyCount->value();

    for(auto i=0; i<emojiCount; ++i){
        words.append(allEmojis[r->bounded(0,allEmojis.length())]);
    }

    for(auto row=0; row<ui->tableWidgetWords->rowCount(); ++row){
        auto word{ui->tableWidgetWords->item(row,0)->text()};
        auto isNumber{false};
        auto count{ui->tableWidgetWords->item(row,1)->text().toInt(&isNumber)};
        if(!isNumber)count = 10;
        for(auto i=0; i<count; ++i){
            words.append(word);
        }
    }

    std::shuffle(words.begin(), words.end(), std::default_random_engine());
    for(auto str : words){
        stream << str << " ";
    }
    f.close();
    QMessageBox::information(this, "Success","All done!");
}

void CContentGeneratorDialog::on_pushButtonReset_clicked()
{
    ui->lineEditFilename->setText("");
    ui->spinBoxRandomWordLen->setValue(5);
    ui->spinBoxSmileyCount->setValue(0);
    ui->tableWidgetWords->setRowCount(0);
}
