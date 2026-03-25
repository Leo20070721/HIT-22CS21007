#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <tchar.h>

#include <algorithm>
#include <cmath>
#include <ctime>
#include <vector>

// 马匹实体：保存位置、速度、缩放、步态相位和毛色。
struct Horse {
	float x;
	float y;
	float speed;
	float scale;
	float phase;
	COLORREF bodyColor;
};

// 画布尺寸与常量。
constexpr int kWidth = 1280;
constexpr int kHeight = 720;
constexpr float kPi = 3.14159265f;

// 生成[left, right]区间的随机整数。
int RandInt(int left, int right) {
	return left + rand() % (right - left + 1);
}

// 生成[left, right]区间的随机浮点数。
float RandFloat(float left, float right) {
	return left + (right - left) * (rand() / static_cast<float>(RAND_MAX));
}

// 绘制天空到地面的渐变背景。
void DrawGradientBackground() {
	for (int y = 0; y < kHeight; ++y) {
		float t = y / static_cast<float>(kHeight);
		int r;
		int g;
		int b;
		if (y < kHeight * 2 / 3) {
			r = static_cast<int>(20 + 60 * t);
			g = static_cast<int>(120 + 70 * t);
			b = static_cast<int>(210 - 80 * t);
		} else {
			float gnd = (y - kHeight * 2 / 3) / static_cast<float>(kHeight / 3);
			r = static_cast<int>(40 + 50 * gnd);
			g = static_cast<int>(110 + 90 * gnd);
			b = static_cast<int>(30 + 30 * gnd);
		}
		setlinecolor(RGB(r, g, b));
		line(0, y, kWidth, y);
	}
}

// 使用多个椭圆拼接出云朵。
void DrawCloud(int x, int y, float scale) {
	setfillcolor(RGB(255, 255, 255));
	setlinecolor(RGB(235, 245, 255));
	solidellipse(static_cast<int>(x - 70 * scale), static_cast<int>(y - 25 * scale),
				 static_cast<int>(x + 15 * scale), static_cast<int>(y + 25 * scale));
	solidellipse(static_cast<int>(x - 25 * scale), static_cast<int>(y - 35 * scale),
				 static_cast<int>(x + 65 * scale), static_cast<int>(y + 25 * scale));
	solidellipse(static_cast<int>(x + 25 * scale), static_cast<int>(y - 25 * scale),
				 static_cast<int>(x + 110 * scale), static_cast<int>(y + 25 * scale));
}

// 绘制三角形山体。
void DrawMountain(int centerX, int baseY, int width, int height, COLORREF color) {
	setfillcolor(color);
	setlinecolor(color);
	POINT points[3] = {
		{centerX - width / 2, baseY},
		{centerX, baseY - height},
		{centerX + width / 2, baseY}
	};
	solidpolygon(points, 3);
}

// 绘制一条两节式马腿（大腿+小腿）。
void DrawLeg(float hipX, float hipY, float len1, float len2, float angle, float bend) {
	float kneeX = hipX + len1 * std::cos(angle);
	float kneeY = hipY + len1 * std::sin(angle);

	float footAngle = angle + bend;
	float footX = kneeX + len2 * std::cos(footAngle);
	float footY = kneeY + len2 * std::sin(footAngle);

	line(static_cast<int>(hipX), static_cast<int>(hipY), static_cast<int>(kneeX), static_cast<int>(kneeY));
	line(static_cast<int>(kneeX), static_cast<int>(kneeY), static_cast<int>(footX), static_cast<int>(footY));
}

// 绘制一匹完整马：身体、头颈、鬃毛、四肢与尘土。
void DrawHorse(const Horse& h, float timeSec) {
	// 通过正弦波让马匹上下起伏，营造奔跑动感。
	float bob = std::sin(timeSec * 7.0f + h.phase) * 4.0f * h.scale;
	float gallop = std::sin(timeSec * 10.0f + h.phase);

	float bodyW = 90.0f * h.scale;
	float bodyH = 42.0f * h.scale;
	float x = h.x;
	float y = h.y + bob;

	setfillcolor(h.bodyColor);
	setlinecolor(RGB(40, 20, 10));
	solidellipse(static_cast<int>(x - bodyW / 2), static_cast<int>(y - bodyH / 2),
				 static_cast<int>(x + bodyW / 2), static_cast<int>(y + bodyH / 2));

	float neckX = x + bodyW * 0.36f;
	float neckY = y - bodyH * 0.25f;
	float headX = x + bodyW * 0.58f;
	float headY = y - bodyH * 0.42f + gallop * 2.0f * h.scale;

	setlinestyle(PS_SOLID, std::max(2, static_cast<int>(6 * h.scale)));
	line(static_cast<int>(neckX), static_cast<int>(neckY), static_cast<int>(headX), static_cast<int>(headY));

	setfillcolor(RGB(90, 60, 35));
	solidcircle(static_cast<int>(headX), static_cast<int>(headY), std::max(4, static_cast<int>(12 * h.scale)));

	setlinecolor(RGB(35, 20, 10));
	setlinestyle(PS_SOLID, std::max(1, static_cast<int>(2 * h.scale)));
	line(static_cast<int>(headX + 5 * h.scale), static_cast<int>(headY - 10 * h.scale),
		 static_cast<int>(headX + 16 * h.scale), static_cast<int>(headY - 20 * h.scale));
	line(static_cast<int>(headX - 2 * h.scale), static_cast<int>(headY - 10 * h.scale),
		 static_cast<int>(headX - 12 * h.scale), static_cast<int>(headY - 20 * h.scale));

	// 头顶后向线条作为鬃毛，并加入摆动。
	for (int i = 0; i < 5; ++i) {
		float t = i / 4.0f;
		float sway = std::sin(timeSec * 12.0f + h.phase + i * 0.5f) * 5.0f * h.scale;
		line(static_cast<int>(x - bodyW * (0.1f + t * 0.25f)), static_cast<int>(y - bodyH * 0.45f),
			 static_cast<int>(x - bodyW * (0.18f + t * 0.35f)), static_cast<int>(y - bodyH * 0.95f + sway));
	}

	setlinestyle(PS_SOLID, std::max(1, static_cast<int>(3 * h.scale)));
	float legLen1 = 22.0f * h.scale;
	float legLen2 = 24.0f * h.scale;

	float frontPhase = timeSec * 10.0f + h.phase;
	float backPhase = frontPhase + kPi;
	float frontAngle = 1.45f + 0.40f * std::sin(frontPhase);
	float backAngle = 1.45f + 0.40f * std::sin(backPhase);
	float frontBend = 0.45f + 0.25f * std::sin(frontPhase + 1.3f);
	float backBend = 0.45f + 0.25f * std::sin(backPhase + 1.3f);

	// 四条腿使用相位差形成交替奔跑节奏。
	DrawLeg(x + bodyW * 0.23f, y + bodyH * 0.36f, legLen1, legLen2, frontAngle, frontBend);
	DrawLeg(x + bodyW * 0.08f, y + bodyH * 0.38f, legLen1, legLen2, backAngle, backBend);
	DrawLeg(x - bodyW * 0.22f, y + bodyH * 0.36f, legLen1, legLen2, backAngle, backBend);
	DrawLeg(x - bodyW * 0.36f, y + bodyH * 0.34f, legLen1, legLen2, frontAngle, frontBend);

	// 马蹄附近尘土粒子，增强速度感。
	setfillcolor(RGB(165, 145, 110));
	for (int i = 0; i < 3; ++i) {
		float dx = x - bodyW * 0.65f - i * 8.0f * h.scale - std::fmod(timeSec * 60.0f, 45.0f);
		float dy = y + bodyH * 0.55f + i * 2.5f;
		solidcircle(static_cast<int>(dx), static_cast<int>(dy), std::max(1, static_cast<int>(3 * h.scale)));
	}
}

// 主程序：初始化窗口、更新动画状态、分层绘制并响应退出按键。
int main() {
	// 初始化随机种子，用于生成不同的马群分布。
	srand(static_cast<unsigned>(time(nullptr)));

	// 初始化图形窗口并启用双缓冲减少闪烁。
	initgraph(kWidth, kHeight);
	BeginBatchDraw();

	// 创建马群。
	std::vector<Horse> horses;
	const int horseCount = 12;
	horses.reserve(horseCount);

	for (int i = 0; i < horseCount; ++i) {
		Horse h{};
		h.scale = RandFloat(0.65f, 1.25f);
		h.x = RandFloat(-1200.0f, -40.0f);
		h.y = RandFloat(420.0f, 640.0f);
		h.speed = RandFloat(140.0f, 300.0f);
		h.phase = RandFloat(0.0f, kPi * 2.0f);

		int tone = RandInt(0, 2);
		if (tone == 0) {
			h.bodyColor = RGB(120, 75, 40);
		} else if (tone == 1) {
			h.bodyColor = RGB(90, 55, 30);
		} else {
			h.bodyColor = RGB(145, 100, 58);
		}
		horses.push_back(h);
	}

	// 使用系统时钟计算每帧间隔，保证不同机器上速度更稳定。
	DWORD lastTick = GetTickCount();
	DWORD startTick = lastTick;

	while (true) {
		DWORD now = GetTickCount();
		float dt = (now - lastTick) / 1000.0f;
		float timeSec = (now - startTick) / 1000.0f;
		lastTick = now;

		// 处理键盘退出。
		if (_kbhit()) {
			int ch = _getch();
			if (ch == 27 || ch == 'q' || ch == 'Q') {
				break;
			}
		}

		// 更新每匹马的位置，越界后回收并随机重生到左侧。
		for (auto& h : horses) {
			h.x += h.speed * dt;
			if (h.x - 140.0f * h.scale > kWidth) {
				h.scale = RandFloat(0.65f, 1.25f);
				h.x = RandFloat(-680.0f, -120.0f);
				h.y = RandFloat(420.0f, 640.0f);
				h.speed = RandFloat(140.0f, 300.0f);
				h.phase = RandFloat(0.0f, kPi * 2.0f);
			}
		}

		// 按y排序，保证近景马覆盖远景马。
		std::sort(horses.begin(), horses.end(), [](const Horse& a, const Horse& b) {
			return a.y < b.y;
		});

		// 分层绘制：背景 -> 山云 -> 草地 -> 马群 -> 文本。
		DrawGradientBackground();

		DrawMountain(280, 470, 480, 220, RGB(78, 95, 95));
		DrawMountain(620, 500, 620, 260, RGB(68, 86, 86));
		DrawMountain(1000, 500, 540, 240, RGB(74, 92, 92));

		float cloudOffset = std::fmod(timeSec * 22.0f, static_cast<float>(kWidth + 320));
		DrawCloud(static_cast<int>(100 + cloudOffset), 110, 1.1f);
		DrawCloud(static_cast<int>(540 + cloudOffset * 0.8f), 160, 0.9f);
		DrawCloud(static_cast<int>(920 + cloudOffset * 0.6f), 120, 1.0f);

		setfillcolor(RGB(56, 140, 56));
		setlinecolor(RGB(56, 140, 56));
		solidrectangle(0, 470, kWidth, kHeight);

		for (const auto& h : horses) {
			DrawHorse(h, timeSec);
		}

		// 叠加标题与提示文本。
		setbkmode(TRANSPARENT);
		settextcolor(RGB(255, 245, 220));
		settextstyle(72, 0, _T("Microsoft YaHei UI"));
		outtextxy(380, 38, "\xCD\xF2\xC2\xED\xB1\xBC\xCC\xDA");

		settextstyle(22, 0, _T("Microsoft YaHei UI"));
		settextcolor(RGB(230, 235, 220));
		outtextxy(24, 680, "\xB0\xB4 Esc \xBB\xF2 Q \xCD\xCB\xB3\xF6");

		// 刷新并控制帧率约60FPS。
		FlushBatchDraw();
		Sleep(16);
	}

	// 释放图形资源。
	EndBatchDraw();
	closegraph();
	return 0;
}
