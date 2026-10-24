#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define M_PI 3.14159265358979323846

typedef int8_t fourcc[4]; //everytime i write fourcc, i mean an array of four 8-bit integers

struct riff_hdr
{
    fourcc id;
    uint32_t size; //unsigned
    fourcc type; 
};

struct fmt_ck //will describe how the data will be encoded
{
    fourcc id;
    uint32_t size;
    uint16_t fmt_tag;
    uint16_t channels;
    uint32_t samples_per_sec;
    uint32_t bytes_per_sec;
    uint16_t block_align;
    uint16_t bits_per_sample;
};

struct data_hdr{
    fourcc id;
    uint32_t size;
};

struct wav_hdr
{
    struct riff_hdr riff;
    struct fmt_ck fmt;
    struct data_hdr data;       
};

typedef int16_t sample_t;
#define SAMPLE_MAX 32767 //max positive amplitude a signed 16-bit sample can store(amplitude to numerical value converter)

#define DURATION 5
#define SR 44100 //sample rate
#define NCHANNELS 1 //mono 
#define NSAMPLES (NCHANNELS*DURATION*SR) //total of individual audio samples

int main(){
    struct wav_hdr hdr = {0}; //initalize all members to zero
    FILE *fp = fopen("output.wav", "wb"); //binary mode

    /*RIFF header*/
    memcpy(&hdr.riff.id, "RIFF", 4);
    hdr.riff.size = 36 + NSAMPLES*sizeof(sample_t); //36 is the starting point after excluding the first 8 bytes of the header
    memcpy(&hdr.riff.type, "WAVE", 4);

    /*FMT chunk*/
    memcpy(&hdr.fmt.id, "fmt ", 4);
    hdr.fmt.size = 16;
    hdr.fmt.fmt_tag = 1;
    hdr.fmt.channels = NCHANNELS;
    hdr.fmt.samples_per_sec = SR;
    hdr.fmt.bytes_per_sec = NCHANNELS*SR*sizeof(sample_t);
    hdr.fmt.block_align = NCHANNELS*sizeof(sample_t);
    hdr.fmt.bits_per_sample = 8*sizeof(sample_t);

    /* DATA header */
    memcpy(&hdr.data.id, "data", 4);
    hdr.data.size = NSAMPLES*sizeof(sample_t);

    fwrite(&hdr, sizeof(struct wav_hdr), 1, fp);

    static sample_t buf[NSAMPLES];

    for (size_t i = 0; i < NSAMPLES; i++)
        buf[i] = lrint(SAMPLE_MAX*sin(2* M_PI *440*i/SR)); //lrint rounds float to nearest int
    
    fwrite(buf, sizeof(buf), 1, fp);

    if (sizeof(buf) % 2 == 1){
        char nil = 0;
        fwrite(&nil, 1, 1, fp);
    }

    fclose(fp);
}