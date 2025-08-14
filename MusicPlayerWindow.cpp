#include "MusicPlayerWindow.h"
#include "Constants.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QUrl>

MusicPlayerWindow::MusicPlayerWindow(QWidget *parent)
    : QMainWindow(parent),
    logic(new MusicPlayerLogic),
    player(new QMediaPlayer(this)),
    audioOutput(new QAudioOutput(this)),
    musicFolderName(new QLabel(DEFAULT_PATH, this)),
    currentTrackLabel(new QLabel("No playback!", this)),
    positionSlider(new QSlider(Qt::Horizontal, this))
{
    resize(700, 150);
    player->setAudioOutput(audioOutput);

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    QPushButton *changeFolderBtn = new QPushButton("Change music folder");
    QPushButton *playBtn = new QPushButton("Play Random music");
    QPushButton *stopBtn = new QPushButton("Pause/Continue music");

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(changeFolderBtn);
    btnLayout->addWidget(playBtn);
    btnLayout->addWidget(stopBtn);

    layout->addWidget(musicFolderName);
    layout->addWidget(currentTrackLabel);
    layout->addWidget(positionSlider);
    layout->addLayout(btnLayout);

    setCentralWidget(central);

    connect(changeFolderBtn, &QPushButton::clicked, this, &MusicPlayerWindow::onChangeFolderClicked);
    connect(playBtn, &QPushButton::clicked, this, &MusicPlayerWindow::onPlayClicked);
    connect(stopBtn, &QPushButton::clicked, this, &MusicPlayerWindow::onStopClicked);

    connect(player, &QMediaPlayer::mediaStatusChanged, [this](QMediaPlayer::MediaStatus status){
        if (status == QMediaPlayer::EndOfMedia) {
            onMediaFinished();
        }
    });

    connect(player, &QMediaPlayer::positionChanged, this, &MusicPlayerWindow::onPositionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MusicPlayerWindow::onDurationChanged);
    connect(positionSlider, &QSlider::sliderReleased, this, &MusicPlayerWindow::onSliderReleased);
}



void MusicPlayerWindow::setMusicFolder(const QString &folder) {
    musicFolder = folder;
    logic->loadMusicFiles();
    history.clear();
}

void MusicPlayerWindow::onChangeFolderClicked()
{
    QString newFolder = QFileDialog::getExistingDirectory(
        this,
        tr("Please choose a new music folder!"),
        logic->getMusicFolder(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

    if (!newFolder.isEmpty() && newFolder != logic->getMusicFolder()) {
        QDir dir(newFolder);
        QStringList allFiles = dir.entryList(QDir::Files);
        QStringList allowedFiles = dir.entryList(SUPPORTED_FORMATS, QDir::Files);

        if (allFiles.isEmpty()) {
            QMessageBox::warning(this,
                                 "Empty folder",
                                 "The selected folder is empty.");
            return;
        }

        if (allFiles.size() != allowedFiles.size()) {
            QMessageBox::warning(this,
                                 "Music format error",
                                 "Not all files in the folder are in a supported format!<br>"
                                 "Supported formats: ogg, mp3, wav, flac, m4a, aac");
            return;
        }

        logic->setMusicFolder(newFolder);
        musicFolderName->setText("Music folder: " + logic->getMusicFolder());
    }
}

void MusicPlayerWindow::onPlayClicked() {
    QString file = logic->chooseNextFile();
    if (!file.isEmpty()) {
        player->setSource(QUrl::fromLocalFile(file));
        player->play();
        currentTrackLabel->setText(PLAYBACK_ON + QFileInfo(file).fileName());
        logic->addToHistory(file);
    }
}

void MusicPlayerWindow::onStopClicked()
{
    if (player->playbackState() == QMediaPlayer::PlayingState) {
        player->pause();
        currentTrackLabel->setText(PLAYBACK_OFF + currentTrackLabel->text().remove(PLAYBACK_ON));
    }
    else if (player->playbackState() == QMediaPlayer::PausedState) {
        player->play();
        currentTrackLabel->setText(PLAYBACK_ON + currentTrackLabel->text().remove(PLAYBACK_OFF));
    }
}



void MusicPlayerWindow::onMediaFinished() {
    onPlayClicked();
}

void MusicPlayerWindow::onPositionChanged(qint64 position) {
    positionSlider->setValue(static_cast<int>(position));
}

void MusicPlayerWindow::onDurationChanged(qint64 duration) {
    positionSlider->setRange(0, static_cast<int>(duration));
}

void MusicPlayerWindow::onSliderReleased() {
    player->setPosition(positionSlider->value());
}
