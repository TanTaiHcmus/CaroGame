#pragma once
#include "Libs.h"
#include "Paint.h"

int main() {
	//Khởi tạo chế độ ngẫu nhiên
	srand(time(NULL));
	
	//Làm ẩn con trỏ
	setcursor(false);

	//Vẽ lựa chọn, bắt đầu là 1
	Luachon(1);
}