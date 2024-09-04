// Includes libraries necessary for the code.
#include "emulator/src/lib_draw.cpp"
#include "image.h"

#define Y_COLOR COLOR(238, 194, 16)

void sensor(bool connected) {
	const char *mac = "XX:XX:XX:XX:XX:XX";
	const char *name = "Picopad BT Keyboard";
	const char *text = "Use Picopad keys";

	DrawImgRle(KeyboardImg_RLE, KeyboardImg_Pal, (WIDTH / 2) - (150 / 2), 30, 150, 150);
	DrawImgRle(IconBackImg_RLE, IconBackImg_Pal, WIDTH - 68 - 10, HEIGHT - 23 - 10, 68, 23);
	SelFont8x8();
	DrawText("BT Name: ", 10, 15, Y_COLOR);
	SelFont8x8();
	DrawText(name, 80, 15, COL_WHITE);

	int x = 0;
	int y = 0;

	if (connected) {
		DrawRect(284, 0, WIDTH - 284, 38, COL_BLACK);
		DrawImgRle(ConnectedImg_RLE, ConnectedImg_Pal, 284, 3, 30, 30);
		x = (WIDTH / 2) - (16 * 8) / 2;
		y = HEIGHT - 55;
		DrawText(text, x, y, COL_WHITE);

		x = 10;
		y = HEIGHT - 25;
		DrawText("MAC: ", x, y, Y_COLOR);
		SelFont8x8();
		DrawText(mac, x + (4 * 8), y, COL_WHITE);
	} else {
		DrawRect(284, 0, WIDTH - 284, 38, COL_BLACK);
		DrawImgRle(DconnectedImg_RLE, DconnectedImg_Pal, 284, 8, 30, 30);
		DrawRect(0, HEIGHT - 50, WIDTH - 68 - 10, 50, COL_BLACK);
	}
	DispUpdate();
}


int main() {
	// Creates a frame with given display width and height.
	FrameTex.create(DispWidth, DispHeight);
	FrameSprite.setTexture(FrameTex);

	bool connected = true;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			// Check if the window was closed, if yes terminate the main loop.
			if (event.type == sf::Event::Closed)
				window.close();

			// If a key was pressed.
			if (event.type == sf::Event::KeyPressed) {
				// If the 'C' key was pressed.
				if (event.key.code == sf::Keyboard::C) {
					// Toggle the connected variable.
					connected = !connected;
				}
				if (event.key.code == sf::Keyboard::Y) {
					window.close();
					return 0;
				}
			}
		}

		DrawClear();
		sensor(connected);

		// Clears the window, draws the frame sprite, and displays the window.
		window.clear();
		window.draw(FrameSprite);
		window.display();
	}

	return 0;
}
