#include "camera.hpp"

void Camera::init(const bMath::float3 &position, float fov, float aspect, float near, float far) {
    m_Position = position;
    m_FOV = fov;
    m_Aspect = aspect;
    m_Near = near;
    m_Far;

    // setAspect(m_Aspect)
}

void Camera::updateShader(Shader *shader) {
    bMath::matrix4 modelMatrix = bMath::matrix4::identity();
    shader->setMatrix4(modelMatrix, "model");
    shader->setMatrix4(m_ViewMatrix, "view");
    shader->setMatrix4(getProjection(), "projection");
    shader->setFloat3(m_Position, "camPos");
}

void Camera::setAspect(float aspect) {
    m_Projection = bMath::perspectiveMatrix(m_FOV, m_Aspect, m_Near, m_Far);
}

const bMath::matrix4 &Camera::getProjection() const
{
    return m_Projection;
}

bMath::float3 Camera::getForward() const
{
    return FORWARD * getDirection();
}

bMath::quaternion Camera::getDirection() const
{
    return bMath::quaternion(-m_Pitch, -m_Yaw, 0.0f);
}

bMath::matrix4 Camera::getViewMatrix() const
{
    return m_ViewMatrix;
}

void Camera::updateViewMatrix() {
    m_Position = m_Focus - getForward() * m_Distance;

}
