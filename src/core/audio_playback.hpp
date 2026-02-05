#pragma once
#include <string>

void Audio_Init();
void Audio_Shutdown();
void Audio_PlayBGM(const std::string& filepath);
void Audio_StopBGM();
void Audio_SetBGMVolume(float volume);
void Audio_PlaySFX(const std::string& filepath);
