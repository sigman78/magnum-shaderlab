#if (!defined(GL_ES) && __VERSION__ >= 130) || (defined(GL_ES) && __VERSION__ >= 300)
#define NEW_GLSL
#endif

#ifndef NEW_GLSL
#define fragmentColor gl_FragColor
#define texture texture2D
#define in varying
#endif

#ifdef NEW_GLSL
out lowp vec4 fragmentColor;
layout(pixel_center_integer) in highp vec4 gl_FragCoord;
#endif


uniform vec3 iResolution;
uniform float iGlobalTime;
uniform float iTimeDelta;
uniform int iTime;
uniform vec4 iMouse;

void main() {
    vec4 fragCoord = gl_FragCoord;
	vec2 uv = fragCoord.xy / iResolution.xy;

	fragmentColor = vec4(uv,0.5+0.5*sin(iGlobalTime),1.0);
}
