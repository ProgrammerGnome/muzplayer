# MuzPlayer

A simple, tiny Qt-based music player application that supports `ogg`, `mp3`, `wav`, `flac`, `m4a`, and `aac` formats.  
The player randomly selects files from the chosen folder and ensures that tracks played within the last 10 minutes are not repeated.

## Features

- **Selectable music folder** – only folders containing exclusively supported formats can be selected.
- **Random playback** – randomly chooses from the folder’s files, avoiding recently played tracks.
- **Playback history handling** – prevents tracks played in the last 10 minutes from repeating.
- **Playback progress bar** – shows the current track position.
- **Currently playing track display** – always visible which file is being played.
- **Default folder** – on startup, loads from the directory set in `DEFAULT_PATH`.

## Supported formats

- `.ogg`
- `.mp3`
- `.wav`
- `.flac`
- `.m4a`
- `.aac`

## Installation & Build

### Dependencies
- **Qt 6** or later (Qt Multimedia module required)
- C++17 compatible compiler (e.g., GCC, Clang, MSVC)

### Build with CMake
```bash
git clone https://github.com/ProgrammerGnome/muzplayer.git
cd muzplayer
mkdir build && cd build
cmake ..
make
```