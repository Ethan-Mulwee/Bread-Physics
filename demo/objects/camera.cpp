#include "camera.hpp"

#include "ext/iostream.hpp"

void Camera::init(const bMath::float3 &position, float fov, float aspect, float near, float far) {
    m_Position = position;
    m_FOV = fov;
    m_Aspect = aspect;
    m_Near = near;
    m_Far = far;

    setAspect(m_Aspect);    
}

void Camera::updateShader(Shader *shader) {
    bMath::matrix4 modelMatrix = bMath::matrix4::identity();
    shader->setMatrix4(modelMatrix, "model");
    shader->setMatrix4(m_ViewMatrix, "view");
    shader->setMatrix4(getProjection(), "projection");
    shader->setFloat3(m_Position, "camPos");

    std::cout << getProjection() << "\n";
    std::cout << m_ViewMatrix << "\n";
}

void Camera::setAspect(float aspect) {
    m_Projection = bMath::transpose(bMath::perspectiveMatrix(m_FOV, m_Aspect, m_Near, m_Far));
}

void Camera::setDistance(float offset) {
    m_Distance += offset;
    updateViewMatrix();
}

const bMath::matrix4 &Camera::getProjection() const
{
    return m_Projection;
}

bMath::float3 Camera::getUp() const
{
    return UP * getDirection();
}

bMath::float3 Camera::getRight() const
{
    return RIGHT * getDirection();
}

bMath::float3 Camera::getForward() const
{
    return FORWARD * getDirection();
}

bMath::float3 Camera::getPosition() const {
    return m_Position;
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

    bMath::quaternion orientation = getDirection();
    bMath::float3x3 rotmat = bMath::quaternionToMatrix(orientation);

    bMath::float4x4 rotmat4(
        rotmat(0,0), rotmat(0,1), rotmat(0,2), 0, 
        rotmat(1,0), rotmat(1,1), rotmat(1,2), 0, 
        rotmat(2,0), rotmat(2,1), rotmat(2,2), 0, 
                  0,           0,           0, 1
    );
    m_ViewMatrix = bMath::translationMatrix(m_Position) * rotmat4;
    std::cout << "before inverse: \n" << m_ViewMatrix << "\n";
    std::cout << "check: " << m_ViewMatrix * bMath::inverse(m_ViewMatrix) << "\n";
    m_ViewMatrix = bMath::inverse(m_ViewMatrix);
}

void Camera::onMouseWheel(double delta) {
    setDistance(delta * 0.5f);

    updateViewMatrix();
}

void Camera::onMouseMove(double x, double y) {
    bMath::float2 pos2d(x,y);

    bMath::float2 delta = (pos2d - m_CurrentPos2d)*0.004f;

    float sign = getUp().y < 0 ? -1.0f : 1.0f;
    m_Yaw += sign * delta.x * ROTATIONSPEED;
    m_Pitch += delta.y * ROTATIONSPEED;

    updateViewMatrix();

    // m_Focus += -1.0f * getRight()*delta.x*m_Distance;
    // m_Focus += getUp() * delta.y * m_Distance;

    // updateViewMatrix();

    m_CurrentPos2d = pos2d;
}
