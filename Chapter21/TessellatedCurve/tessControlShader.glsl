#version 430 core

layout(vertices=4) out;

uniform float tessLevelOuter1;

void main(void)
{
   switch(gl_InvocationID)
   {
      case 0: gl_out[ gl_InvocationID ].gl_Position = gl_in[0].gl_Position;
	  break;
	  case 1: gl_out[ gl_InvocationID ].gl_Position = (gl_in[0].gl_Position + gl_in[1].gl_Position)/2.0;
	  break;
	  case 2: gl_out[ gl_InvocationID ].gl_Position = (gl_in[1].gl_Position + gl_in[2].gl_Position)/2.0;
	  break;
	  case 3: gl_out[ gl_InvocationID ].gl_Position = gl_in[2].gl_Position;
	  break;
	  default:
	  break;
   }

   gl_TessLevelOuter[0] = 1.0;
   gl_TessLevelOuter[1] = tessLevelOuter1;
}
