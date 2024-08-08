#include "spng_main.h"

void read_png_to_argb(const char* filepath, unsigned char** argb_matrix, size_t* width, size_t* height) {
    spng_ctx *ctx = spng_ctx_new(0);

    // Read the PNG file from the specified path
    FILE *fp = fopen(filepath, "rb");
    if (!fp) {
        fprintf(stderr, "Error opening file: %s\n", filepath);
        return;
    }

    fseek(fp, 0, SEEK_END);
    size_t buf_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    unsigned char *buf = malloc(buf_size);
    fread(buf, 1, buf_size, fp);
    fclose(fp);

    spng_set_png_buffer(ctx, buf, buf_size);

    // Calculate output image size
    size_t len;
    spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &len);

    // Allocate memory for the ARGB matrix
    *argb_matrix = malloc(len);

    // Decode the image to an 8-bit RGBA format
    spng_decode_image(ctx, *argb_matrix, len , SPNG_FMT_RGBA8 , 0);

    struct spng_ihdr ihdr;
    int ret = spng_get_ihdr(ctx, &ihdr);
    if(ret)
    {
        printf("spng_get_ihdr() error: %s\n", spng_strerror(ret));
        exit(-1);
    }
    *height = ihdr.height;
    *width = ihdr.width;

    // Clean up
    free(buf);
    spng_ctx_free(ctx);
}