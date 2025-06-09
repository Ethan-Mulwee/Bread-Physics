#ifndef BENGINE_CAMERA
#define BENGINE_CAMERA

#include "shader/b_shader.hpp"

#include "bMath.hpp"

class Camera {
    public:
        void init(const bMath::float3 &position, float fov, float aspect, float near, float far);

        void updateShader(Shader* shader);



        void set_aspect(float aspect);

        void set_distance(float offset);

        const bMath::matrix4& get_projection() const;

        const bMath::matrix4& get_view_projection() const;

        bMath::float3 get_up() const;
        
        bMath::float3 get_right() const;

        bMath::float3 get_forward() const;

        bMath::quaternion get_direction() const;

        bMath::matrix4 get_view_matrix() const;

        void update_view_matrix();


    private:
        bMath::matrix4 m_ViewMatrix = bMath::matrix4::identity();
        bMath::matrix4 m_Projection = bMath::matrix4::identity();

        bMath::float3 m_Position;
        bMath::float3 m_Focus;

        float m_Distance = 5.0f;
        float m_Aspect, m_FOV, m_Near, m_Far;

        float m_Pitch = 0.0f;
        float m_Yaw = 0.0f;

        bMath::float2 m_CurrentPos2d;

        const bMath::float3 RIGHT = {1.0f,0.0f,0.0f};
        const bMath::float3 UP = {0.0f,1.0f,0.0f};
        const bMath::float3 FORWARD = {0.0f,0.0f,-1.0f};

        const float ROTATIONSPEED = 2.0f;
};

#endif