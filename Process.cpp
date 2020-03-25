#pragma once
#include "Process.h"
#include "Paint.h"
#include "Libs.h"

//Khởi tạo biến toàn cục kết quả
int Result = 0;

//Mảng điểm, thay đổi thử, cái này quyết định độ thông minh của máy
int AttackArr[10] = { 0, 1, 9, 81, 729, 6561, 59049, 531441, 4782969, 500000000};
int DefendArr[10] = { 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };

/* Kiểm tra ô này có nằm trong bàn cờ hay ko */
bool ischan(int x, int y) {
	return ((x < 0) || (x > Row - 1) || (y < 0) || (y > Column - 1));
}

/* Kiểm tra kết quả, xem thử với ô [x][y], 
   thì các hướng xung quanh cộng với ô [x][y]
   có bằng hoặc lớn hơn 5 ko, có kiểm tra chặn 2 đầu */
bool CheckResult(int PlayerCom[MaxN][MaxN], int x, int y) {
	int tX[8] = { 1,0,1,1,-1,0,-1,-1 };
	int tY[8] = { 0,1,1,-1,0,-1,-1,1 };
	int Ally[4]{}, Enemy[4]{};

	for (int k = 0; k < 8; k++) {
		for (int i = 1; i < 6; i++) {
			if (
				ischan(
					x + i * tX[k],
					y + i * tY[k]
				) ||
				PlayerCom[x + i * tX[k]][y + i * tY[k]] == 0
			) break;

			if (PlayerCom[x + i * tX[k]][y + i * tY[k]] == PlayerCom[x][y]) {
				Ally[k % 4]++;
			}

			if (PlayerCom[x + i * tX[k]][y + i * tY[k]] == 3 - PlayerCom[x][y]) {
				Enemy[k % 4]++;
				break;
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		if (Ally[i] >= 4 && Enemy[i] < 2) return true;
	}

	return false;
}

/* Phần này là làm cho máy đánh */
#pragma region Computer

/* Tính điểm tấn công của ô [x][y]
   Nếu đánh ô [x][y] thì điểm tấn công sẽ là bao nhiêu */
int AttackPoint(int PlayerCom[MaxN][MaxN], int x, int y) {
	int tX[8] = { 1,0,1,1,-1,0,-1,-1 };
	int tY[8] = { 0,1,1,-1,0,-1,-1,1 };
	int Ally[4]{}, Enemy[4]{}, Block[4]{};

	for (int k = 0; k < 8; k++) {
		for (int i = 1; i < 6; i++) {

			if (ischan(x + i * tX[k], y + i * tY[k])) {
				Block[k % 4]++;
				break;
			}

			if (PlayerCom[x + i * tX[k]][y + i * tY[k]] == 0) break;

			if (PlayerCom[x + i * tX[k]][y + i * tY[k]] == 2) {
				Ally[k % 4]++;
			}

			if (PlayerCom[x + i * tX[k]][y + i * tY[k]] == 1) {
				Enemy[k % 4]++;
				break;
			}
		}
	}

	int SumPoint = 0;
	for (int i = 0; i < 4; i++) {
		int Point = AttackArr[Ally[i]];

		//Mình có 4 con, ưu tiên đánh
		if (Ally[i] == 4) Point = AttackArr[9];

		//Bị chặn thì giảm điểm
		if (Enemy[i] == 1 || Block[i] == 1) Point /= 2;

		//Bị chặn hai đầu thì không đánh
		if (
			Enemy[i] == 1 &&
			Ally[i] < 4 &&
			Block[i] == 1
			) Point = 0;
		if (Enemy[i] == 2) Point = 0;
		SumPoint += Point;
	}

	return SumPoint;
}

/* Tính điểm phòng ngự của ô [x][y]
   Tính xem đánh ô [x][y] thì có chặn đc địch không, 
   khả năng chặn có lớn không */
int DefendPoint(int PlayerCom[MaxN][MaxN], int x, int y) {
	int tX[8] = { 1,0,1,1,-1,0,-1,-1 };
	int tY[8] = { 0,1,1,-1,0,-1,-1,1 };
	int Ally[4]{}, Enemy[4]{}, Block[4]{};

	for (int k = 0; k < 8; k++) {
		for (int i = 1; i < 6; i++) {
			if (ischan(x + i * tX[k], y + i * tY[k])) {
				Block[k % 4]++;
				break;
			}

			if (PlayerCom[x + i * tX[k]][y + i * tY[k]] == 0) break;

			if (PlayerCom[x + i * tX[k]][y + i * tY[k]] == 1) {
				Enemy[k % 4]++;
			}

			if (PlayerCom[x + i * tX[k]][y + i * tY[k]] == 2) {
				Ally[k % 4]++;
				break;
			}
		}
	}

	int SumPoint = 0;
	for (int i = 0; i < 4; i++) {
		int Point = DefendArr[Enemy[i]];

		//Địch có 4 con, ưu tiên chặn
		if (Enemy[i] == 4) Point = DefendArr[9]; 

		//Bị chặn thì giảm điểm
		if ((Ally[i] == 1) || (Block[i] == 1)) Point /= 2; 

		//Bị chặn hai đầu thì điểm bằng 0
		if (
			Ally[i] == 1 &&
			Enemy[i] < 4 &&
			Block[i] == 1
			) Point = 0; 
		if (Ally[i] == 2) Point = 0;
		SumPoint += Point;
	}

	return SumPoint;
}

/* Phần máy đánh */
bool ComputerGo(int PlayerCom[MaxN][MaxN]) {
	int x, y;

	//Hết ô, hòa cờ
	if (!FindNullPos(PlayerCom)) {
		Result = 3;
		return true;
	}

	//Tính điểm tổng = điểm công + điểm thủ
	int MaxPoint = -DefendArr[9];
	for (int i = 0; i < Row; i++) {
		for (int j = 0; j < Column; j++) {
			if (PlayerCom[i][j] == 0)
			{
				//Chọn r ngẫu nhiên, tăng tính ngẫu nhiên
				int r = rand() % 2; 
				int Point = AttackPoint(PlayerCom, i, j) + DefendPoint(PlayerCom, i, j);

				//Lấy tổng điểm lớn nhất (MaxPoint), đánh vào đó
				if (
					MaxPoint < Point ||
					(
						MaxPoint == Point &&
						r == 0
					)
				) {
					MaxPoint = Point;
					x = i;
					y = j;
				}
			}
		}
	}

	//Đến ô cờ [x][y], chỉnh màu, đánh vô đó
	Gotoxy(x * 2 + 1, y * 4 + 2);
	SetColor(12);
	PlayerCom[x][y] = 2;
	cout << 'O';

	//Kiểm tra kết quả, thắng thì out luôn, báo máy thắng
	if (CheckResult(PlayerCom, x, y)) {
		Result = 2;
		return true;
	}
	 
	return false;
}

/* Phần người đánh */
bool PlayerGo(int PlayerCom[MaxN][MaxN], int &x, int &y) {
	//Hết bàn thì hòa cờ
	if (!FindNullPos(PlayerCom)) {
		Result = 3;
		return true;
	}

	/*Phần đánh cờ, Move dùng để cho người chơi di chuyển con trỏ
	  và đánh vào ô [x][y] được người chơi chọn */
	if (Move(PlayerCom, x, y, 10)) {
		Gotoxy(x * 2 + 1, y * 4 + 2);
		PlayerCom[x][y] = 1;
		cout << 'X';
	};

	//Kiểm tra kết quả, thắng thì out
	if (CheckResult(PlayerCom, x, y)) {
		Result = 1;
		return true;
	}

	return false;
}

void PlayerWithCom() {
	//Khởi tạo mảng đánh với máy, vẽ bàn cờ
	int PlayerCom[MaxN][MaxN] = { {} };
	Vebanco(Row, Column);
	SetColor(11);
	DrawCaRo(8, 95);

	SetColor(15);
	Gotoxy(14, 94);
	cout << "Mui ten de di chuyen,";
	Gotoxy(15, 94);
	cout << "Enter de danh co.";
	/* ---------------------------------- */
	//Khởi tạo kết quả chưa xác định
	Result = 0;
	int x = 6, y = 10;

	//Khi chưa ngả ngũ, thì đánh tiếp
	do {
		SetColor(10); //Đổi màu
		if (PlayerGo(PlayerCom, x, y)) break;
		SetColor(12); //Đổi màu
		if (ComputerGo(PlayerCom)) break;
	} while (Result == 0);

	XuatKQ(Result);
}
#pragma endregion

//Phần người đánh với nhau
#pragma region Player

//Người thứ nhất đánh
bool PlayerGoFirst(int PlayerPlayer[MaxN][MaxN], int &x, int &y) {
	//Hết bàn thì hòa
	if (!FindNullPos(PlayerPlayer)) {
		Result = 3;
		return true;
	}

	//Phần đánh cờ của người chơi
	if (Move(PlayerPlayer, x, y, 10)) {
		Gotoxy(x * 2 + 1, y * 4 + 2);
		PlayerPlayer[x][y] = 1;
		cout << 'X';
	};
	
	//Kiểm tra kết quả
	if (CheckResult(PlayerPlayer, x, y)) {
		Result = 1;
		return true;
	}

	return false;
}

//Người thứ hai đánh
bool PlayerGoSecond(int PlayerPlayer[MaxN][MaxN], int &x, int &y) {
	//Hết bàn, hòa cờ
	if (!FindNullPos(PlayerPlayer)) {
		Result = 3;
		return true;
	}

	//Phần người chơi đánh cờ
	if (Move(PlayerPlayer, x, y, 12)) {
		Gotoxy(x * 2 + 1, y * 4 + 2);
		PlayerPlayer[x][y] = 2;
		cout << 'O';
	};

	//Kiểm tra kết quả
	if (CheckResult(PlayerPlayer, x, y)) {
		Result = 2;
		return true;
	}

	return false;
}

void PlayerWithPlayer() {
	//Khởi tạo mảng người chơi, vẽ bàn cờ
	int PlayerPlayer[MaxN][MaxN] = { {} };
	Vebanco(Row, Column);
	SetColor(11);
	DrawCaRo(8, 95);

	SetColor(15);
	Gotoxy(14, 94);
	cout << "Mui ten de di chuyen,";
	Gotoxy(15, 94);
	cout << "Enter de danh co.";
	/* ---------------------------------- */

	//Khởi tạo kết quả
	Result = 0;
	int x = 6, y = 10;

	//Khi kết quả chưa có gì thì đánh tiếp
	do {
		SetColor(10);
		if (PlayerGoFirst(PlayerPlayer, x, y)) break;
		SetColor(12);
		if (PlayerGoSecond(PlayerPlayer, x, y)) break;
	} while (Result == 0);

	//Xuất kết quả
	XuatKQ(Result);
}

#pragma endregion





