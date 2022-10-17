#include "stdio.h"
#include "stdlib.h"
#include <conio.h>
#include <windows.h>
#include <time.h>

// 可以调节的三个参数
#define MAP_X_LENGTH 15
#define MAP_Y_LENGTH 15
#define CORE_TIME 300

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define NO_DIRECTION 0

#define MAP_VOID 0
#define MAP_SNAKE_HEAD 1
#define MAP_SNAKE_BODY 2
#define MAP_FOOD 3

struct Coordinate
{
    int x;
    int y;
};

int getDirection();
void initSnake(struct Coordinate *snake);
void draw();
struct Coordinate getNewSnakeHead(struct Coordinate *snake, int direction);
struct Coordinate getRandomPosition();
struct Coordinate getNewFoodPosition(struct Coordinate *snake);

int main()
{
    system("chcp 65001"); // 设置编码为 UTF-8
    system("cls");
    srand((unsigned int)time(0)); // 设置随机数种子。若不设置，每次随机数相同

    printf("任意按键开始...");
    getch();

    int lastDirection = UP;
    int direction;
    int flag = 1;
    struct Coordinate newHead;
    struct Coordinate snake[MAP_X_LENGTH * MAP_Y_LENGTH];
    struct Coordinate food;

    initSnake(snake);
    snake[0] = getRandomPosition();
    food = getNewFoodPosition(snake);

    while (flag)
    {
        // 获取控制参数
        direction = getDirection();
        if (direction == NO_DIRECTION)
        {
            direction = lastDirection;
        }

        newHead = getNewSnakeHead(snake, direction);

        // 蛇身移动
        struct Coordinate temp1 = newHead;
        struct Coordinate temp2;
        int i = 0;
        for (; snake[i].x > -1; i++)
        {
            temp2 = snake[i];
            snake[i] = temp1;
            temp1 = temp2;
        }

        // 吃食物判断
        if (newHead.x == food.x && newHead.y == food.y)
        {
            snake[i] = temp1;
            food = getNewFoodPosition(snake);
        }

        // 是否碰撞身体
        for (i = 1; snake[i].x > -1; i++)
        {
            if (newHead.x == snake[i].x && newHead.y == snake[i].y)
            {
                flag = 0;
            }
        }

        draw(snake, food);
        _sleep(CORE_TIME);

        lastDirection = direction;
    }

    printf("GAME OVER!\n");
    getchar();

    return 0;
}

void initSnake(struct Coordinate *snake)
{
    for (int i = 0; i < MAP_X_LENGTH * MAP_Y_LENGTH; i++)
    {
        snake[i].x = -1;
        snake[i].y = -1;
    }
}

int getDirection()
{
    if (GetKeyState(VK_LEFT) < 0)
    {
        return LEFT;
    }
    else if (GetKeyState(VK_RIGHT) < 0)
    {
        return RIGHT;
    }
    else if (GetKeyState(VK_UP) < 0)
    {
        return UP;
    }
    else if (GetKeyState(VK_DOWN) < 0)
    {
        return DOWN;
    }

    return NO_DIRECTION;
}

// 渲染层，根据 snake 和 food 渲染出视图
void draw(struct Coordinate *snake, struct Coordinate food)
{
    int map[MAP_Y_LENGTH][MAP_X_LENGTH];
    // 填充空白
    for (int x = 0; x < MAP_X_LENGTH; x++)
    {
        for (int y = 0; y < MAP_Y_LENGTH; y++)
        {
            map[y][x] = MAP_VOID;
        }
    }

    // 填充蛇
    map[snake[0].y][snake[0].x] = MAP_SNAKE_HEAD;
    for (int i = 1; snake[i].x > -1; i++)
    {
        map[snake[i].y][snake[i].x] = MAP_SNAKE_BODY;
    }

    // 填充食物
    map[food.y][food.x] = MAP_FOOD;

    system("cls");
    for (int y = 0; y < MAP_Y_LENGTH; y++)
    {
        for (int x = 0; x < MAP_X_LENGTH; x++)
        {
            if (map[y][x] == MAP_VOID)
            {
                printf("口");
            }
            else if (map[y][x] == MAP_SNAKE_HEAD)
            {
                printf("头");
            }
            else if (map[y][x] == MAP_SNAKE_BODY)
            {
                printf("身");
            }
            else if (map[y][x] == MAP_FOOD)
            {
                printf("食");
            }
        }
        printf("\n");
    }
}

struct Coordinate getNewSnakeHead(struct Coordinate *snake, int direction)
{
    struct Coordinate p = snake[0];

    // 蛇头移动
    if (direction == UP)
    {
        p.y -= 1;
    }
    else if (direction == DOWN)
    {
        p.y += 1;
    }
    else if (direction == LEFT)
    {
        p.x -= 1;
    }
    else if (direction == RIGHT)
    {
        p.x += 1;
    }

    // 边界判断
    if (p.x >= MAP_X_LENGTH)
    {
        p.x = 0;
    }
    if (p.x < 0)
    {
        p.x = MAP_X_LENGTH - 1;
    }
    if (p.y >= MAP_Y_LENGTH)
    {
        p.y = 0;
    }
    if (p.y < 0)
    {
        p.y = MAP_Y_LENGTH - 1;
    }

    return p;
}

struct Coordinate getRandomPosition()
{
    struct Coordinate p;
    p.x = rand() % MAP_X_LENGTH;
    p.y = rand() % MAP_Y_LENGTH;
    return p;
}

struct Coordinate getNewFoodPosition(struct Coordinate *snake)
{
    struct Coordinate p;
    int flag = 1;
    // 不断随机，直到不出现在有蛇身体的部分
    while (flag)
    {
        p.x = rand() % MAP_X_LENGTH;
        p.y = rand() % MAP_Y_LENGTH;

        flag = 0;
        for (int i = 0; snake[i].x > -1; i++)
        {
            if (snake[i].x == p.x && snake[i].y == p.y)
            {
                flag = 1;
            }
        }
    }

    return p;
}
