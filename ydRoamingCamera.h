#pragma once
#include "ydCamera.h"
#include "YNTime.h"
#include "ydInput.h"

namespace dx11
{
    using namespace DirectX::SimpleMath;
    class RoamingCamera : public Camera
    {
    public:
        RoamingCamera()
        {
            SetWorldPostion({ 0,1,-1.0000000f });
        }
    private:
        static constexpr float CamMoveSpeed = 1.11f;
        static constexpr float RotationSpeed = 10.11f;
        int prevMouseX{ 0 }, prevMouseY{ 0 };
        // 카메라 위치와 회전 변경
        virtual void Update()override
        {
            //SetWorldRotation(DirectX::SimpleMath::Quaternion{ 0.38f,0,0,0.92f });
            Vector3 moveDir = Vector3::Zero;
            if (Input::IsKeyDown<'W'>())
                moveDir += Foward();
            if (Input::IsKeyDown<'S'>())
                moveDir -= Foward();
            if (Input::IsKeyDown<'A'>())
                moveDir -= Right();
            if (Input::IsKeyDown<'D'>())
                moveDir += Right();
            if (Input::IsKeyDown<'Q'>())
                moveDir -= Vector3::Up;
            if (Input::IsKeyDown<'E'>())
                moveDir += Vector3::Up;
            moveDir.Normalize();

            SetWorldPostion(Transform::GetWorldPosition() + moveDir * CamMoveSpeed * YNTime::Instance().GetDeltaTime());

            int mouseX, mouseY;
            Input::GetMousePosition(mouseX, mouseY);
            int deltaX = mouseX - prevMouseX;
            int deltaY = mouseY - prevMouseY;

            prevMouseX = mouseX;
            prevMouseY = mouseY;

            if (Input::IsKeyDown<VK_RBUTTON>())
            {
                // Compute rotation angles based on mouse movement
                float yaw = deltaX * RotationSpeed * YNTime::Instance().GetDeltaTime();
                float pitch = deltaY * RotationSpeed * YNTime::Instance().GetDeltaTime();

                // Create rotation quaternions for yaw and pitch
                Quaternion yawRotation{ Quaternion::CreateFromAxisAngle(Vector3::Up, yaw) };
                //auto pitchAxis = Right() - Right().Dot(Vector3::Up) * Vector3::Up;
                //pitchAxis.Normalize();
                //Quaternion pitchRotation{ Quaternion::CreateFromAxisAngle(pitchAxis, pitch) };
                Quaternion pitchRotation{ Quaternion::CreateFromAxisAngle(Right(), pitch) };

                // Combine yaw and pitch rotations
                //Quaternion combinedRotation = pitchRotation * yawRotation;
                //SetWorldRotation(pitchRotation * yawRotation * GetWorldRotation());
                //SetWorldRotation(yawRotation * pitchRotation * GetWorldRotation());
                SetWorldRotation(GetWorldRotation() * yawRotation * pitchRotation);
            }
        }
    };
}
