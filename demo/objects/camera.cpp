#include "camera.hpp"

void Camera::init(const bMath::float3 &position, float fov, float aspect, float near, float far) {
    m_Position = position;
    m_FOV = fov;
    m_Aspect = aspect;
    m_Near = near;
    m_Far;
}

void Camera::updateShader(Shader *shader) {
    bMath::matrix4 modelMatrix = bMath::matrix4::identity();
    shader->set_mat4(modelMatrix, "model");
    shader->set_mat4(m_ViewMatrix, "view");
    shader->set_mat4(get_projection(), "projection");
    shader->set_float3(m_Position, "camPos");
}

const bMath::matrix4 &Camera::get_projection() const
{
    return m_Projection;
}

bMath::float3 Camera::get_forward() const
{
    return FORWARD * get_direction();
}

bMath::quaternion Camera::get_direction() const
{
    return bMath::quaternion(-m_Pitch, -m_Yaw, 0.0f);
}

bMath::matrix4 Camera::get_view_matrix() const
{
    return m_ViewMatrix;
}

void Camera::update_view_matrix() {
    m_Position = m_Focus - get_forward() * m_Distance;

}
