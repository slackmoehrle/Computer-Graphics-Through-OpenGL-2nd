#version 430 core

uniform vec4 pointSetColor;
uniform sampler2D starTex;
uniform uint renderChoice;

out vec4 colorsOut;

const vec4 backgroundColor = vec4(1.0, 1.0, 1.0, 0.0);
const vec2 pointCoordCenter = vec2(0.5);
const float pointCoordRadius = 0.5;

vec2 coordWRTcenter;
float distFromCenter, alpha;
float startBlend = 0.4;

void main(void)
{
   switch(renderChoice)
   {
      case 0:
	     colorsOut = pointSetColor;
		 break;
	  case 1:
         coordWRTcenter = gl_PointCoord - pointCoordCenter;
         distFromCenter = sqrt(dot(coordWRTcenter, coordWRTcenter));
         if (distFromCenter > pointCoordRadius) discard;	  
		 colorsOut = pointSetColor;
		 break;
	  case 2:
         coordWRTcenter = gl_PointCoord - pointCoordCenter;
         distFromCenter = sqrt(dot(coordWRTcenter, coordWRTcenter));
         if (distFromCenter > pointCoordRadius) discard;
         alpha = clamp( (distFromCenter - startBlend)/
		                (pointCoordRadius - startBlend), 0.0, 1.0 );
         colorsOut = mix(pointSetColor, backgroundColor, alpha);
		 break;
	  case 3:
	     colorsOut = texture(starTex, gl_PointCoord);
		 break;
      default:
         break;
   }
}