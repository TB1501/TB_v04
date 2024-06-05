#include "nwpwin.h"

// : prepare class ("STATIC") for a ship

class Static : public vsite::nwp::window {
public:
	std::string class_name() override {
		return "Static";
	}
};

static const int ship_size = 25;
static const DWORD style = WS_CHILD | WS_VISIBLE | SS_CENTER;
class main_window : public vsite::nwp::window
{
protected:
	void on_left_button_down(POINT p) override {
		// : create ship if it doesn't exist yet
		// : change current location
		poz = p;
		if (!ship) {
			ship.create(*this, style, "X", 0, p.x, p.y, ship_size, ship_size);
		}
		else
		{
			SetWindowPos(ship, 0, p.x, p.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
	}
	void on_key_up(int vk) override {
		// : mark ship (if exists) as "not moving"
		DWORD style = GetWindowLong(ship, GWL_STYLE);
		style &= ~WS_BORDER;

		SetWindowLong(ship, GWL_STYLE, style);
		SetWindowPos(ship, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE);
	}
	void on_key_down(int vk) override {
		RECT window_border;
		GetClientRect(*this, &window_border);

		int speed = GetAsyncKeyState(VK_CONTROL) ? 50 : 10;

		if (ship)
		{
			switch (vk)
			{
			case VK_UP:
				poz.y = max(poz.y - speed, window_border.top);
				break;
			case VK_DOWN:
				poz.y = min(poz.y + speed, window_border.bottom - ship_size);
				break;
			case VK_LEFT:
				poz.x = max(poz.x - speed, window_border.left);
				break;
			case VK_RIGHT:
				poz.x = min(poz.x + speed, window_border.right - ship_size);
				break;
			default:
				return;
			}

			DWORD style = GetWindowLong(ship, GWL_STYLE);
			style |= WS_BORDER;

			SetWindowLong(ship, GWL_STYLE, style);
			SetWindowPos(ship, 0, poz.x, poz.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
		}
	}
	void on_destroy() override {
		::PostQuitMessage(0);
	}
private:
	Static ship;
	POINT poz;

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	vsite::nwp::application app;
	main_window w;
	w.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, "NWP 4");
	return app.run();
}