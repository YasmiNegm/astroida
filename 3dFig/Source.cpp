#include <Windows.h>
#include <math.h>
#define Pi 3.14159
#define N 240 
#define p 20 

using namespace std;

int par1 = -10, par2 = 10; 
double ugol = Pi / 8; 
double parm1 = 0.9, parm2 = 1.1;

double Cx = 0.0, Cy = 0.0, Cz = 0.0; 
double minX = 1000.0, maxX = -1000.0, minY = 1000.0, maxY = -1000.0, minZ = 1000.0, maxZ = -1000.0;

void multiply(double matrix[][N][4], double m[4][4]) {
	float res[4];
	for (int t = 0; t < p; t++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < 4; j++) {
				res[j] = 0;
				for (int k = 0; k < 4; k++)
					res[j] = res[j] + matrix[t][i][k] * m[k][j];
			}
			for (int j = 0; j < 4; j++)
				matrix[t][i][j] = res[j];
		}
	}
}

void preobr(double matrix[][N][4]) {
	for (int i = 0; i < p; i++) {
		for (int j = 0; j < N; j++) {
			matrix[i][j][0] = matrix[i][j][0] / matrix[i][j][3];
			matrix[i][j][1] = matrix[i][j][1] / matrix[i][j][3];
		}
	}
}

void proection(double matrix[][N][4], double add[][N][4]) {
	for (int i = 0; i < p; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < 4; k++) {
				add[i][j][k] = matrix[i][j][k];
			}
		}
	}
	double m[4][4] = { {-0.93,-0.13,0.35,0},{0,-0.93,-0.35,0},{-0.38,0.33,-0.36,0},{0,0,0,1} };
	multiply(add, m);
	preobr(add);
}

void change_centr(double matrix[][N][4]) {
	minX = 1000.0; maxX = -1000.0; minY = 1000.0; maxY = -1000.0; minZ = 1000.0; maxZ = -1000.0;

	for (int i = 0; i < p; i++) {
		for (int j = 0; j < N; j++) {
			if (matrix[i][j][0] < minX)
				minX = matrix[i][j][0];
			if (matrix[i][j][0] > maxX)
				maxX = matrix[i][j][0];
			if (matrix[i][j][1] < minY)
				minY = matrix[i][j][1];
			if (matrix[i][j][1] > maxY)
				maxY = matrix[i][j][1];
			if (matrix[i][j][2] < minZ)
				minZ = matrix[i][j][2];
			if (matrix[i][j][2] > maxZ)
				maxZ = matrix[i][j][2];
		}
	}

	Cx = (minX + maxX) / 2;
	Cy = (minY + maxY) / 2;
	Cz = (minZ + maxZ) / 2;
}
 
void move(double matrix[][N][4], double d, int param) {
	double m[4][4] = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };
	switch (param) {
	case 1:
		m[3][0] = d;
		break;
	case 2:
		m[3][1] = d;
		break;
	case 3:
		m[3][2] = d;
		break;
	}
	multiply(matrix, m);
	change_centr(matrix);
}

void scale(double matrix[][N][4], double d, int param) {
	double m[4][4] = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };
	switch (param) {
	case 1:
		m[0][0] = d;
		break;
	case 2:
		m[1][1] = d;
		break;
	case 3:
		m[2][2] = d;
		break;
	}
	multiply(matrix, m);
}

void turn(double matrix[][N][4], int param) {
	double m[4][4] = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };
	switch (param) {
	case 1:
		m[1][1] = cos(ugol);
		m[1][2] = sin(ugol);
		m[2][1] = -sin(ugol);
		m[2][2] = cos(ugol);
		break;
	case 2:
		m[0][0] = cos(ugol);
		m[2][0] = sin(ugol);
		m[0][2] = -sin(ugol);
		m[2][2] = cos(ugol);
		break;
	case 3:
		m[0][0] = cos(ugol);
		m[0][1] = sin(ugol);
		m[1][0] = -sin(ugol);
		m[1][1] = cos(ugol);
		break;
	}
	multiply(matrix, m);
	change_centr(matrix);
}

void turn_(double matrix[][N][4], int param) {
	double m1[4][4] = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };
	double m2[4][4] = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };
	m1[3][0] = -Cx;
	m1[3][1] = -Cy;
	m1[3][2] = -Cz;
	multiply(matrix, m1);
	switch (param) {
	case 1:
		m2[1][1] = cos(ugol);
		m2[1][2] = sin(ugol);
		m2[2][1] = -sin(ugol);
		m2[2][2] = cos(ugol);
		break;
	case 2:
		m2[0][0] = cos(ugol);
		m2[2][0] = sin(ugol);
		m2[0][2] = -sin(ugol);
		m2[2][2] = cos(ugol);
		break;
	case 3:
		m2[0][0] = cos(ugol);
		m2[0][1] = sin(ugol);
		m2[1][0] = -sin(ugol);
		m2[1][1] = cos(ugol);
		break;
	}
	multiply(matrix, m2);
	m1[3][0] = Cx;
	m1[3][1] = Cy;
	m1[3][2] = Cz;
	multiply(matrix, m1);
}


LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;

	static double fig1[p][N][4];
	static double fig2[p][N][4];

	double t = 0.0, alfa = 20;
	double h1 = 2 * Pi / N, h2 = (580 - 40) / p, h3 = (90 - 20) / p;
	int z = 40;

	switch (uMsg) {
	case WM_CREATE:
	{
		GetClientRect(hwnd, &rect);
		for (int i = 0; i < p; i++) {
			for (int j = 0; j < N; j++) {
				fig1[i][j][0] = 50 * (tan(alfa*Pi / 180)*(pow(cos(t), 3)));
				fig1[i][j][1] = 50 * (tan(alfa*Pi / 180)*(pow(sin(t), 3)));
				fig1[i][j][2] = z;
				fig1[i][j][3] = 1;

				if (fig1[i][j][0] < minX)
					minX = fig1[i][j][0];

				if (fig1[i][j][0] > maxX)
					maxX = fig1[i][j][0];

				if (fig1[i][j][1] < minY)
					minY = fig1[i][j][1];
				if (fig1[i][j][1] > maxY)
					maxY = fig1[i][j][1];
				if (fig1[i][j][2] < minZ)
					minZ = fig1[i][j][2];
				if (fig1[i][j][2] > maxZ)
					maxZ = fig1[i][j][2];

				t += h1;
			}
			t = 0.0;
			z += h2;
			alfa += h3;
		}

		Cx = (minX + maxX) / 2;
		Cy = (minY + maxY) / 2;
		Cz = (minZ + maxZ) / 2;
		break;
	}
	case WM_PAINT:
	{
		HDC hdc1;
		hdc1 = BeginPaint(hwnd, &ps);
		proection(fig1, fig2);
		GetClientRect(hwnd, &rect);

		int x = rect.right / 2, y = rect.bottom / 2;
		double p1 = 0, p2 = 0;
		for (int i = 0; i < p; i++) {
			for (int j = 0; j < N; j++) {

				if ((j + N / 6) % (N / 3) == 0) { p1 = 0; p2 = 0; }
				p1 += 1.725;
				p2 += 1.525;
				HPEN realPen = CreatePen(PS_SOLID, 2, RGB(p1, p2, 255));

				SelectObject(hdc1, realPen);
				if (j != N - 1) {

					MoveToEx(hdc1, fig2[i][j][0] + x, fig2[i][j][1] + y, NULL);
					LineTo(hdc1, fig2[i][j + 1][0] + x, fig2[i][j + 1][1] + y);
				}
				else {
					MoveToEx(hdc1, fig2[i][j][0] + x, fig2[i][j][1] + y, NULL);
					LineTo(hdc1, fig2[i][0][0] + x, fig2[i][0][1] + y);
				}
		
				if (i < p - 1) {
					MoveToEx(hdc1, fig2[i][j][0] + x, fig2[i][j][1] + y, NULL);
					LineTo(hdc1, fig2[i + 1][j][0] + x, fig2[i + 1][j][1] + y);
				}
				DeleteObject(realPen);
			}
		}
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_RIGHT: //движение по оси X вправо  ->
			move(fig1, par1, 1);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case VK_LEFT: //движение по оси X влево <-
			move(fig1, par2, 1);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case VK_UP: // движение по оси Y вверх - стрелка вверх
			move(fig1, par2, 2);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case VK_DOWN: // движение по оси Y вниз - стрелка вниз
			move(fig1, par1, 2);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case 81: // движение по оси Z вперед - клавиша Q
			move(fig1, par2, 3);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case 87: // движение по оси Z назад - клавиша W
			move(fig1, par1, 3);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case 69: // масштабирование ось X(увеличение)- клавиша E
			scale(fig1, parm2, 1);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case 82: // масштабирование ось X(уменьшение)- клавиша R
			scale(fig1, parm1, 1);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case 84: // масштабирование ось Y(увеличение)- клавиша T
			scale(fig1, parm2, 2);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case 89: // масштабирование ось Y(уменьшение)- клавиша Y
			scale(fig1, parm1, 2);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case 65: // масштабирование ось Z(увеличение)- клавиша A
			scale(fig1, parm2, 3);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case 83: // масштабирование ось Z(уменьшение)- клавиша S
			scale(fig1, parm1, 3);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case 90: // поворот по оси X - клавиша Z
			turn(fig1, 1);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case 88: // поворот по оси Y - клавиша X
			turn(fig1, 2);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case 67: // поворот по оси Z - клавиша C
			turn(fig1, 3);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case 86: // поворот вокруг себя по оси Х - клавиша V
			turn_(fig1, 1);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case 68: // поворот вокруг себя по оси Y- клавиша D
			turn_(fig1, 2);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		case 66: // поворот вокруг себя по оси Z- клавиша В
			turn_(fig1, 3);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hwnd;
	MSG msg;
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "WinClass";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Cannot register class", "ERROR", MB_OK);
		return 0;
	}
	hwnd = CreateWindowEx(NULL, "WinClass", "Астроида", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);
	if (!hwnd) {
		MessageBox(NULL, "Cannot create window", "ERROR", MB_OK);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

