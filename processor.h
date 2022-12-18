#ifndef CPROCESSOR_H
#define CPROCESSOR_H

#include <QThread>

class CProcessor : public QThread
{
    Q_OBJECT
public:
    explicit CProcessor(const QString &sFilename, QObject *parent = nullptr);
    virtual ~CProcessor(){}
    void run()override;

signals:
    void StateChanged(const QString & state);
    void FileLoaded(const QString & content);
    void Error(const QString & err);
    void Finished(int emojiCount, const QHash<QString,int> & wordCounts);

private:
    QString filename;
};

#endif // CPROCESSOR_H
