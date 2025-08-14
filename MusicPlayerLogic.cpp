#include "MusicPlayerLogic.h"
#include "Constants.h"
#include <QDir>
#include <QFileInfo>
#include <QRandomGenerator>
#include <algorithm>

MusicPlayerLogic::MusicPlayerLogic()
    : musicFolder(DEFAULT_PATH) {
    loadMusicFiles();
}


/*
 * The randomization algoritm's functions
 */

void MusicPlayerLogic::setMusicFolder(const QString &folder) {
    musicFolder = folder;
    loadMusicFiles();
    history.clear();
}

QString MusicPlayerLogic::getMusicFolder() const {
    return musicFolder;
}

QStringList MusicPlayerLogic::getMusicFiles() const {
    return musicFiles;
}

bool MusicPlayerLogic::loadMusicFiles() {
    QDir dir(musicFolder);
    musicFiles = dir.entryList(SUPPORTED_FORMATS, QDir::Files);
    return !musicFiles.isEmpty();
}

QString MusicPlayerLogic::chooseNextFile() {
    cleanHistory();

    QStringList recentFiles;
    for (const auto &t : history)
        recentFiles.append(QFileInfo(t.filePath).fileName());

    QStringList candidates;
    for (const QString &f : musicFiles)
        if (!recentFiles.contains(f))
            candidates.append(f);

    if (candidates.isEmpty()) {
        history.clear();
        candidates = musicFiles;
    }

    if (candidates.isEmpty())
        return QString();

    int idx = QRandomGenerator::global()->bounded(candidates.size());
    return QDir(musicFolder).filePath(candidates.at(idx));
}

void MusicPlayerLogic::addToHistory(const QString &filePath) {
    history.append({filePath, QDateTime::currentDateTime()});
    cleanHistory();
}

void MusicPlayerLogic::cleanHistory() {
    QDateTime now = QDateTime::currentDateTime();
    history.erase(std::remove_if(history.begin(), history.end(),
                                 [now](const PlayedTrack &t){
                                     return t.playedAt.msecsTo(now) > 10 * 60 * 1000;
                                 }),
                  history.end());
}
