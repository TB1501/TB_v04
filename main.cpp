#include "nwpwin.h"

// prepare class ("STATIC") for a ship

class STATIC : public vsite::nwp::window
{
	public:std::string class_name() override { return "STATIC"; }
};

class main_window : public vsite::nwp::window
{
protected:
	void on_left_button_down(POINT p) override { 
		//create ship if it doesn't exist yet
		// change current location
		
		location = p;
		if (!ship)
		{
			ship.create(*this, WS_CHILD | WS_VISIBLE | SS_CENTER, "X", 0, p.x, p.y, 50, 50);
		}
		else {
			SetWindowPos(ship, 0, p.x, p.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
	}
	void on_key_up(int vk) override {
		// mark ship (if exists) as "not moving"
		
		DWORD style = GetWindowLong(ship, GWL_STYLE);
		style &= ~WS_BORDER;


		if (ship) {
			SetWindowLong(ship, GWL_STYLE, style);
			SetWindowPos(ship, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOZORDER);
		}
	}
	void on_key_down(int vk) override {
		// if ship exists, move it depending on key and mark as "moving"

		RECT rc;
		GetClientRect(*this, &rc);
		int speed = GetKeyState(VK_CONTROL) < 0 ? 20 : 5;
		DWORD style1 = GetWindowLong(ship, GWL_STYLE);

		switch (vk)
		{
			case VK_UP:
			location.y = max(0, location.y - speed);
			break;

			case VK_DOWN:
				location.y = min(rc.bottom - 50, location.y + speed);
				break;

			case VK_LEFT:
				location.x = max(0, location.x - speed);
				break;

			case VK_RIGHT:
				location.x = min(rc.right - 50, location.x + speed);
				break;
		
			default:
				return;
		}
		style1 |= WS_BORDER;
		SetWindowLong(ship, GWL_STYLE, style1);
		SetWindowPos(ship, 0, location.x, location.y, 0, 0, SWP_NOSIZE | SWP_FRAMECHANGED | SWP_NOZORDER);
	}
	void on_destroy() override {
		::PostQuitMessage(0);
	}
private:
	STATIC ship;
	POINT location;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	vsite::nwp::application app;
	main_window w;
	w.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, "NWP 4");
	return app.run();
}
