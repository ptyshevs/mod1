#include "minilibx_macos/mlx.h"
#include <stdint.h>
#include <stdlib.h>

void *mlx;
void *win;
void *img;
uint32_t *pxl;

typedef struct s_point
{
    int x;
    int y;
}               point_t;

int myexit(int kc, void *ptr)
{
    exit(0);
}

void draw_curve(point_t p1, point_t p2, point_t p3, int color)
{
    double t = 0.0;
    int x;
    int y;

    int len = abs(p1.x + p2.x + p3.x) > abs(p1.y + p2.y + p3.y) ?
                abs(p1.x + p2.x + p3.x) : abs(p1.y + p2.y + p3.y);

    for (int i = 0; i < len; ++i)
    {
        t = (double)i / len;
        x = (1 - 2 * t + t * t) * p1.x + (2 * t * (1 - t)) * p2.x + t * t * p3.x;
        y = (1 - 2 * t + t * t) * p1.y + (2 * t * (1 - t)) * p2.y + t * t * p3.y;
        mlx_pixel_put(mlx, win, x, y, color);
    }
}

void draw_quadratic_curve(point_t p1, point_t p2, point_t p3, point_t p4, int color)
{
    double t = 0.0;
    int x;
    int y;

    int len = abs(p1.x + p2.x + p3.x + p4.x) > abs(p1.y + p2.y + p3.y + p4.y) ?
                abs(p1.x + p2.x + p3.x + p4.y) : abs(p1.y + p2.y + p3.y + p4.y);
    for (; t < 1.0; t += 0.00001)
    {
        x = (1 - 3 * t + 3 * t * t - t * t * t) * p1.x +
            (3 * t - 6 * t * t + 3 * t * t) * p2.x +
            (3 * t * t - 3 * t * t * t) * p3.x +
            t * t * t * p4.x;
        y = (1 - 3 * t + 3 * t * t - t * t * t) * p1.y +
            (3 * t - 6 * t * t + 3 * t * t) * p2.y +
            (3 * t * t - 3 * t * t * t) * p3.y +
            t * t * t * p4.y;
            mlx_pixel_put(mlx, win, x, y, color);
    }
}

int main(void)
{
    mlx = mlx_init();
    win = mlx_new_window(mlx, 400, 400, "curves");
    for(int y = 0; y < 400; ++y)
        for(int x = 0; x < 400; ++x)
            mlx_pixel_put(mlx, win, x, y, 0xacacac);
    // draw_curve((point_t){0, 200},(point_t){400, 0}, (point_t){0, 400}, 0x820202);
    // draw_curve((point_t){0, 200},(point_t){0, 400}, (point_t){400, 0}, 0x3202a2);
    // draw_curve((point_t){0, 200},(point_t){400, 400}, (point_t){0, 0}, 0xf2e2a2);
    // draw_curve((point_t){0, 200},(point_t){200, 100}, (point_t){400, 400}, 0x823494);
    draw_quadratic_curve((point_t){0, 400}, (point_t){0, 0}, (point_t){400, 400}, (point_t){400, 0}, 0xffffff);
    draw_quadratic_curve((point_t){0, 0}, (point_t){0, 200}, (point_t){0, 200}, (point_t){400, 0}, 0x820202);
    mlx_hook(win, 2, 5, myexit, NULL);
    mlx_loop(mlx);
    return (0);
}