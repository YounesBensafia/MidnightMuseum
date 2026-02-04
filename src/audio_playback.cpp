#include "audio_playback.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

void Audio_Init() {}
void Audio_Shutdown() {
    mciSendStringA("close bgm", NULL, 0, NULL);
}

void Audio_PlayBGM(const std::string& filepath) {
    std::string cmd = "open \"" + filepath + "\" type mpegvideo alias bgm";
    MCIERROR err = mciSendStringA(cmd.c_str(), NULL, 0, NULL);
    if (err == 0) {
        mciSendStringA("setaudio bgm volume to 500", NULL, 0, NULL);
        mciSendStringA("play bgm repeat", NULL, 0, NULL);
    }
}

void Audio_StopBGM() {
    mciSendStringA("stop bgm", NULL, 0, NULL);
    mciSendStringA("close bgm", NULL, 0, NULL);
}

void Audio_SetBGMVolume(float volume) {
    int v = (int)(volume * 1000);
    char buf[64];
    snprintf(buf, sizeof(buf), "setaudio bgm volume to %d", v);
    mciSendStringA(buf, NULL, 0, NULL);
}

void Audio_PlaySFX(const std::string& filepath) {
    PlaySoundA(filepath.c_str(), NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
}

#else

#include <unistd.h>
#include <sys/wait.h>

static pid_t bgm_pid = 0;

void Audio_Init() {
    printf("[Audio] Using system audio tools\n");
}

void Audio_Shutdown() {
    Audio_StopBGM();
}

void Audio_PlayBGM(const std::string& filepath) {
    Audio_StopBGM();

    pid_t pid = fork();
    if (pid == 0) {
        // Child process - try cvlc first, then ffplay, then aplay
        execlp("cvlc", "cvlc", "--play-and-exit", "--loop", filepath.c_str(), (char*)NULL);
        execlp("ffplay", "ffplay", "-nodisp", "-autoexit", "-loop", "0", filepath.c_str(), (char*)NULL);
        // If all fail, exit child
        _exit(1);
    } else if (pid > 0) {
        bgm_pid = pid;
        printf("[Audio] BGM started via system player (pid %d)\n", pid);
    }
}

void Audio_StopBGM() {
    if (bgm_pid > 0) {
        kill(bgm_pid, SIGTERM);
        waitpid(bgm_pid, NULL, WNOHANG);
        bgm_pid = 0;
        printf("[Audio] BGM stopped\n");
    }
}

void Audio_SetBGMVolume(float volume) {
    (void)volume;
}

void Audio_PlaySFX(const std::string& filepath) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp("aplay", "aplay", "-q", filepath.c_str(), (char*)NULL);
        _exit(1);
    }
}

#endif
