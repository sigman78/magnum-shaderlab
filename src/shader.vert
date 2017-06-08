#if (!defined(GL_ES) && __VERSION__ >= 130) || (defined(GL_ES) && __VERSION__ >= 300)
#define NEW_GLSL
#endif

#ifndef NEW_GLSL
attribute mediump vec2 position;
#endif

void main() {
    #ifdef NEW_GLSL
    gl_Position = vec4((gl_VertexID == 2) ?  3.0 : -1.0,
                       (gl_VertexID == 1) ? -3.0 :  1.0, 0.0, 1.0);
    #else
    gl_Position = position;
    #endif
}
