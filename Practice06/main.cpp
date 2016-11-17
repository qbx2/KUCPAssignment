#include <process.h>
#include <iostream>
#include <Windows.h>

class KeyEvent {
private:
	HANDLE hIn;
	COORD KeyWhere;
	DWORD EventCount;
	INPUT_RECORD InRec;
	DWORD NumRead;
	int downKey;
public:
	KeyEvent() {
		hIn = GetStdHandle(STD_INPUT_HANDLE);
		EventCount = 1;
	}

	int getKey() {
		ReadConsoleInput(hIn, &InRec, 1, &NumRead);

		if(InRec.EventType == KEY_EVENT) {
			if(InRec.Event.KeyEvent.bKeyDown) {
				downKey = InRec.Event.KeyEvent.wVirtualKeyCode;
				return downKey;
			} else {
				return -1;
			}
		}

		return -1;
	}
};

using namespace std;
//
//unsigned int __stdcall mythread(void *) {
//	int i = 0;
//	while(1) {
//		cout << GetCurrentThreadId() << ": " << i++ << endl;
//		Sleep(500);
//	}
//	return 0;
//}
//
//unsigned int __stdcall keyEvent(void *) {
//	KeyEvent k;
//	int i;
//	while(1) {
//		i = k.getKey();
//		switch(i) {
//		case VK_LEFT:
//			cout << "Left key is pressed" << endl;
//			break;
//		case VK_UP:
//			cout << "Up key is pressed" << endl;
//			break;
//		case VK_RIGHT:
//			cout << "Right key is pressed" << endl;
//			break;
//		case VK_DOWN:
//			cout << "Down key is pressed" << endl;
//			break;
//
//		}
//	}
//	return 0;
//}

class Game {
private:
	int x;
	int y;
	int direction;
	void gotoXY(int x, int y) {
		COORD pos = { x, y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	}
public:
	Game() {
		x = 5;
		y = 5;
		direction = 0;
	}

	void setDirection(int i) {
		direction = i;
	}

	void move() {
		if(direction == VK_LEFT) {
			x--;
		} else if(direction == VK_UP) {
			y--;
		} else if(direction == VK_RIGHT) {
			x++;
		} else if(direction == VK_DOWN) {
			y++;
		}
		system("cls");
		this->gotoXY(x, y);
		cout << x << " " << y;
		cout << "X";
	}
};

Game *game = new Game();

unsigned int __stdcall mythread(void *) {
	int i = 0;
	while(1) {
		game->move();
		Sleep(500);
	}
	return 0;
}

unsigned int __stdcall keyEvent(void *) {
	KeyEvent k;
	int i;
	while(1) {
		i = k.getKey();
		if(i != -1) game->setDirection(i);
	}
	return 0;
}
int main() {
	HANDLE handleA, handleB;
	handleA = (HANDLE)_beginthreadex(0, 0, mythread, (void *)0, 0, 0);
	handleB = (HANDLE)_beginthreadex(0, 0, keyEvent, (void *)0, 0, 0);

	WaitForSingleObject(handleA, INFINITE);
	WaitForSingleObject(handleB, INFINITE);

	CloseHandle(handleA);
	CloseHandle(handleB);


}