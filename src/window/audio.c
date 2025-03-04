#include "audio.h"

#include <SDL_audio.h>
#include <SDL_error.h>
#include <SDL_stdinc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../error.h"

static void audiomixer(void* const userdata, uint8_t* const stream, int32_t const len) {
    memset(stream, 0, len);            // clear the playing audio
    audiodevice* const dev = userdata; // retreive the callback data

    struct audioplayer* prev = NULL;
    struct audioplayer* curr = dev->audio_players;
    while (curr != NULL) {
        // if the current audio fragment has reached the end of their data
        if (curr->len == 0) {
            struct audioplayer* ncurr = curr->nxt;

            // free the memory allocated to it and assign the next to to the currently playing
            free(curr);
            curr = ncurr;

            // write to the audio device if prev hasn't been set yet
            if (prev == NULL)
                dev->audio_players = curr;
            else
                prev->nxt = curr;

            // continue so if curr is now NULL, the loop stops
            continue;
        }

        // calculate how much of the current audio player we should mix into the stream
        uint32_t const mixlen = SDL_min(curr->len, (uint32_t)len);

        // mix the current buffer into the stream, and update the audio player values accordingly
        SDL_MixAudioFormat(stream, curr->buf, dev->fmt, mixlen, SDL_MIX_MAXVOLUME);
        curr->buf += mixlen;
        curr->len -= mixlen;

        // increment the current node
        prev = curr;
        curr = curr->nxt;
    }
}

static void audio_cvt(audiodevice const* dev, SDL_AudioSpec const* spec, uint8_t** bufptr, uint32_t* len) {
    // init the converter
    SDL_AudioCVT cvt;
    SDL_BuildAudioCVT(&cvt, spec->format, spec->channels, spec->freq, dev->fmt, dev->channels, dev->freq);
    cvt.len = (*len) * spec->channels;                  // calculate the size of the source data in bytes by multiplying the length by the amount of channels (warn: uint32_t -> int32_t)
    cvt.buf = realloc(*bufptr, cvt.len * cvt.len_mult); // grow the inputted buffer for the conversion

    if (cvt.buf == NULL)
        error(ERROR_STD_MEMORY, __FILE_NAME__, __LINE__, "something went wrong whilst growing the audio buffer whilst converting!");

    // converts the audio to the new format
    if (!SDL_ConvertAudio(&cvt))
        error(ERROR_SDL_AUDIO_INIT, __FILE_NAME__, __LINE__, "something went wrong when loading/converting an audio buffer! SDL Error: %s", SDL_GetError());
    *len = cvt.len;

    *bufptr = realloc(cvt.buf, cvt.len_cvt);
    if (*bufptr == NULL)
        error(ERROR_STD_MEMORY, __FILE_NAME__, __LINE__, "something went wrong whilst shrinking the audio buffer whilst converting!");
}

audiodevice* audio_device_init(int32_t freq, SDL_AudioFormat fmt, uint8_t channels, uint16_t samples) {
    audiodevice* dev = malloc(sizeof(audiodevice));

    if (dev == NULL)
        error(ERROR_STD_INIT, __FILE_NAME__, __LINE__, "null pointer when allocating memory for the audio device!");

    // define the audio specification
    SDL_AudioSpec spec = {freq, fmt, channels, 0, samples, 0, 0, NULL, NULL};
    spec.callback = audiomixer;
    spec.userdata = dev;

    // create the audio device
    *dev = (audiodevice){
        NULL,
        SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0),
        freq,
        fmt,
        channels,
    };

    // if the audio device isn't set, cause an error
    if (dev->id < 1)
        error(ERROR_SDL_AUDIO_INIT, __FILE_NAME__, __LINE__, "audio device failed to open! SDL Error: %s", SDL_GetError());

    // default state of the device is paused, so we unpause it here
    SDL_PauseAudioDevice(dev->id, 0);
    return dev;
}

void audio_play(audiodevice* dev, audiodata const* audio) {
    // create an audio player
    struct audioplayer* player = malloc(sizeof(struct audioplayer));
    *player = (struct audioplayer){
        dev->audio_players, // set nxt to the first item in dev (can be NULL, this is fine)
        audio->buf,
        audio->len,
    };

    // assign ourselves to the first item
    dev->audio_players = player;
}

void audio_device_free(audiodevice* dev) {
    SDL_CloseAudioDevice(dev->id);

    struct audioplayer* curr = dev->audio_players;

    // free all audio players
    while (curr != NULL) {
        dev->audio_players = curr->nxt; // use audio_players in dev as a cache
        free(curr);
        curr = dev->audio_players;
    }

    // free the audio device itself
    free(dev);
}

audiodata audio_wav_load(audiodevice const* dev, char const* fpath) {
    SDL_AudioSpec spec;
    audiodata audio;

    debug("loading audio file '%s'...", fpath);

    // load and parse the audio to the correct format
    SDL_LoadWAV(fpath, &spec, &audio.buf, &audio.len);
    audio_cvt(dev, &spec, &audio.buf, &audio.len);

    // calculate the time in miliseconds of the audio fragment
    audio.ms = 1000 * (((audio.len) / (SDL_AUDIO_BITSIZE(dev->fmt) / 8)) / spec.channels / dev->freq);

    return audio;
}

void audio_wav_unload(audiodata* audio) {
    free(audio->buf);
    *audio = (audiodata){0}; // zero out all audio data
}
