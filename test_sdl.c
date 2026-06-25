#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL.h>
#include <stdio.h>

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *surface = IMG_Load("examples/hello_sprite_batch/assets/environment.png");
    if (!surface) return 1;
    
    printf("Image is %dx%d\n", surface->w, surface->h);
    
    // Ensure format is 32-bit RGBA for easy reading
    SDL_Surface *conv = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
    Uint32 *pixels = (Uint32*)conv->pixels;
    
    for (int ty = 0; ty < 2; ty++) {
        for (int tx = 0; tx < 4; tx++) {
            int solid = 0;
            for (int y = 0; y < 16; y++) {
                for (int x = 0; x < 16; x++) {
                    int px = tx * 16 + x;
                    int py = ty * 16 + y;
                    Uint32 pixel = pixels[py * conv->w + px];
                    Uint8 r, g, b, a;
                    SDL_GetRGBA(pixel, SDL_GetPixelFormatDetails(conv->format), NULL, &r, &g, &b, &a);
                    if (a > 0) solid++;
                }
            }
            printf("Tile at col %d, row %d has %d solid pixels\n", tx, ty, solid);
        }
    }
    
    SDL_DestroySurface(conv);
    SDL_DestroySurface(surface);
    SDL_Quit();
    return 0;
}
