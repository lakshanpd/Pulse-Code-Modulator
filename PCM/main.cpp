#include <math.h>
#include <stdio.h>
#include <stdlib.h>
// function prototypes

typedef struct {
    float A;
    float omega;
    float sigma;
    int duration;
} asignal;
void run(asignal * inputsignal);

// Implement functions here
float analog_signal_generator(asignal signal, int t)
{
    //TODO
    float A;
    float omega;
    float sigma;

    A = signal.A;
    omega = signal.omega;
    sigma = signal.sigma;

    float result;
    result = A*sin(omega*t+sigma);
    return result;
}

int sample_size;

void sampler(float *samples, int interval, asignal signal)
{
    //TODO
    int duration;
    duration = signal.duration;
    int index = 0;

    for (int t=0;t<=duration;t+=interval)
    {
        samples[index] = analog_signal_generator(signal, t);
        index++;
    }
    sample_size = signal.duration/interval+1;
}

void quantizer1(float *samples, int *pcmpulses, int levels)
{
    //TODO
    float min = INFINITY;
    float max = -INFINITY;
    int index=0;

    while (index<=levels+1)
    {
        if (samples[index]<min)
        {
            min = samples[index];
        }
        if (samples[index]>max)
        {
            max = samples[index];
        }
        index++;
    }
    min = floor(min);
    max = floor(max)+1;
    index=0;
    float  quantization_levels;

    while (index<=levels+1)
    {
        quantization_levels = ((samples[index]-min)/(max-min))*levels;
        pcmpulses[index] = floor(quantization_levels);
        index++;
    }
}

void encoder(int *pcmpulses, int *dsignal, int encoderbits)
{
    //TODO
    int dsignal_size = sample_size*encoderbits;
    int index = 0;
    int i = 0;
    int j=0;
    int binary[encoderbits];
    int index_pulse=0;

    while (index<=dsignal_size)
    {
        for (j=0;j<encoderbits;j++)
        {
            binary[j] = 0;
        }

        while (pcmpulses[index_pulse] > 0 && i < encoderbits) {
            binary[i++] = pcmpulses[index_pulse] % 2;
            pcmpulses[index_pulse] = pcmpulses[index_pulse] / 2;
        }

        for (int k=encoderbits-1;k>=0;k--)
        {
            dsignal[index] = binary[k];
            index++;
        }

        i = 0;
        index_pulse++;
    }

    for (int i=0 ; i<12 ; i++)
    {
        printf("%d",dsignal[i]);
    }
}

//-------------------------------------------------------------

int main()
{
    asignal * inputsignal = (asignal *) malloc(sizeof(asignal));
    run(inputsignal);

    //call any other function here

    free(inputsignal);
    return 0;
}

void run(asignal * inputsignal)
{
    float A;
    float omega;
    float sigma;
    int duration;
    int interval;
    int encoderbits;

    scanf("%f",&A);

    scanf("%f",&omega);

    scanf("%f",&sigma);

    scanf("%d",&duration);

    scanf("%d",&interval);

    scanf("%d",&encoderbits);


    inputsignal->duration = duration;
    inputsignal->A = A;
    inputsignal->omega = omega;
    inputsignal->sigma = sigma;

    int no_of_samples = duration/interval+1;
    float *samples = (float*) malloc(sizeof(float)*no_of_samples);
    int *pcmpulses = (int*) malloc(sizeof(int)*no_of_samples);
    int *dsignal = (int*) malloc(sizeof(int)*no_of_samples*encoderbits);


    sampler(samples, interval, *inputsignal);

    quantizer1(samples, pcmpulses, (int)pow(2,encoderbits));

    encoder(pcmpulses, dsignal, encoderbits);

    free(samples);
    free(pcmpulses);
    free(dsignal);
}
