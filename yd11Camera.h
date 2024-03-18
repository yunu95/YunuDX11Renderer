#pragma once
#include "yd11DXDependency.h"
#include "yd11Transform.h"
#include "ydUpdatable.h"

namespace dx11
{
    // 카메라는 버텍스 좌표 사상에 필요한 변수만 관리할 뿐, 렌더 타겟의 크기를 바꾸는 등 dx 객체에 대한 호출을 하지는 않는다.
    // 카메라는 z축을 바라본다.
    // 유니티처럼 z가 앞, y가 위, x가 오른쪽인 왼손 좌표계임.
    class Camera : public Transform, public Updatable
    {
    public:
        Camera();
        virtual void OnTMChange()override;
        void SetVerticalFOV(float verticalFovRadian);
        void SetHorizontalFOV(float horizontalFovRadian);
        void SetNear(float cameraNear);
        void SetFar(float cameraFar);
        void SetResolution(float width, float height);
        float GetVerticalFOV();
        float GetHorizontalFOV();
        float GetNear();
        float GetFar();
        void GetResolution(float* width, float* height);
        // 종횡비 = width/height
        float GetAspectRatio();
        static Camera* GetMainCam();
        void SetAsMain();

        const DirectX::SimpleMath::Matrix& GetWTM() { return this->wtm; }
        const DirectX::SimpleMath::Matrix& GetVTM() { return this->vtm; }
        const DirectX::SimpleMath::Matrix& GetVTMOrtho() { return this->vtmOrtho; }
        const DirectX::SimpleMath::Matrix& GetPTM() { return this->ptm; }

        DirectX::SimpleMath::Matrix GetPTM90();

        DirectX::BoundingFrustum& GetFrustum() { return this->frustum; };

    private:
        void CreateFrustum();

    private:
        static Camera* mainCam;
        DirectX::SimpleMath::Matrix wtm;
        DirectX::SimpleMath::Matrix vtm;
        DirectX::SimpleMath::Matrix vtmOrtho;
        DirectX::SimpleMath::Matrix ptm;
        // 종횡비(aspect ratio) = w/h
        // 수직 시야각/2 = atan(y높이 / 2 * z거리), 카메라가 위 아래로 벌어진 각도로 0~180도 범위에 있다.
        // 수평 시야각/2 = atan(x너비 / 2 * z거리), 카메라가 위 아래로 벌어진 각도로 0~180도 범위에 있다.
        // 수직 시야각과 수평 시야각은 서로에게 종속적이다.
        float verticalFov = DirectX::XM_PI / 4.f;
        float horizontalFov{ 0 };
        float cameraNear = 0.1f;
        float cameraFar = 1000.f;
        // 카메라 가로 해상도
        float width = 800;
        // 카메라 세로 해상도
        float height = 800;

        DirectX::BoundingFrustum frustum;
    };
}
