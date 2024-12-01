#include "emulator/src/lib_draw.h"
#include "../src/include.h"

#include <SFML/Graphics.hpp>

extern sf::Texture FrameTex;
extern sf::Sprite FrameSprite;
extern sf::RenderWindow window;

// HID key map array
// The keyboard layout and the image of the virtual keyboard were taken from the JRPN emulator – Jovial Reverse Polish
// Notation Calculators - https://jrpn.jovial.com/
constexpr uint16_t hid_key_map[] = {
	sf::Keyboard::A, sf::Keyboard::B, sf::Keyboard::C, sf::Keyboard::D, sf::Keyboard::E, sf::Keyboard::F,
	sf::Keyboard::Numpad7, sf::Keyboard::Numpad8, sf::Keyboard::Numpad9, sf::Keyboard::Divide, sf::Keyboard::U,
	sf::Keyboard::T, sf::Keyboard::I, sf::Keyboard::Z, sf::Keyboard::K, sf::Keyboard::L, sf::Keyboard::Numpad4,
	sf::Keyboard::Numpad5, sf::Keyboard::Numpad6, sf::Keyboard::Multiply, sf::Keyboard::LBracket, sf::Keyboard::RBracket,
	sf::Keyboard::V, sf::Keyboard::Y, sf::Keyboard::Backspace, sf::Keyboard::Enter, sf::Keyboard::Numpad1,
	sf::Keyboard::Numpad2, sf::Keyboard::Numpad3, sf::Keyboard::Subtract, sf::Keyboard::O, sf::Keyboard::M,
	sf::Keyboard::G, sf::Keyboard::S, sf::Keyboard::R, sf::Keyboard::Numpad0, sf::Keyboard::Period, sf::Keyboard::H,
	sf::Keyboard::Add
};

int get_key_idx(const uint16_t key) {
	for (int i = 0; i < sizeof(hid_key_map); i++) {
		if (key == hid_key_map[i]) {
			return i;
		}
	}
	return -1;
}

int main() {
	FrameTex.create(DispWidth, DispHeight);
	FrameSprite.setTexture(FrameTex);

	sf::FloatRect buttonAreas[40];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 10; j++) {
			int idx = i * 10 + j;
			if (idx != 25) {
				if (idx == 35) {
					buttonAreas[idx] = sf::FloatRect(col_start[j], ROW_START_Y + (i * ROW_HEIGHT) - ROW_HEIGHT, BUTTON_W,
					                                 ROW_HEIGHT + BUTTON_H);
				} else {
					buttonAreas[idx] = sf::FloatRect(col_start[j], ROW_START_Y + (i * ROW_HEIGHT), BUTTON_W, BUTTON_H);
				}
			}
		}
	}

	DrawClearCol(COL_BLACK);
	SelFont8x8();

	sim_display_init(nullptr);
	sim_t *sim = sim_init(nullptr, nullptr, nullptr);
	sim_start(sim);

	while (window.isOpen()) {
		sf::Event event{};
		while (window.pollEvent(event)) {
			// Handle mouse click
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				// Get mouse position relative to the window
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
				for (int i = 0; i < 40; i++) {
					if (buttonAreas[i].contains(mousePosF)) {
						sim_key(sim, i, true);
						break;
					}
				}
			}
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				// Get mouse position relative to the window
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
				for (int i = 0; i < 40; i++) {
					if (buttonAreas[i].contains(mousePosF)) {
						sim_key(sim, i, false);
						break;
					}
				}
			}

			// Check if the window was closed, if yes terminate the main loop.
			if (event.type == sf::Event::Closed)
				window.close();

			// If a key was pressed.
			if (event.type == sf::Event::KeyPressed) {
				if (int idx = get_key_idx(event.key.code); idx != -1) {
					sim_key(sim, idx, true);
				}
			}
			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
				if (int idx = get_key_idx(event.key.code); idx != -1) {
					sim_key(sim, idx, false);
				}
			}
		}

		sim_run(sim);

		window.clear();
		window.draw(FrameSprite);

		window.display();
	}

	return 0;
}
