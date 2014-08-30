#version 430 core

layout(triangles_adjacency) in;
layout(line_strip, max_vertices=2) out;

uniform uint isSilhouette;

void main(void)
{
   float orient024 = determinant(mat3(gl_in[0].gl_Position.xyz, 
                                      gl_in[2].gl_Position.xyz,
									  gl_in[4].gl_Position.xyz));
   float orient120 = determinant(mat3(gl_in[1].gl_Position.xyz, 
                                      gl_in[2].gl_Position.xyz,
									  gl_in[0].gl_Position.xyz));
   float orient234 = determinant(mat3(gl_in[2].gl_Position.xyz, 
                                      gl_in[3].gl_Position.xyz,
									  gl_in[4].gl_Position.xyz));									  
   float orient045 = determinant(mat3(gl_in[0].gl_Position.xyz, 
                                      gl_in[4].gl_Position.xyz,
									  gl_in[5].gl_Position.xyz));
									  
   if (isSilhouette == 1)
   {								 
      if ( orient024 * orient120 <= 0.0 )
      {
         gl_Position = gl_in[0].gl_Position;
         EmitVertex( );
         gl_Position = gl_in[2].gl_Position;
         EmitVertex( );
         EndPrimitive( );
      }
      if ( orient024 * orient234 <= 0.0 )
      {
         gl_Position = gl_in[2].gl_Position;
         EmitVertex( );
         gl_Position = gl_in[4].gl_Position;
         EmitVertex( );
         EndPrimitive( );
      }
      if ( orient024 * orient045 <= 0.0 )
      {
         gl_Position = gl_in[4].gl_Position;
         EmitVertex( );
         gl_Position = gl_in[0].gl_Position;
         EmitVertex( );
         EndPrimitive( );
      }
   }
   
   if (isSilhouette == 0)
   {		
      gl_Position = gl_in[0].gl_Position;
      EmitVertex( );
      gl_Position = gl_in[2].gl_Position;
      EmitVertex( );
      EndPrimitive( );
  
      gl_Position = gl_in[2].gl_Position;
      EmitVertex( );
      gl_Position = gl_in[4].gl_Position;
      EmitVertex( );
      EndPrimitive( );
	  
      gl_Position = gl_in[4].gl_Position;
      EmitVertex( );
      gl_Position = gl_in[0].gl_Position;
      EmitVertex( );
      EndPrimitive( );
   }
}