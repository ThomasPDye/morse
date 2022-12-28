#include "morse.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    // Read the input file
    const char *input_file = argv[1];
    FILE *f = fopen(input_file, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Error: Could not open file %s\n", input_file);
        return 1;
    }

    // Determine the size of the input file
    fseek(f, 0, SEEK_END);
    long input_size = ftell(f);
    rewind(f);

    // Allocate a buffer for the input string
    char *input_string = calloc(input_size + 1, sizeof(char));
    if (input_string == NULL)
    {
        fprintf(stderr, "Error: Could not allocate memory for input string\n");
        fclose(f);
        return 1;
    }

    // Read the input file into the input string
    size_t num_read = fread(input_string, 1, input_size, f);
    if (num_read != (size_t)input_size)
    {
        fprintf(stderr, "Error: Could not read input file\n");
        fclose(f);
        free(input_string);
        return 1;
    }
    fclose(f);

    // Terminate the input string with a null character
    input_string[input_size] = '\0';

    // Allocate a buffer for the output samples
    float *output_samples = calloc(NUM_SAMPLES, sizeof(float));
    if (output_samples == NULL)
    {
        fprintf(stderr, "Error: Could not allocate memory for output samples\n");
        free(input_string);
        return 1;
    }

    // Set up the paTestData structure with the input string and output samples
    paTestData data;
    data.input_string = input_string;
    data.output_samples = output_samples;

    // Initialize PortAudio
    PaError err = Pa_Initialize();
    if (err != paNoError)
    {
        fprintf(stderr, "Error: Could not initialize PortAudio: %s\n",
                Pa_GetErrorText(err));
        free(input_string);
        free(output_samples);
        return 1;
    }

    // Open the default output stream
    PaStream *stream;
    err = Pa_OpenDefaultStream(
        &stream, 0, NUM_CHANNELS, paFloat32, SAMPLE_RATE,
        paFramesPerBufferUnspecified, morse_callback, &data);
    if (err != paNoError)
    {
        fprintf(stderr, "Error: Could not open output stream: %s\n",
                Pa_GetErrorText(err));
        Pa_Terminate();
        free(input_string);
        free(output_samples);
        return 1;
    }

    // Start the output stream
    err = Pa_StartStream(stream);
    if (err != paNoError)
    {
        fprintf(stderr, "Error: Could not start output stream: %s\n",
                Pa_GetErrorText(err));
        Pa_CloseStream(stream);
        Pa_Terminate();
        free(input_string);
        free(output_samples);
        return 1;
    }

    // Sleep for the duration of the output
    Pa_Sleep(NUM_SECONDS * 1000);

    // Stop the output stream
    err = Pa_StopStream(stream);
    if (err != paNoError)
    {
        fprintf(stderr, "Error: Could not stop output stream: %s\n",
                Pa_GetErrorText(err));
        Pa_CloseStream(stream);
        Pa_Terminate();
        free(input_string);
        free(output_samples);
        return 1;
    }

    // Close the output stream
    err = Pa_CloseStream(stream);
    if (err != paNoError)
    {
        fprintf(stderr, "Error: Could not close output stream: %s\n",
                Pa_GetErrorText(err));
        Pa_Terminate();
        free(input_string);
        free(output_samples);
        return 1;
    }

    // Terminate PortAudio
    Pa_Terminate();

    // Free the input string and output samples buffers
    free(input_string);
    free(output_samples);

    return 0;
}
