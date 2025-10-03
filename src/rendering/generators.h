#ifndef __RR_R_R_GENERATORS
#define __RR_R_R_GENERATORS

#define RR_AUTOATTRIB(Stuct, Name, Location) \
    Location = glGetUniformLocation(program, ##Name);\
    glEnableVertexAttribArray(Location); \
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, \
                          sizeof(Struct), (void*) offsetof(Struct, Name));
#endif
