#include <thread>
#include <atomic>
#include <map>
#include <string>
#include <mmsystem.h>
#include <windows.h>
#include <mutex>

class MusicManager {
private:
    void musicLoop() {
        while (running) {
            int trackToPlay;
            {
                std::lock_guard<std::mutex> lock(mutex);
                trackToPlay = currentTrack;
            }
            if (trackToPlay != -1) {
                PlaySound(tracks[trackToPlay].c_str(), NULL, SND_FILENAME | SND_ASYNC);
            }
        }
    }

    std::map<int, std::string> tracks;
    std::atomic<int> currentTrack = 0;
    std::atomic<bool> running;
    std::thread musicThread;
    std::mutex mutex;

public:
    MusicManager() {
        this->running = false;
        this->tracks[0] = "C:\\ufpi\\9 periodo\\COMPUTACAO_GRAFICA\\atividades\\opengl_glut\\bomberman\\sons\\som_jogo.wav";
        this->tracks[1] = "C:\\ufpi\\9 periodo\\COMPUTACAO_GRAFICA\\atividades\\opengl_glut\\bomberman\\sons\\som_bomba.wav";
        this->tracks[2] = "C:\\ufpi\\9 periodo\\COMPUTACAO_GRAFICA\\atividades\\opengl_glut\\bomberman\\sons\\som_start.wav";
    }

    ~MusicManager() {
        stop();
    }

    // void addTrack(int trackID, const std::string& filename) {
    //     tracks[trackID] = filename;
    // }

    void start(int track) {
        std::lock_guard<std::mutex> lock(mutex);
        this->currentTrack = track;
        if (!running) {
            running = true;
            musicThread = std::thread(&MusicManager::musicLoop, this);
        }
    }

    void stop() {
        running = false;
        if (musicThread.joinable()) {
            musicThread.join();
        }
    }

};
