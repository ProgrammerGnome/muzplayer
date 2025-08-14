#pragma once

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QLabel>
#include <QSlider>
#include <QString>
#include <MusicPlayerLogic.h>

/*
 * MusicPlayerWindow depends on MusicPlayerLogic headers, but not vice versa
 */
class MusicPlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MusicPlayerWindow(QWidget *parent = nullptr);
    void setMusicFolder(const QString &folder);

private slots:
    void onChangeFolderClicked();
    void onPlayClicked();
    void onStopClicked();
    void onMediaFinished();
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    void onSliderReleased();

private:
    MusicPlayerLogic *logic;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;

    QLabel *musicFolderName;
    QLabel *currentTrackLabel;
    QSlider *positionSlider;

    QString musicFolder;
    QStringList musicFiles;
    QVector<PlayedTrack> history;
};

