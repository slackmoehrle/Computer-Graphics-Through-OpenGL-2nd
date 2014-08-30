#version 430 core

layout(isolines, equal_spacing) in;

vec4 q0, q1, q2, q3;
float u, c0, c1, c2, c3;

void main( )
{
   q0 = gl_in[0].gl_Position;
   q1 = gl_in[1].gl_Position;
   q2 = gl_in[2].gl_Position;
   q3 = gl_in[3].gl_Position;

   u = gl_TessCoord.x;

   c0 = (1.0-u) * (1.0-u) * (1.0-u);
   c1 = 3.0 * u * (1.0-u) * (1.0-u);
   c2 = 3.0 * u * u * (1.0-u);
   c3 = u * u * u;

   gl_Position = c0*q0 + c1*q1 + c2*q2 + c3*q3;
}
