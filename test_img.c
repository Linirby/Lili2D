#define STB_IMAGE_IMPLEMENTATION
#include "src/render/core/stb_image.h"
#include <stdio.h>
int main() {
    int w, h, c;
    unsigned char *data = stbi_load("examples/hello_sprite_batch/assets/environment.png", &w, &h, &c, 4);
    if (!data) return 1;
    printf("Image %dx%d\n", w, h);
    for (int ty = 0; ty < 2; ty++) {
        for (int tx = 0; tx < 4; tx++) {
            int non_zero = 0;
            for (int y = 0; y < 16; y++) {
                for (int x = 0; x < 16; x++) {
                    int px = tx * 16 + x;
                    int py = ty * 16 + y;
                    if (data[(py * w + px) * 4 + 3] > 0) non_zero++;
                }
            }
            printf("Tile %d,%d: %d solid pixels\n", tx, ty, non_zero);
        }
    }
    return 0;
}
