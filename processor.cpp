#include "processor.h"
#include <QFile>
#include <QtConcurrent>
#include <QHash>

CProcessor::CProcessor(const QString &sFilename, QObject *parent)
    : QThread{parent},
      filename{sFilename}
{

}

void CProcessor::run()
{
    emit StateChanged("Loading");
    QFile f{filename};
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit Error(f.errorString());
        return;
    }
    auto content = f.readAll();
    f.close();
    emit FileLoaded(content);
    auto words = content.split(' ');

    QHash<QString, int> wordFreq;
    int emojiCount = 0;
    auto fut = QtConcurrent::map(words, [&wordFreq, &emojiCount](auto word){
        // TODO : use regex instead of this ugly comparison
        if((word.length() > 1 && word.length()<4) && (word[0] == ':' || word[0] == ';'))emojiCount++;
        else wordFreq.contains(word) ? wordFreq[word]++ : wordFreq[word] = 0;
    });
    fut.waitForFinished();

    emit StateChanged("Done");
    emit Finished(emojiCount, wordFreq);
}

