/*

Console Game Engine by Christian Kosman for Intro to C++ M10A Final Project

*/

#include "gameEngine.h"

const string lowerLetters = "abcdefghijklmnopqrstuvwxyz";

CHAR_INFO *buffer;
CHAR_INFO *uiBuffer;
chrono::system_clock::time_point a = chrono::system_clock::now();
chrono::system_clock::time_point b = chrono::system_clock::now();
key keys[256];
double deltaTime;
unsigned long frame;
vector<sprite> spriteBank;
vector<sprite> charBank;
LPWSTR winTitle;
bool running = true;
int PIXEL_SCALE = 4;
config configuration;

/*
draw: places a character into the screen buffer provided its index and color

int index: the index of the screen buffer to replace
wchar_t character: the character to draw
WORD color: the color of the character
*/
void draw(int index, wchar_t character, WORD color) {
	if (index >= 0 && index < SCREEN_WIDTH * SCREEN_HEIGHT) {
		buffer[index].Char.UnicodeChar = character;
		buffer[index].Attributes = color;
	}
}

/*
drawUI: places a character into the UI buffer provided its index and color

int index: the index of the screen buffer to replace
wchar_t character: the character to draw
WORD color: the color of the character
*/
void drawUI(int index, wchar_t character, WORD color) {
	if (index >= 0 && index < SCREEN_WIDTH * SCREEN_HEIGHT) {
		uiBuffer[index].Char.UnicodeChar = character;
		uiBuffer[index].Attributes = color;
	}
}

/*
draw: places a character into the screen buffer provided its position and color

int x: the x position of the character to be drawn
int y: the y position of the character to be drawn
wchar_t character: the character to draw
WORD color: the color of the character
*/
void draw(int x, int y, wchar_t character, WORD color) {
	if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT && x >= 0 && y >= 0) {
		int index = SCREEN_WIDTH * y + x;
		draw(index, character, color);
	}
}

/*
draw: places a character into the UI buffer provided its position and color

int x: the x position of the character to be drawn
int y: the y position of the character to be drawn
wchar_t character: the character to draw
WORD color: the color of the character
*/
void drawUI(int x, int y, wchar_t character, WORD color) {
	if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT && x >= 0 && y >= 0) {
		int index = SCREEN_WIDTH * y + x;
		drawUI(index, character, color);
	}
}

/*
drawSprite: draws a sprite into the UI buffer

int x: x position of the sprite from the left side
int y: y position of the sprite from the top side
sprite spr: reference to a sprite object
*/
void drawSprite(int x, int y, sprite spr) {
	int width = spr.width;
	int height = spr.height;
	for (int i = 0; i < width*height; i++) {
		drawUI(x + i % width, y + floor(i / width), spr.chars[i], spr.colors[i]);
	}
}

/*
drawSpriteScaled: draws a scaled sprite into the UI buffer

int x: x position of the sprite from the left side
int y: y position of the sprite from the top side
double scale: the scale of the sprite
sprite spr: reference to a sprite object
*/
void drawSpriteScaled(int x, int y, double scale, sprite spr) {
	int width = spr.width;
	int height = spr.height;
	int texX = 0, texY = 0;
	for (int y2 = 0; y2 < height*scale; y2++) {
		texY = (double)y2 / scale;
		for (int x2 = 0; x2 < width*scale; x2++) {
			texX = (double)x2 / scale;
			int index = texY * width + texX;
			drawUI(x2+x, y2+y, spr.chars[index], spr.colors[index]);
		}
	}
}

/*
drawSpriteTransparent: draws a transparent sprite into the UI buffer (transparent color upon loading sprite is rgb(69, 4, 20))

int x: x position of the sprite from the left side
int y: y position of the sprite from the top side
sprite spr: reference to a sprite object
*/
void drawSpriteTransparent(int x, int y, sprite spr) {
	int width = spr.width;
	int height = spr.height;
	for (int i = 0; i < width*height; i++) {
		if (spr.colors[i] > 0)
			drawUI(x + i % width, y + floor(i / width), spr.chars[i], spr.colors[i]);
	}
}

/*
drawSpriteScaledTransparent: draws a scaled transparent sprite into the UI buffer

int x: x position of the sprite from the left side
int y: y position of the sprite from the top side
double scale: the scale of the sprite
sprite spr: reference to a sprite object
*/
void drawSpriteScaledTransparent(int x, int y, double scale, sprite spr) {
	int width = spr.width;
	int height = spr.height;
	int texX = 0, texY = 0;
	for (int y2 = 0; y2 < height*scale; y2++) {
		texY = (double)y2 / scale;
		for (int x2 = 0; x2 < width*scale; x2++) {
			texX = (double)x2 / scale;
			int index = texY * width + texX;
			if (spr.colors[index] > 0)
				drawUI(x2 + x, y2 + y, spr.chars[index], spr.colors[index]);
		}
	}
}

/*
line: draws a line to the screen provided 2 points and color

int x1: the first point's x of the line
int y1: the first point's y of the line
int x2: the second point's x of the line
int y2: the second point's y of the line
wchar_t character: the character to draw the line in
WORD color: the color of the line
*/
void line(int x1, int y1, int x2, int y2, wchar_t character, WORD color) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	int steps = 0;

	if (abs(dx) > abs(dy)) {
		steps = abs(dx);
	} else {
		steps = abs(dy);
	}

	float xinc = dx / (float)steps;
	float yinc = dy / (float)steps;

	float x = x1, y = y1;

	for (int i = 0; i < steps; i++) {
		draw(round(x), round(y), character, color);
		x += xinc;
		y += yinc;
	}
}

/*
vertLine: draws a vertical line given an x position and 2 y positions

int x: the x position of the line
int y1: the first y position of the line
int y2: the second y position of the line
wchar_t character: the character to draw the line in
WORD color: the color of the line to draw
*/
void vertLine(int x, int y1, int y2, wchar_t character, WORD color) {
	int diff = y2 - y1;
	int steps = abs(diff);
	int y = y1;
	for (int i = 0; i < steps + 1; i++) {
		draw(x, y, character, color);
		if(steps > 0)
			y += diff / steps;
	}
}

/*
vertLineUI: draws a vertical line given an x position and 2 y positions to the UI buffer

int x: the x position of the line
int y1: the first y position of the line
int y2: the second y position of the line
wchar_t character: the character to draw the line in
WORD color: the color of the line to draw
*/
void vertLineUI(int x, int y1, int y2, wchar_t character, WORD color) {
	int diff = y2 - y1;
	int steps = abs(diff);
	int y = y1;
	for (int i = 0; i < steps + 1; i++) {
		drawUI(x, y, character, color);
		if (steps > 0)
			y += diff / steps;
	}
}

/*
horizLine: draws a vertical line given an x position and 2 y positions

int y: the y position of the line
int x1: the first x position of the line
int x2: the second x position of the line
wchar_t character: the character to draw the line in
WORD color: the color of the line to draw
*/
void horizLine(int y, int x1, int x2, wchar_t character, WORD color) {
	int diff = x2 - x1;
	int steps = abs(diff);
	int x = x1;
	for (int i = 0; i < steps + 1; i++) {
		draw(x, y, character, color);
		if (steps > 0)
			x += diff / steps;
	}
}

/*
horizLineUI: draws a vertical line given an x position and 2 y positions to the UI buffer

int y: the y position of the line
int x1: the first x position of the line
int x2: the second x position of the line
wchar_t character: the character to draw the line in
WORD color: the color of the line to draw
*/
void horizLineUI(int y, int x1, int x2, wchar_t character, WORD color) {
	int diff = x2 - x1;
	int steps = abs(diff);
	int x = x1;
	for (int i = 0; i < steps + 1; i++) {
		drawUI(x, y, character, color);
		if (steps > 0)
			x += diff / steps;
	}
}

/*
fillScreen: fills the entire screen buffer with a character and its color

wchar_t character: the character to fill the screen buffer with
WORD color: the color of the characters filled
*/
void fillScreen(wchar_t character, WORD color) {
	for (int i = 0; i < BUFFER_SIZE; i++) {
		draw(i, character, color);
	}
}

/*
clearScreen: zeros the screen buffer
*/
void clearScreen() {
	memset(buffer, 0, BUFFER_SIZE*sizeof(CHAR_INFO));
}

/*
clearUI: zeros the UI buffer
*/
void clearUI() {
	memset(uiBuffer, 0, BUFFER_SIZE * sizeof(CHAR_INFO));
}

/*
printText: prints text to the uiBuffer using character sprites located at resources/textures/chars

char* text: a pointer to a string to be printed
int x: the x position of the text
int y: the y position of the text
LETTERCASE letterCase: the case of the text (UPPER, DEFAULT, LOWER)
*/
void printText(char* text, int x, int y, LETTERCASE letterCase, char* font) {
	for (int i = 0; i < strlen(text); i++) {
		if (text[i] == ' ') {
			x += 4;
		} else {
			char* spritePath = new char[MAX_PATH];
			int character = 0;
			if (letterCase == UPPER) character = toupper(text[i]);
			else if (letterCase == LOWER) character = tolower(text[i]);
			else character = text[i];
			string charString(1, character);
			if (text[i] == '.') charString = "DEC";
			if (text[i] == '/') charString = "FORWARD_SLASH";
			if (text[i] == ':') charString = "COLON";
			if (islower(character))
				sprintf(spritePath, "resources\\textures\\%s\\%ss.bmp", font, charString.c_str());
			else
				sprintf(spritePath, "resources\\textures\\%s\\%s.bmp", font, charString.c_str());
			
			sprite letter = getCharSprite(string(1, character));
			if (letter.valid == INVALID_SPRITE.valid) {
				loadSprite(spritePath, string(1, character), 1);
				letter = getCharSprite(string(1, character));
			}
			
			if (letter.valid) {
				drawSpriteTransparent(x, y, letter);
				x += letter.width;
			}

			delete[] spritePath;
		}
	}
}

void printText(char* text, int x, int y, LETTERCASE letterCase) {
	printText(text, x, y, letterCase, "chars");
}

/*
mergeBuffers: combines the UI and main buffers together
*/
void mergeBuffers() {
	for (int i = 0; i < BUFFER_SIZE; i++) {
		if (uiBuffer[i].Attributes > 0) {
			buffer[i].Char.UnicodeChar = uiBuffer[i].Char.UnicodeChar;
			buffer[i].Attributes = uiBuffer[i].Attributes - 1;
		}
	}
}

/*
exitGame: exits the main loop (surprising huh)
*/
void exitGame() {
	running = false;
}

/*
loadSprite: loads a sprite from a 24-bit .bmp file and tries to match colors to console-compatible colors

char* fileName: the path to the file relative to the executable's directory
string name: the name to assign to the sprite
bool characterBank: load sprite into the character sprite bank?
*/
bool loadSprite(char* fileName, string name, bool characterBank) {
	char curDir[MAX_PATH];
	GetModuleFileNameA(NULL, curDir, MAX_PATH);
	string::size_type pos = string(curDir).find_last_of("\\/");
	char filePath[MAX_PATH];
	sprintf(filePath, "%s\\%s", (string(curDir).substr(0, pos)).c_str(), fileName);

	FILE* f = fopen(filePath, "rb");
	if (f != NULL) {
		unsigned char header[54];
		fread(header, sizeof(unsigned char), 54, f);

		int fileSize, offset, width, height, depth;
		memcpy(&fileSize, header + 2, sizeof(int));
		memcpy(&offset, header + 10, sizeof(int));
		memcpy(&width, header + 18, sizeof(int));
		memcpy(&height, header + 22, sizeof(int));
		memcpy(&depth, header + 28, sizeof(int));

		int bpp = 3;
		// This allows bmps with odd widths to load correctly
		int rowPadded = (width * bpp + 3) & (~3);
		int dataSize = bpp * width * height;
		unsigned char* pixels = new unsigned char[rowPadded];

		sprite output = {
			width,
			height,
			new wchar_t[width * height],
			new WORD[width * height],
			true,
			name
		};

		for (int i = 0; i < height; i++) {
			fread(pixels, sizeof(unsigned char), rowPadded, f);
			for (int j = 0; j < width*bpp; j += bpp) {
				unsigned char b = pixels[j];
				unsigned char g = pixels[j + 1];
				unsigned char r = pixels[j + 2];

				WORD color = 0;
				wchar_t shade = PIXEL_SHADE0;

				if (b > 0) color |= FOREGROUND_BLUE;
				if (g > 0) color |= FOREGROUND_GREEN;
				if (r > 0) color |= FOREGROUND_RED;

				if (r == 69 && g == 4 && b == 20) {
					color = 0;
				}
				else {
					color |= FOREGROUND_INTENSITY;

					WORD avg;
					avg = 0;
					WORD avgColor = 0;
					avgColor = (r + g + b) / 3;
					if (b > avg) avg = b;
					if (g > avg) avg = g;
					if (r > avg) avg = r;

					if (avg < 256) shade = PIXEL_SHADE0;
					if (avg < 224 && avg >= 192) color ^= FOREGROUND_INTENSITY;
					if (avg < 192) shade = PIXEL_SHADE1;
					if (avg < 160 && avg >= 128) color ^= FOREGROUND_INTENSITY;
					if (avg < 128) shade = PIXEL_SHADE2;
					if (avg < 96 && avg >= 64) color ^= FOREGROUND_INTENSITY;
					if (avg < 64) shade = PIXEL_SHADE3;
					if (avg < 32) color ^= FOREGROUND_INTENSITY;

					color++;
				}

				output.colors[(height - 1 - i) * width + j / bpp] = color;
				output.chars[(height - 1 - i) * width + j / bpp] = shade;
			}
		}

		delete[] pixels;
		fclose(f);

		if(characterBank)
			charBank.push_back(output);
		else
			spriteBank.push_back(output);
		return true;
	} else {
		return false;
	}
}

bool loadSprite(char* fileName, string name) {
	return loadSprite(fileName, name, false);
}

/*
getSprite: returns a sprite by name from the sprite bank

string name: the name of the sprite
*/
sprite getSprite(string name) {
	for (int i = 0; i < spriteBank.size(); i++) {
		if (spriteBank[i].name == name) return spriteBank[i];
	}
	return INVALID_SPRITE;
}

/*
getCharSprite: returns a character sprite by name from the character sprite bank

string name: the name of the sprite
*/
sprite getCharSprite(string name) {
	for (int i = 0; i < charBank.size(); i++) {
		if (charBank[i].name == name) return charBank[i];
	}
	return INVALID_SPRITE;
}

/*
lerp: linearly interpolates between 2 values

double a: the first value
double b: the second value
double f: the "between" value
*/
double lerp(double a, double b, double f) {
	return (a * (1.0 - f)) + (b * f);
}

/*
setWindowTitle: sets the title of the console window

LPWSTR title: the title of the window
*/
void setWindowTitle(LPWSTR title) {
	winTitle = title;
}

/*
loadConfig: loads a configuration file used for game settings

char* defaultConfig: if the config file doesn't exist, write this to a new file
*/
void loadConfig(char* defaultConfig) {
	ifstream config("settings.ini");
	if (config.is_open()) {
		char attribute[256];
		char value[256];
		while (config.getline(attribute, 256, '=') && config.getline(value, 256, '\n')) {
			configuration.attributes.push_back(string(attribute));
			configuration.values.push_back(string(value));
		}
	} else {
		ofstream defConf("settings.ini");
		defConf << defaultConfig;
		defConf.close();
		loadConfig(defaultConfig);
	}

	config.close();
}

/*
getConfigValue: returns the value of the specified configuration attribute

char* attribute: the attribute to read
*/
string getConfigValue(char* attribute) {
	for (int i = 0; i < configuration.attributes.size(); i++) {
		if (strcmp(configuration.attributes[i].c_str(), attribute) == 0)
			return string(configuration.values[i]);
	}
}

/*
setConfigValue: sets a configuration value and writes it to the config file

const char* attribute: the attribute to write to
const char* value: the value to override
*/
void setConfigValue(const char* attribute, const char* value) {
	string newCfg = "";
	for (int i = 0; i < configuration.attributes.size(); i++) {
		if (configuration.attributes[i] == attribute) {
			configuration.values[i] = value;
		}
		newCfg.append(configuration.attributes[i]);
		newCfg.append("=");
		newCfg.append(configuration.values[i]);
		newCfg.append("\n");
	}

	ofstream conf("settings.ini");
	conf << newCfg;
	conf.close();
}

/*
setupWindow: sets up the console window's properties and style
*/
void setupWindow() {
	frame = 0;

	// Initialize the screen buffer
	buffer = new CHAR_INFO[BUFFER_SIZE];
	memset(buffer, 0, sizeof(CHAR_INFO) * BUFFER_SIZE);
	// Initialize the UI buffer
	uiBuffer = new CHAR_INFO[BUFFER_SIZE];
	memset(uiBuffer, 0, sizeof(CHAR_INFO) * BUFFER_SIZE);

	HWND consoleWindow = GetConsoleWindow();
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE consoleHandleIn = GetStdHandle(STD_INPUT_HANDLE);

	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(consoleHandle, &cci);
	cci.bVisible = false;
	SetConsoleCursorInfo(consoleHandle, &cci);

	SMALL_RECT winRect = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(consoleHandle, TRUE, &winRect);

	// Set console text buffer
	COORD bufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	SetConsoleScreenBufferSize(consoleHandle, bufferSize);
	SetConsoleActiveScreenBuffer(consoleHandle);

	// Set custom console font so it can be square pixels
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = PIXEL_SCALE;
	cfi.dwFontSize.Y = PIXEL_SCALE;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(consoleHandle, FALSE, &cfi);

	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(consoleHandle, &bufferInfo);

	// Set the screen size and text buffer
	winRect = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
	SetConsoleWindowInfo(consoleHandle, TRUE, &winRect);

	SetConsoleActiveScreenBuffer(consoleHandle);

	DWORD consoleMode;
	GetConsoleMode(consoleHandleIn, &consoleMode);
	SetConsoleMode(consoleHandleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING | (consoleMode & ~ENABLE_QUICK_EDIT_MODE));

	DWORD consoleStyle = GetWindowLong(consoleWindow, GWL_STYLE);
	SetWindowLong(consoleWindow, GWL_STYLE, consoleStyle & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	ShowWindow(consoleWindow, SW_SHOW);

	// Center window in screen
	RECT rectWindow;
	GetWindowRect(consoleWindow, &rectWindow);
	int posX, posY;
	posX = GetSystemMetrics(SM_CXSCREEN) / 2 - (rectWindow.right - rectWindow.left) / 2;
	posY = GetSystemMetrics(SM_CYSCREEN) / 2 - (rectWindow.bottom - rectWindow.top) / 2;
	MoveWindow(consoleWindow, posX, posY, rectWindow.right - rectWindow.left, rectWindow.bottom - rectWindow.top, TRUE);

	winTitle = L"Christian Kosman's Game Engine";

	// Determine max milliseconds between frames
	double framerate = 1000.0 / FPS;

	double smoothing = 0.9;

	onWindowCreated();

	// Main game loop
	while (running) {
		// This part handles timing on the main thread
		a = chrono::system_clock::now();
		chrono::duration<double, milli> work_time = a - b;

		if (work_time.count() < framerate) {
			chrono::duration<double, milli> msDiff(framerate - work_time.count());
			auto msDiffDuration = chrono::duration_cast<chrono::milliseconds>(msDiff);
			this_thread::sleep_for(chrono::milliseconds(msDiffDuration.count()));
		}

		b = chrono::system_clock::now();
		chrono::duration<double, milli> sleep_time = b - a;

		// Calculate delta time (time between previous frame and this frame)
		deltaTime = (work_time + sleep_time).count() / 1000.0;

		// Capture key input
		for (int i = 0; i < 256; i++) {
			keys[i].newState = GetAsyncKeyState(i);

			keys[i].released = false;
			keys[i].pressed = false;

			if (keys[i].oldState != keys[i].newState) {
				if (keys[i].newState & 0x8000) {
					keys[i].pressed = !keys[i].held;
					keys[i].held = true;
				} else {
					keys[i].released = true;
					keys[i].held = false;
				}
			}

			keys[i].oldState = keys[i].newState;
		}

		// Call update function once per frame
		update();

		// Draw screen and set window title
		SetConsoleTitle(winTitle);
		mergeBuffers();
		WriteConsoleOutput(consoleHandle, buffer, { SCREEN_WIDTH, SCREEN_HEIGHT }, { 0, 0 }, &winRect);

		frame++;
	}
}