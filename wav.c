#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

typedef int16_t i16;
typedef uint16_t u16;
typedef uint32_t u32;

typedef float f32;

void write_le_16(FILE* f, u16 n) {
    fwrite(&n, sizeof(u16), 1, f);
}

void write_le_32(FILE* f, u32 n) {
    fwrite(&n, sizeof(u32), 1, f);
}

#define WRITE_STR_LIT(f, s) fwrite((s), 1, sizeof(s) - 1, f)

#define FREQ 44100
#define DURATION 3

int main(void) {
    FILE* f = fopen("test.wav", "wb");
    
    u32 num_sample = DURATION * FREQ;
    u32 file_size = num_sample * sizeof(u16) + 44;

    WRITE_STR_LIT(f, "RIFF");
    write_le_32(f, file_size - 8);
    WRITE_STR_LIT(f, "WAVE");

    WRITE_STR_LIT(f, "fmt ");
    write_le_32(f, 16);
    write_le_16(f, 1);
    write_le_16(f, 1);
    write_le_32(f, FREQ);
    write_le_32(f, FREQ * sizeof(u16));
    write_le_16(f, sizeof(u16));
    write_le_16(f, sizeof(u16) * 8 );

    WRITE_STR_LIT(f, "data");
    write_le_32(f, num_sample * sizeof(u16));

    for (u32 i = 0; i < num_sample; i++) {
        f32 t = (f32)i / FREQ;

        f32 y = 0.25f * sinf(t * 440.0f * 2.0f * 3.1415926535f);
        i16 sample = (i16)(y * INT16_MAX);

        write_le_16(f, sample);
    }

    fclose(f);

    return 0;
}
