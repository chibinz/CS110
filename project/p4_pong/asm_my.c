#include "utils.h"
#include <stdlib.h>

#define TOP 10
#define DOWN 70
#define RIGHT 150
#define LEFT 10

typedef struct
{
    // Top left coordinate
    int x;
    int y;

    // Number of pixels between top and down
    int length;
    int width;
} pad;

typedef struct
{
    // Top left Coordinates
    int x;
    int y;

    // Actually length/width of a square
    int size;

    // Velocity
    int vx;
    int vy;

} ball;

// Player 1 and 2
extern pad p1, p2;
// Score of p1, p2
extern int s1, s2;
// Pong
extern ball pong;

int abs(int a)
{
    return a < 0 ? -(a) : a;
}

int sign(int a)
{
    return a > 0 ? 1 : -1;
}

void cpu_tick(pad *p) { p->y = pong.y - (p->length / 2); }

void pad_tick(pad *p)
{
    if (Get_Button(0))
        p->y += 2;
    else if (Get_Button(1))
        p->y -= 2;

    p->y = p->y <= TOP ? TOP : p->y;
    p->y = p->y + p->length >= DOWN ? DOWN - p->length : p->y;
}


void ball_tick(ball *b)
{
    b->x += b->vx;
    b->y += b->vy;
}

void collide_pad(ball *b, pad *p)
{
    // On collision reverse pong's horizontal velocity
    if ((b->y >= p->y) && (b->y <= p->y + p->length))
    {
        if (b->x <= LEFT + p->width)
        {
            b->vx = ((rand() % 3) + 1);
            b->vy = (rand() % 7) - 3;
        }
        else if (b->x + b->size >= RIGHT)
        {
            b->vx = -((rand() % 3) + 1);
            b->vy = (rand() % 7) - 3;
        }
    }
}

void collide_wall(ball *b)
{
    // On collision reverse pong's vertical velocity
    if (b->y <= TOP || b->y >= DOWN)
    {
        b->vy = -(b->vy);
    }
}

void init()
{
    p1.x = LEFT;
    p2.x = RIGHT;
    p1.y = p2.y = 40;
    p1.length = p2.length = 20;
    p1.width = p2.width = 4;

    pong.x = 80;
    pong.y = 40;
    pong.size = 2;

    while (pong.vx == 0)
    {
        pong.vx = (rand() % 7) - 3;
        pong.vy = (rand() % 7) - 3;
    }
}
