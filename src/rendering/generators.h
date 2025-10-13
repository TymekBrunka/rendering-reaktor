#ifndef _RR_DENINED_GENERATORS
#define _RR_DENINED_GENERATORS

#include <glad/gl.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

GLint f__RR_AUTOATTRIB_GL_SIZE(glm::vec2 v) {
    return 2;
}

GLint f__RR_AUTOATTRIB_GL_SIZE(glm::vec3 v) {
    return 3;
}

GLenum f__RR_AUTOATTRIB_GL_TYPE(glm::vec2 v) {
    return GL_FLOAT;
}

GLenum f__RR_AUTOATTRIB_GL_TYPE(glm::vec3 v) {
    return GL_FLOAT;
}

#define RR_AUTOATTRIB(Struct, Field, Normalized) f__RR_AUTOATTRIB(program, &Struct::Field, Normalized, this->location_##Field, #Field, (void*)offsetof(Struct, Field))

template<typename T, typename F>
void f__RR_AUTOATTRIB(GLuint& program, F T::* Field, GLboolean normalized, GLint& Location, const char* locname, void* offset) {
    Location = glGetAttribLocation(program, locname);
    glEnableVertexAttribArray(Location);
    glVertexAttribPointer(
        Location,
        f__RR_AUTOATTRIB_GL_SIZE(*Field),
        f__RR_AUTOATTRIB_GL_TYPE(*Field),
        normalized,
        sizeof(T), offset
    );
}

#endif