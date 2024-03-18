#include "yd11Camera.h"

namespace dx11
{
    Camera* Camera::mainCam{ nullptr };
    Camera::Camera()
    {
        SetVerticalFOV(verticalFov);
    }
    void Camera::OnTMChange()
    {
        // 세상을 뷰 좌표계로 전환하는 것은 카메라의 월드 트랜스폼의 역행렬을 곱하는 것과 같다.
        this->wtm = GetWorldTM();
        this->vtm = GetWorldTM().Invert();
        // 뷰 좌표계를 NDC공간으로 변환하는 프로젝션 행렬을 만든다. 각 버텍스의 깊이값에 따라 연산을 수행하는 건 따로 적용된다.
        this->ptm = DirectX::XMMatrixPerspectiveFovLH(this->verticalFov, this->width / this->height, this->cameraNear, this->cameraFar);
        this->vtmOrtho = DirectX::XMMatrixOrthographicLH(this->width * 1.f, this->height * 1.f, this->cameraNear, this->cameraFar);

        CreateFrustum();
    }

    void Camera::SetVerticalFOV(float verticalFovRadian)
    {
        this->verticalFov = verticalFovRadian;
        this->horizontalFov = 2 * static_cast<float>(atan(GetAspectRatio() * tan(verticalFovRadian * 0.5)));
        CreateFrustum();
    }
    void Camera::SetHorizontalFOV(float horizontalFovRadian)
    {
        this->horizontalFov = horizontalFovRadian;
        this->verticalFov = 2 * static_cast<float>(atan(1.0 / GetAspectRatio() * tan(horizontalFovRadian * 0.5)));
        CreateFrustum();
    }

    void Camera::SetNear(float cameraNear)
    {
        this->cameraNear = cameraNear;
        CreateFrustum();
    }

    void Camera::SetFar(float cameraFar)
    {
        this->cameraFar = cameraFar;
        CreateFrustum();
    }

    void Camera::SetResolution(float width, float height)
    {
        this->width = width;
        this->height = height;
        CreateFrustum();
    }
    float Camera::GetVerticalFOV()
    {
        return this->verticalFov;
    }
    float Camera::GetHorizontalFOV()
    {
        return this->horizontalFov;
    }
    float Camera::GetNear()
    {
        return this->cameraNear;
    }
    float Camera::GetFar()
    {
        return this->cameraFar;
    }
    void Camera::GetResolution(float* width, float* height)
    {
        *width = this->width;
        *height = this->height;
    }
    float Camera::GetAspectRatio()
    {
        return width / height;
    }

    Camera* Camera::GetMainCam()
    {
        return mainCam;
    }
    void Camera::SetAsMain()
    {
        mainCam = this;
    }

    DirectX::SimpleMath::Matrix Camera::GetPTM90()
    {
        return DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 2.f, this->width / this->height, this->cameraNear, this->cameraFar);
    }

    void Camera::CreateFrustum()
    {
        this->frustum.CreateFromMatrix(this->frustum, ptm);
    }
}