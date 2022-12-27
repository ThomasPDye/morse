#include "morse.h"
#include <string.h>
#include <math.h>
#include <ctype.h>

#define M_PI 3.14159265358979323846

const char *morse_code[] = {
    ".-",   // A
    "-...", // B
    "-.-.", // C
    "-..",  // D
    ".",    // E
    "..-.", // F
    "--.",  // G
    "....", // H
    "..",   // I
    ".---", // J
    "-.-",  // K
    ".-..", // L
    "--",   // M
    "-.",   // N
    "---",  // O
    ".--.", // P
    "--.-", // Q
    ".-.",  // R
    "...",  // S
    "-",    // T
    "..-",  // U
    "...-", // V
    ".--",  // W
    "-..-", // X
    "-.--", // Y
    "--.."  // Z
};

const char *morse_numbers[] = {
    "-----", // 0
    ".----", // 1
    "..---", // 2
    "...--", // 3
    "....-", // 4
    ".....", // 5
    "-....", // 6
    "--...", // 7
    "---..", // 8
    "----."  // 9
};

// Morse code callback function
int morse_callback(const void *input_buffer, void *output_buffer,
                   unsigned long frames_per_buffer,
                   const PaStreamCallbackTimeInfo *time_info,
                   PaStreamCallbackFlags status_flags, void *user_data)
{
    // Cast input and output buffers to the correct type
    float *out = (float *)output_buffer;
    paTestData *data = (paTestData *)user_data;

    const char *input_string = data->input_string;
    float *output_samples = data->output_samples;

    // Output silence for the first few samples to allow the audio hardware to
    // stabilize
    for (unsigned int i = 0; i < frames_per_buffer; i++)
    {
        *out++ = 0;
    }

    // Generate Morse code beeps for each character in the input string
    static int current_index = 0;
    static int sample_index = 0;
    static int current_char = 0;
    static int current_symbol = 0;
    static float amplitude = 0.5;
    static float frequency = 1000;
    static float symbol_duration = MORSE_DOT_DURATION;
    for (unsigned int i = 0; i < frames_per_buffer; i++)
    {
        if (current_index < strlen(input_string))
        {
            char c = input_string[current_index];

            // Check if the current character is a letter, numeral, or special character
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
            {
                // Convert the letter to uppercase
                c = toupper(c);

                // Get the Morse code for the current letter
                const char *morse = morse_code[c - 'A'];
                int num_symbols = strlen(morse);

                // Generate Morse code beeps for each symbol in the letter
                if (current_symbol < num_symbols)
                {
                    if (sample_index < symbol_duration * SAMPLE_RATE)
                    {
                        // Generate a sine wave sample
                        *out++ = amplitude * sinf(2 * M_PI * frequency *
                                                  (float)sample_index /
                                                  (float)SAMPLE_RATE);
                        sample_index++;
                    }
                    else
                    {
                        // Advance to the next symbol
                        sample_index = 0;
                        current_symbol++;
                        symbol_duration =
                            (morse[current_symbol] == '.')
                                ? MORSE_DOT_DURATION
                                : MORSE_DASH_DURATION;
                    }
                }
                else
                {
                    // Add a gap between letters
                    if (sample_index < MORSE_LETTER_GAP_DURATION * SAMPLE_RATE)
                    {
                        *out++ = 0;
                        sample_index++;
                    }
                    else
                    {
                        // Advance to the next letter
                        sample_index = 0;
                        current_index++;
                        current_symbol = 0;
                    }
                }
            }
            else if (c >= '0' && c <= '9')
            {
                // Get the Morse code for the current numeral
                const char *morse = morse_numbers[c - '0'];
                int num_symbols = strlen(morse);

                // Generate Morse code beeps for each symbol in the numeral
                if (current_symbol < num_symbols)
                {
                    if (sample_index < symbol_duration * SAMPLE_RATE)
                    {
                        // Generate a sine wave sample
                        *out++ = amplitude * sinf(2 * M_PI * frequency *
                                                  (float)sample_index /
                                                  (float)SAMPLE_RATE);
                        sample_index++;
                    }
                    else
                    {
                        // Advance to the next symbol
                        sample_index = 0;
                        current_symbol++;
                        symbol_duration =

                            (morse[current_symbol] == '.')
                                ? MORSE_DOT_DURATION
                                : MORSE_DASH_DURATION;
                    }
                }
                else
                {
                    // Add a gap between numerals
                    if (sample_index < MORSE_LETTER_GAP_DURATION * SAMPLE_RATE)
                    {
                        *out++ = 0;
                        sample_index++;
                    }
                    else
                    {
                        // Advance to the next numeral
                        sample_index = 0;
                        current_index++;
                        current_symbol = 0;
                    }
                }
            }
            else
            {
                // Add a gap between words
                if (sample_index < MORSE_WORD_GAP_DURATION * SAMPLE_RATE)
                {
                    *out++ = 0;
                    sample_index++;
                }
                else
                {
                    // Advance to the next word
                    sample_index = 0;
                    current_index++;
                    current_symbol = 0;
                }
            }
        }
        else
        {
            // Output silence when the input string is finished
            *out++ = 0;
        }
    }

    return paContinue;
}
