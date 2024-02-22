#pragma once
#include "imgui.h"
#include <memory>

// 예제의 실행단위
// Snippet 하나당 객체 초기화를 담당하게 됨.
class Snippet
{
public:
	static std::unique_ptr<Snippet> current;
	// 초기화
	Snippet() {};
	// 시간에 따른 매 프레임 업데이트
	virtual void Update(float dt) {};
	// Imgui 메뉴
	virtual void ImguiUpdate() {};
private:
	virtual ~Snippet() {};
	friend std::default_delete<Snippet>;
};
std::unique_ptr<Snippet> Snippet::current;
