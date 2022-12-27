#ifndef MORSE_H
#define MORSE_H
#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>

#define SAMPLE_RATE 44100
#define NUM_SECONDS 10
#define NUM_CHANNELS 1
#define NUM_SAMPLES (NUM_SECONDS * SAMPLE_RATE)
#define MORSE_UNIT_DURATION 0.2
#define MORSE_DOT_DURATION (MORSE_UNIT_DURATION * 1)
#define MORSE_DASH_DURATION (MORSE_UNIT_DURATION * 3)
#define MORSE_LETTER_GAP_DURATION (MORSE_UNIT_DURATION * 3)
#define MORSE_WORD_GAP_DURATION (MORSE_UNIT_DURATION * 7)

// Data structure for passing input and output data to the callback function
typedef struct
{
    const char *input_string;
    float *output_samples;
} paTestData;

int morse_callback(const void *input_buffer, void *output_buffer,
                   unsigned long frames_per_buffer,
                   const PaStreamCallbackTimeInfo *time_info,
                   PaStreamCallbackFlags status_flags, void *user_data);

#endif // MORSE_H