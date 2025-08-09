#include "MainWindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    player(new QMediaPlayer(this)),
    audioOutput(new QAudioOutput(this))
{
    player->setAudioOutput(audioOutput); // Qt6-ban kötelező

    // Teszt mappa
    //musicFolder = "/home/kmark7/Zene"; // módosítsd a sajátodra
    musicFolder = "/mnt/arch_home/home/kmark7/Zenék/";

    QDir dir(musicFolder);
    oggFiles = dir.entryList({"*.ogg"}, QDir::Files);
    if (oggFiles.isEmpty()) {
        QMessageBox::warning(this, "Hiba", "Nincsenek .ogg fájlok a mappában!");
    }

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    QPushButton *playBtn = new QPushButton("Play Random");
    QPushButton *stopBtn = new QPushButton("Stop");
    QPushButton *skipBtn = new QPushButton("Skip");

    layout->addWidget(playBtn);
    layout->addWidget(stopBtn);
    layout->addWidget(skipBtn);

    setCentralWidget(central);

    connect(playBtn, &QPushButton::clicked, this, &MainWindow::onPlayClicked);
    connect(stopBtn, &QPushButton::clicked, this, &MainWindow::onStopClicked);
    connect(skipBtn, &QPushButton::clicked, this, &MainWindow::onSkipClicked);

    connect(player, &QMediaPlayer::mediaStatusChanged, [this](QMediaPlayer::MediaStatus status){
        if (status == QMediaPlayer::EndOfMedia) {
            onMediaFinished();
        }
    });
}

void MainWindow::onPlayClicked()
{
    playRandom();
}

void MainWindow::onStopClicked()
{
    player->stop();
}

void MainWindow::onSkipClicked()
{
    playRandom();
}

void MainWindow::playRandom()
{
    QString file = chooseNextFile();
    if (!file.isEmpty()) {
        playSelected(file);
    }
}

void MainWindow::playSelected(const QString &filePath)
{
    player->setSource(QUrl::fromLocalFile(filePath)); // <-- Qt6-ban setSource()
    player->play();

    PlayedTrack t{filePath, QDateTime::currentDateTime()};
    history.append(t);
    cleanHistory();
}

QString MainWindow::chooseNextFile()
{
    cleanHistory();

    QStringList recentFiles;
    for (const auto &t : history) {
        recentFiles.append(QFileInfo(t.filePath).fileName());
    }

    QStringList candidates;
    for (const QString &f : oggFiles) {
        if (!recentFiles.contains(f)) {
            candidates.append(f);
        }
    }

    if (candidates.isEmpty()) {
        std::sort(history.begin(), history.end(), [](const PlayedTrack &a, const PlayedTrack &b){
            return a.playedAt < b.playedAt;
        });
        if (!history.isEmpty())
            return history.first().filePath;
        else
            return QString();
    }

    int idx = QRandomGenerator::global()->bounded(candidates.size());
    return QDir(musicFolder).filePath(candidates.at(idx));
}

void MainWindow::cleanHistory()
{
    QDateTime now = QDateTime::currentDateTime();
    history.erase(std::remove_if(history.begin(), history.end(),
                                 [now](const PlayedTrack &t){
                                     return t.playedAt.msecsTo(now) > 10 * 60 * 1000;
                                 }), history.end());
}

void MainWindow::onMediaFinished()
{
    playRandom();
}
