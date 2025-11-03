#ifndef _RR_DENINED_GENERATORS
#define _RR_DENINED_GENERATORS

#include <glad/gl.h>
// #include <glm/vec2.hpp>
// #include <glm/vec3.hpp>

namespace RR {
    typedef float vec2[2];
    typedef float vec3[3];

    template<typename T>
    struct ATTRIB_SPECS {
        static const GLint size = -2137;
        static const GLenum type = GL_FLOAT;
    };

    template<>
    struct ATTRIB_SPECS<RR::vec2> {
        static const GLint size = 2;
        static const GLenum type = GL_FLOAT;
    };

    template<>
    struct ATTRIB_SPECS<RR::vec3> {
        static const GLint size = 3;
        static const GLenum type = GL_FLOAT;
    };

    #define RR_AUTOATTRIB(Struct, Field, Normalized) RR::f__RR_AUTOATTRIB<decltype(Struct::Field)>(program.id, Normalized, location_##Field, (const char*)#Field, (void*)offsetof(Struct, Field), sizeof(Struct))

    template<typename T/*, typename F*/>
    inline void f__RR_AUTOATTRIB(GLuint program, /*F T::* T Field,*/ GLboolean normalized, GLint& Location, const char* locname, const void* offset, GLsizei stride) {
        Location = glGetAttribLocation(program, locname);
        glEnableVertexAttribArray(Location);
        glVertexAttribPointer(
            Location,
            RR::ATTRIB_SPECS<T>::size,
            RR::ATTRIB_SPECS<T>::type,
            normalized,
            stride, offset
        );
    }
}

#endif