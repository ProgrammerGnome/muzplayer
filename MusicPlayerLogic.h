#pragma once
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QVector>

struct PlayedTrack {
    QString filePath;
    QDateTime playedAt;
};

class MusicPlayerLogic {
public:
    MusicPlayerLogic();

    void setMusicFolder(const QString &folder);
    QString getMusicFolder() const;
    QStringList getMusicFiles() const;

    bool loadMusicFiles();
    QString chooseNextFile();
    void playSelected(const QString &filePath);
    void addToHistory(const QString &filePath);
    void cleanHistory();

private:
    QString musicFolder;
    QStringList musicFiles;
    QVector<PlayedTrack> history;
};
