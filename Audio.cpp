#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


using namespace std;

vector<Mix_Chunk*> sounds;
vector<Mix_Music*> music;
int volume;

int loadMusic(const char* filename) {
	Mix_Music *m = NULL;
	m = Mix_LoadMUS(filename);
	if(m == NULL) {
		printf("Failed to load music. SDL_Mixer error: %s\n", Mix_GetError());
		return -1;
	}
	music.push_back(m);
	return music.size()-1;
}


int loadSound(const char* filename) {
	Mix_Chunk *m = NULL;
	m = Mix_LoadWAV(filename);
	if(m == NULL) {
		printf("Failed to load music. SDL_Mixer error: %s\n", Mix_GetError());
		return -1;
	}
	sounds.push_back(m);
	return sounds.size()-1;
}


void setVolume(int v) {
	volume = (MIX_MAX_VOLUME * v) / 100;
}


int playMusic(int m) {
	if(Mix_PlayingMusic() == 0) {
		Mix_Volume(-1, 10);
		Mix_PlayMusic(music[m], -1);
	}
	return 0;
}


int playSound(int s) {
	Mix_Volume(-1, volume);
    Mix_PlayChannel(-1, sounds[s], 0);
	return 0;
}


int initMixer() {
	Mix_Init(MIX_INIT_MP3);
	SDL_Init(SDL_INIT_AUDIO);
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_Mixer couldnt init. Err: %s\n", Mix_GetError());
		return -1;
	}
	setVolume(80);
	return 0;
}




void togglePlay() {

	if(Mix_PausedMusic() == 1) {
		Mix_ResumeMusic();
	} else {
		Mix_PauseMusic();
	}
}

void quitMixer() {

    for(int s=0; s<sounds.size(); s++) {
        Mix_FreeChunk(sounds[s]);
        sounds[s]=NULL;
    }
    for(int m=0; m<music.size(); m++) {
        Mix_FreeMusic(music[m]);
        music[m]=NULL;
    }

    Mix_Quit();
}

