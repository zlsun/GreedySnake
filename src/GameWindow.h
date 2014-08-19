#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <sstream>
#include <fstream>
#include <string>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <zlog.h>

#include "SDLWindow.h"

const int SCREEN_X      = SDL_WINDOWPOS_CENTERED;
const int SCREEN_Y      = SDL_WINDOWPOS_CENTERED;
const int SCREEN_WIDTH  = 480;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE    = 15;
const int BLOCK_WIDTH   = SCREEN_WIDTH / BLOCK_SIZE;
const int BLOCK_HEIGHT  = SCREEN_HEIGHT / BLOCK_SIZE;
const int MAX_TIMEOUT   = 300;

const char dataPath[]   = "./record.dat";
const char fontFile[]   = "./res/Lesson6/segoeui.ttf";
SDLColor textColor      = 0xFFFFFF;
int fontSize            = 20;

enum Diretion {
    ELeft, ERight, EUp, EDown
};
int trans[4][2] = {{ -1, 0}, {1, 0}, {0, -1}, {0, 1}};

class Block {
public:
    int x, y;
    Block(int xx, int yy): x(xx), y(yy) {}
};

class Food {
public:
    int x, y;
    int type;
    Food(int xx, int yy, int tp = 0): x(xx), y(yy), type(tp) {}
};

class Snake {
public:
    std::deque<Block> blocks;
    Diretion diretion;
    bool eating;

    Snake(int x, int y): diretion(ERight), eating(false) {
        blocks.push_front(Block(x, y));
    }

    // 设置方向
    void setDirecion(Diretion dir) {
        switch (dir) {
            case ELeft:
                if (diretion != ERight) {
                    diretion = ELeft;
                }
                break;
            case ERight:
                if (diretion != ELeft) {
                    diretion = ERight;
                }
                break;
            case EUp:
                if (diretion != EDown) {
                    diretion = EUp;
                }
                break;
            case EDown:
                if (diretion != EUp) {
                    diretion = EDown;
                }
                break;
            default: break;
        }
    }

    // 移动
    void move() {
        // 新的头部
        Block head = blocks.front();
        head.x += trans[diretion][0];
        head.y += trans[diretion][1];
        // 越界处理
        if (head.x < 0) {
            head.x = BLOCK_WIDTH - 1;
        } else if (head.x >= BLOCK_WIDTH) {
            head.x = 0;
        }
        if (head.y < 0) {
            head.y = BLOCK_HEIGHT - 1;
        } else if (head.y >= BLOCK_HEIGHT) {
            head.y = 0;
        }
        // 添加新的头部
        blocks.push_front(head);
        if (!eating) {
            // 没吃到食物，去掉尾部
            blocks.pop_back();
        } else {
            eating = false;
        }
    }

    // 是否碰到身体
    bool crash() {
        for (size_t i = 0; i < blocks.size() - 1; ++i) {
            for (size_t j = i + 1; j < blocks.size(); ++j) {
                if (blocks[i].x == blocks[j].x &&
                        blocks[i].y == blocks[j].y) {
                    // zlog blocks.size(), i, j, blocks[i].x,
                    //     blocks[i].y, blocks[j].x, blocks[j].y;
                    return true;
                }
            }
        }
        return false;
    }

    // 是否碰到食物
    bool contain(Food f) {
        for (auto b : blocks) {
            if (f.x == b.x && f.y == b.y) {
                return true;
            }
        }
        return false;
    }

    // 是否吃到食物
    bool fetch(Food f) {
        Block head = blocks.front();
        if (head.x == f.x && head.y == f.y) {
            return true;
        }
        return false;
    }

    // 吃食物
    void eat(Food f) {
        eating = true;
    }
};

class Database {
public:
    std::string path;
    int record;

    Database(const std::string& path): path(path) {}

    // 创建文件
    void create() {
        // 设置默认值
        record = 0;
        // 写入默认值
        save();
    }

    // 读取文件
    void read() {
        std::ifstream file;
        file.open(path);
        // 如果文件不存在
        if (file.fail()) {
            // 关闭文件句柄
            file.close();
            // 创建文件
            create();
            return;
        }
        file >> record;
        file.close();
    }

    // 保存文件
    void save() {
        std::ofstream file;
        file.open(path);
        file << record;
        file.close();
    }
};


class GameWindow: public SDLWindow {
private:
    enum State {
        EMenu, EPlay, EPause, EDie, ERecord
    };

    int score;
    SDLTexture texMenu, texPause, texScore, texDie, texRecord;
    State state;
    Diretion dir;
    Snake snake;
    Food food;
    Database db;

public:
    GameWindow(): SDLWindow(),
        snake(-1, -1), food(-1, -1), db(dataPath) {
    }

    void onCreate() {
        zlog "GameWindow::onCreate";
        state = EMenu;
        srand(time(NULL));  // 设置随机种子
        db.read();          // 读取记录
        loadResource();     // 加载资源
    }

    // 加载静态资源
    void loadResource() {
        texMenu = renderer.loadText("Greedy Snake v 1.0.0", fontFile, textColor, fontSize * 2);
        texPause = renderer.loadText("Pause", fontFile, textColor, fontSize * 2);
        texDie = renderer.loadText("You Die!", fontFile, textColor, fontSize * 2);
    }

    // 更新成绩资源
    void updateScore() {
        std::ostringstream str;
        str << "Score: " << score;
        texScore = renderer.loadText(str.str(), fontFile, textColor, fontSize);
    }

    // 更新记录资源
    void updateRecord() {
        std::ostringstream str;
        str << "Record: " << db.record;
        texRecord = renderer.loadText(str.str(), fontFile, textColor, fontSize);
    }

    void start() {
        state = EPlay;
        dir = ERight;
        snake = Snake(BLOCK_WIDTH / 2, BLOCK_HEIGHT / 2);
        score = 0;
        updateScore();
        updateRecord();
        generateFood();
        setTimeout(MAX_TIMEOUT);
    }

    void pause() {
        state = EPause;
    }

    void resume() {
        state = EPlay;
    }

    void quit() {
        db.save();
        close();
    }

    void generateFood() {
        do {
            food.x = rand() % BLOCK_WIDTH;
            food.y = rand() % BLOCK_HEIGHT;
        } while (snake.contain(food));
    }

    void drawSnake(SDLRenderer* ren) {
        ren->setDrawColor(0x00, 0xFF, 0x00);
        SDLRect rc(0, 0, BLOCK_SIZE, BLOCK_SIZE);
        for (auto b : snake.blocks) {
            rc.x = b.x * BLOCK_SIZE;
            rc.y = b.y * BLOCK_SIZE;
            ren->fillRect(rc);
        }
    }

    void drawFood(SDLRenderer* ren) {
        ren->setDrawColor(0xFF, 0x00, 0x00);
        SDLRect rc(food.x * BLOCK_SIZE, food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
        ren->fillRect(rc);
    }

    void drawScore(SDLRenderer* ren) {
        ren->drawTexture(texScore, 0, 0);
    }

    void drawRecord(SDLRenderer* ren) {
        ren->drawTexture(texRecord, SCREEN_WIDTH, 0, EAlignRight | EAlignTop);
    }

    void drawMenu(SDLRenderer* ren) {
        ren->drawTexture(texMenu, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, EAlignHCenter | EAlignVCenter);
    }

    void drawPause(SDLRenderer* ren) {
        ren->drawTexture(texPause, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, EAlignHCenter | EAlignVCenter);
    }

    void drawDie(SDLRenderer* ren) {
        ren->drawTexture(texDie, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, EAlignHCenter | EAlignVCenter);
    }

    void drawNewRecord(SDLRenderer* ren) {
        std::ostringstream str;
        str << "New Record: " << db.record;
        ren->drawText(str.str(), fontFile, textColor, fontSize * 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, EAlignHCenter | EAlignVCenter);
    }

    void onKeyDown(SDL_KeyboardEvent* key) {
        if (state == EMenu) {
            switch (key->keysym.sym) {
                case SDLK_SPACE:
                    start();
                    break;
                case SDLK_ESCAPE:
                    quit();
                    break;
                default:
                    break;
            }
        } else if (state == EPlay) {
            switch (key->keysym.sym) {
                case SDLK_a:
                case SDLK_LEFT:
                    dir = ELeft;
                    break;
                case SDLK_d:
                case SDLK_RIGHT:
                    dir = ERight;
                    break;
                case SDLK_w:
                case SDLK_UP:
                    dir = EUp;
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    dir = EDown;
                    break;
                case SDLK_SPACE:
                    pause();
                    break;
                case SDLK_ESCAPE:
                    quit();
                    break;
                default:
                    break;
            }
        } else if (state == EPause) {
            switch (key->keysym.sym) {
                case SDLK_SPACE:
                    resume();
                    break;
                case SDLK_ESCAPE:
                    quit();
                    break;
                default:
                    break;
            }
        } else if (state == EDie || state == ERecord) {
            switch (key->keysym.sym) {
                case SDLK_SPACE:
                    start();
                    break;
                case SDLK_ESCAPE:
                    quit();
                    break;
                default:
                    break;
            }
        }
    }

    void onMouseButtonDown(SDL_MouseButtonEvent* button) {
    }

    void onQuitEvent(SDL_QuitEvent* quit_event) {
        quit();
    }

    void onPaint(SDLRenderer* ren) {
        ren->clear(0x000000);
        if (state == EMenu) {
            drawMenu(ren);
        } else {
            drawSnake(ren);
            drawFood(ren);
            drawScore(ren);
            drawRecord(ren);
            if (state == EPause) {
                drawPause(ren);
            } else if (state == EDie) {
                drawDie(ren);
            } else if (state == ERecord) {
                drawNewRecord(ren);
            }
        }
        ren->present();
    }

    void onIdle() {
        if (state == EPlay) {
            snake.setDirecion(dir);
            snake.move();
            if (snake.crash()) {
                if (score > db.record) {
                    db.record = score;
                    state = ERecord;
                } else {
                    state = EDie;
                }
                setTimeout(100);
            } else if (snake.fetch(food)) {
                snake.eat(food);
                ++score;
                updateScore();
                generateFood();
                setTimeout(MAX_TIMEOUT - 10 * sqrt(score));
            }
        }
    }

};


#endif // GAME_WINDOW_H