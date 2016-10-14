#include "Gizmos.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Pipeline.hpp"

using namespace FishEngine;

Color Gizmos::s_color{0, 1, 0, 1};
std::shared_ptr<SimpleMesh> Gizmos::s_lineMesh = nullptr;
std::shared_ptr<SimpleMesh> Gizmos::s_circleMesh = nullptr;
std::shared_ptr<SimpleMesh> Gizmos::s_boxMesh = nullptr;
//std::shared_ptr<SimpleMesh> Gizmos::s_wiredSphereMesh = nullptr;

constexpr int circleVertexCount = 64;

void Gizmos::Init()
{
    // line
    float line_vertex[] = {
        0, 0, 0,
        1, 0, 0,
    };
    s_lineMesh = std::make_shared<SimpleMesh>(line_vertex, 2, GL_LINES);
    
    // circle
    constexpr float radStep = 2.0f * Mathf::PI / circleVertexCount;
    float circle_vertex[circleVertexCount*3];
    for (int i = 0; i < circleVertexCount; ++i) {
        const int j = i * 3;
        const float rad = radStep * i - Mathf::PI/2;
        circle_vertex[j] = std::cosf(rad);
        circle_vertex[j+1] = 0.f;
        circle_vertex[j+2] = std::sinf(rad);
    }
    
    s_circleMesh = std::make_shared<SimpleMesh>(circle_vertex, circleVertexCount, GL_LINE_LOOP);

    
    float box_vertex[] = {
         0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
        
        -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
        
        -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,
        
         0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
    };
    s_boxMesh = std::make_shared<SimpleMesh>(box_vertex, 24, GL_LINES);
    
    // wired sphere
    //constexpr float radStep = 2.0f * Mathf::PI / circleVertexCount;
//    float wiredSphereVertex[circleVertexCount*3*3];
//    for (int i = 0; i < circleVertexCount; ++i) {
//        const int j1 = i * 3;
//        const int j2 = j1 + circleVertexCount*3;
//        const int j3 = j2 + circleVertexCount*3;
//        const float rad = radStep * i;
//        wiredSphereVertex[j3+1] = wiredSphereVertex[j2] =  wiredSphereVertex[j1] = std::cosf(rad);
//        wiredSphereVertex[j3] = wiredSphereVertex[j2+2] = wiredSphereVertex[j1+1] = 0.f;
//        wiredSphereVertex[j3+2] = wiredSphereVertex[j2+1] = wiredSphereVertex[j1+2] = std::sinf(rad);
//    }
//    
//    s_wiredSphereMesh = std::make_shared<SimpleMesh>(wiredSphereVertex, circleVertexCount*3, GL_LINE_STRIP);
}

void Gizmos::DrawLine(const Vector3& from, const Vector3& to)
{
    //glLineWidth(50);  // TODO, no effect
    const auto& shader = Shader::builtinShader("SolidColor");
    shader->Use();
    Matrix4x4 m;
    Vector3 dir = to-from;
    float len = dir.magnitude();
    m.SetTRS(from, Quaternion::FromToRotation(Vector3(1, 0, 0), dir.normalized()), Vector3(len, len, len));
    ShaderUniforms uniforms;
    auto v = Camera::main()->worldToCameraMatrix();
    auto p = Camera::main()->projectionMatrix();
    uniforms.vec4s["Color"] = s_color;
    //uniforms.mat4s["MATRIX_MVP"] = p * v * m;
    Pipeline::perDrawUniformData.MATRIX_MVP = p * v * m;
    Pipeline::BindPerDrawUniforms();
    shader->BindUniforms(uniforms);
    s_lineMesh->Render();
}

void Gizmos::DrawIcon(Vector3 center,
                      const std::string& name,
                      bool allowScaling)
{
    
}

void Gizmos::DrawWireSphere(const Vector3& center,
                            float radius,
                            const Matrix4x4& modelMatrix)
{
    const auto& shader = Shader::builtinShader("SolidColor");
    shader->Use();
    Matrix4x4 m;
    //m.SetTRS(center, Quaternion::identity, Vector3::one * radius);
    ShaderUniforms uniforms;
    auto v = Camera::main()->worldToCameraMatrix();
    auto p = Camera::main()->projectionMatrix();
    uniforms.vec4s["Color"] = s_color;
    shader->BindUniforms(uniforms);
    
    float euler_angles[] = {
        0, 0, 90,
        0, 0, 0,
        90, 0, 0,
    };
    
    for (int i = 0; i < 3; ++i) {
        float* e = euler_angles + i*3;
        m.SetTRS(center, Quaternion::Euler(Vector3(e)), Vector3::one * radius);
        Pipeline::perDrawUniformData.MATRIX_MVP = p * v * modelMatrix * m;
        Pipeline::BindPerDrawUniforms();
        s_circleMesh->Render();
    }
}

void Gizmos::
DrawHalfWireSphere(const Vector3& center,
                   const float radius,
                   const Matrix4x4& modelMatrix)
{
    const auto& shader = Shader::builtinShader("SolidColor");
    shader->Use();
    Matrix4x4 m;
    //m.SetTRS(center, Quaternion::identity, Vector3::one * radius);
    ShaderUniforms uniforms;
    auto v = Camera::main()->worldToCameraMatrix();
    auto p = Camera::main()->projectionMatrix();
    uniforms.vec4s["Color"] = s_color;
    shader->BindUniforms(uniforms);
    
    float euler_angles[] = {
        0, 0, 90,
        0, 0, 0,
        0, 90, 90,
    };
    
    for (int i = 0; i < 3; ++i) {
        float* e = euler_angles + i*3;
        m.SetTRS(center, Quaternion::Euler(Vector3(e)), Vector3::one * radius);
        Pipeline::perDrawUniformData.MATRIX_MVP = p * v * m * modelMatrix;
        Pipeline::BindPerDrawUniforms();
        glBindVertexArray(s_circleMesh->m_VAO);
        int count = s_circleMesh->m_positionBuffer.size()/3;
        if (i == 1) {
            glDrawArrays(GL_LINE_LOOP, 0, count);
        } else {
            count = count/2+1;
            glDrawArrays(GL_LINE_STRIP, 0, count);
        }
        
        glBindVertexArray(0);
    }

}

void Gizmos::
DrawWireCube(const Vector3& center,
             const Vector3& size,
             const Matrix4x4& modelMatrix)
{
    const auto& shader = Shader::builtinShader("SolidColor");
    shader->Use();
    Matrix4x4 m;
    m.SetTRS(center, Quaternion::identity, size);
    ShaderUniforms uniforms;
    auto v = Camera::main()->worldToCameraMatrix();
    auto p = Camera::main()->projectionMatrix();
    uniforms.vec4s["Color"] = s_color;
    //uniforms.mat4s["MATRIX_MVP"] = p * v * modelMatrix * m;
    Pipeline::perDrawUniformData.MATRIX_MVP = p * v * modelMatrix * m;
    Pipeline::BindPerDrawUniforms();
    shader->BindUniforms(uniforms);
    s_boxMesh->Render();
}

void Gizmos::
DrawWireCapsule(const Vector3& center,
                const float radius,
                const float height,
                const Matrix4x4& modelMatrix)
{
    Vector3 c1 = center+Vector3(0, height*0.5f-radius, 0);
    Vector3 c2 = center-Vector3(0, height*0.5f-radius, 0);
//    glBindVertexArray(s_circleMesh->m_VAO);
//    glDrawArrays(GL_LINE, 0, s_circleMesh->m_positionBuffer.size()/3/2+1);
//    glBindVertexArray(0);
    DrawHalfWireSphere(c1, radius);
    DrawHalfWireSphere(c2, radius, Matrix4x4::Scale(-1, 1, 1));
    
    Vector3 offset(radius, 0, 0);
    DrawLine(c1+offset, c2+offset);
    offset.Set(-radius, 0, 0);
    DrawLine(c1+offset, c2+offset);
    offset.Set(0, 0, radius);
    DrawLine(c1+offset, c2+offset);
    offset.Set(0, 0, -radius);
    DrawLine(c1+offset, c2+offset);
}
