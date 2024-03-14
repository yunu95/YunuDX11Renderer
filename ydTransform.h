#pragma once
#include "ydDX11Dependency.h"

namespace dx11
{
    using namespace DirectX::SimpleMath;
    // 카메라는 버텍스 좌표 사상에 필요한 변수만 관리할 뿐, 렌더 타겟의 크기를 바꾸는 등 dx 객체에 대한 호출을 하지는 않는다.
    class Transform
    {
    public:
        void SetWorldPostion(const DirectX::SimpleMath::Vector3& position)
        {
            if (!srtCached)
                CacheSRT();
            cachedPosition = position;
            SetWTMFromCachedSRT();
        }
        void SetWorldScale(const DirectX::SimpleMath::Vector3& scale)
        {
            if (!srtCached)
                CacheSRT();
            cachedScale = scale;
            SetWTMFromCachedSRT();
        }
        void SetWorldRotation(const DirectX::SimpleMath::Quaternion rotation)
        {
            if (!srtCached)
                CacheSRT();
            cachedRotation = rotation;
            SetWTMFromCachedSRT();
        }
        void SetWorldTM(const DirectX::SimpleMath::Matrix wtm)
        {
            srtCached = false;
            this->wtm = wtm;
            OnTMChange();
        };
        DirectX::SimpleMath::Vector3 GetWorldPosition() const
        {
            if (!srtCached)
                CacheSRT();
            return cachedPosition;
        }
        DirectX::SimpleMath::Vector3 GetWorldScale()const
        {
            if (!srtCached)
                CacheSRT();
            return cachedScale;
        }
        DirectX::SimpleMath::Quaternion GetWorldRotation()const
        {
            if (!srtCached)
                CacheSRT();
            return cachedRotation;
        }
        const DirectX::SimpleMath::Matrix& GetWorldTM()const { return wtm; }
        DirectX::SimpleMath::Vector3 Foward()
        {
            return Vector3::Transform(-DirectX::SimpleMath::Vector3::Forward, GetWorldRotation());
        }
        DirectX::SimpleMath::Vector3 Up()
        {
            return Vector3::Transform(DirectX::SimpleMath::Vector3::Up, GetWorldRotation());
        }
        DirectX::SimpleMath::Vector3 Right()
        {
            return Vector3::Transform(DirectX::SimpleMath::Vector3::Right, GetWorldRotation());
        }
    protected:
        virtual void OnTMChange() {};
    private:
        void CacheSRT() const
        {
            wtm.Decompose(cachedScale, cachedRotation, cachedPosition);
            srtCached = true;
        }
        void SetWTMFromCachedSRT()
        {
            wtm = DirectX::SimpleMath::Matrix::CreateScale(cachedScale) *
                DirectX::SimpleMath::Matrix::CreateFromQuaternion(cachedRotation) *
                DirectX::SimpleMath::Matrix::CreateTranslation(cachedPosition);
            OnTMChange();
        }
        mutable bool srtCached{ true };
        mutable DirectX::SimpleMath::Vector3 cachedPosition{ DirectX::SimpleMath::Vector3::Zero };
        mutable DirectX::SimpleMath::Vector3 cachedScale{ DirectX::SimpleMath::Vector3::One };
        mutable DirectX::SimpleMath::Quaternion cachedRotation{ DirectX::SimpleMath::Quaternion::Identity };
        mutable DirectX::SimpleMath::Matrix wtm;
    };
}