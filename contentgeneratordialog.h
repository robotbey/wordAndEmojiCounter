#ifndef CONTENTGENERATORDIALOG_H
#define CONTENTGENERATORDIALOG_H

#include <QDialog>

namespace Ui {
class CContentGeneratorDialog;
}

class CContentGeneratorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CContentGeneratorDialog(QWidget *parent = nullptr);
    ~CContentGeneratorDialog();

private slots:
    void on_pushButtonBrowse_clicked();

    void on_pushButtonAddRow_clicked();

    void on_pushButtonDelRow_clicked();

    void on_pushButtonRandomWord_clicked();

    void on_pushButtonGenerate_clicked();

    void on_pushButtonReset_clicked();

private:
    Ui::CContentGeneratorDialog *ui;
};

#endif // CONTENTGENERATORDIALOG_H
