#ifndef GAMEENGINEH
#define GAMEENGINEH

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define _WIN32_WINNT 0x0500

#include <iostream>
#include <string>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>

using namespace std;

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128
#define BUFFER_SIZE SCREEN_WIDTH*SCREEN_HEIGHT
#define FPS 60
#define PIXEL_SHADE0 0x2588
#define PIXEL_SHADE1 0x2593
#define PIXEL_SHADE2 0x2592
#define PIXEL_SHADE3 0x2591
#define DEBUG_MODE 0
#define PI 3.14159265

struct vec2 {
	double x;
	double y;
};
struct vec2Int {
	int x;
	int y;
};
struct key {
	short newState;
	short oldState;
	bool pressed;
	bool released;
	bool held;
};
struct sprite {
	int width;
	int height;
	wchar_t* chars;
	WORD* colors;
	bool valid;
	string name;
};
struct config {
	vector<string> attributes;
	vector<string> values;
};

const sprite INVALID_SPRITE = { NULL, NULL, NULL, NULL, false, "INVALID_SPRITE" };

//Screen buffer
extern CHAR_INFO *buffer;
extern CHAR_INFO *uiBuffer;
extern chrono::system_clock::time_point a;
extern chrono::system_clock::time_point b;
extern key keys[256];
extern double deltaTime;
extern unsigned long frame;
extern int PIXEL_SCALE;

void draw(int x, int y, wchar_t character, WORD color);
void draw(int index, wchar_t character, WORD color);
void drawUI(int x, int y, wchar_t character, WORD color);
void drawSprite(int x, int y, sprite spr);
void drawSpriteTransparent(int x, int y, sprite spr);
void mergeBuffers();
void line(int x1, int y1, int x2, int y2, wchar_t character, WORD color);
void vertLine(int x, int y1, int y2, wchar_t character, WORD color);
void horizLine(int y, int x1, int x2, wchar_t character, WORD color);
void fillScreen(wchar_t character, WORD color);
void clearScreen();
void clearUI();
void printText(char* text, int x, int y);
bool loadSprite(char* fileName, string name);
sprite getSprite(string name);
double lerp(double a, double b, double f);

void loadConfig(char* defaultConfig);
string getConfigValue(char* attribute);
void setConfigValue(const char* attribute, const char* value);
void setupWindow();
void setWindowTitle(LPWSTR title);
void update();
void onWindowCreated();
void exitGame();

#endif