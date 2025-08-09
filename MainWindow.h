#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QDir>
#include <QDateTime>

struct PlayedTrack {
    QString filePath;
    QDateTime playedAt;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void playRandom();
    void playSelected(const QString &filePath);
    void onPlayClicked();
    void onStopClicked();
    void onSkipClicked();
    void onMediaFinished();

private:
    QMediaPlayer *player;
    QAudioOutput *audioOutput;  // <-- új Qt6-ban
    QStringList oggFiles;
    QList<PlayedTrack> history;
    QString musicFolder;

    QString chooseNextFile();
    void cleanHistory();
};
