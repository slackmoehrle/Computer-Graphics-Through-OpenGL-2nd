//////////////////////////////////////////////////////////////////////////////////
// sphereInBoxPOV.pov   
//
// This program mimics the scene of sphereInBox1.cpp using POV-Ray. The six sides
// of the box are drawn as rectangular polygons. The light and camera are located
// respectively at the same points as in sphereInBox1.cpp. The surface finishes
// are minimal - to obtain reflection and a specular highlight on the sphere.    
// There is no animation or interaction.
//////////////////////////////////////////////////////////////////////////////////

#include "colors.inc"

// Enable radiosity with default settings.
// global_settings{radiosity{}}
    
// Camera definition    
camera 
{
   location <0.0, 3.0, 3.0>  // Location co-ordinates.
   look_at <0.0, 0.0, 0.0>  // Co-ordinates towards which camera is pointing.  
}
       
background {White} // Background color.
       
// Light source definition.
light_source 
{ 
   <0.0, 1.5, 3.0>  // Location co-ordinates.
   color White  // Light color.
}

// Front face of box.
polygon 
{
   4, // Number of vertices.
   <-1, -1>, <1, -1>, <1, 1>, <-1, 1> // Vertex co-ordinates.          
   translate <0.0, 0.0, 1.0>  // Translate 1 up z-axis.  
   pigment {Red}  // Material color.    
   finish  // Material finish.
   {   
      reflection {0.4}
   }
}

// Back face of box.
polygon 
{
   4, // Number of vertices.
   <-1, -1>, <1, -1>, <1, 1>, <-1, 1> // Vertex co-ordinates.   
   translate <0.0, 0.0, -1.0>  // Translate 1 down z-axis.
   pigment {Red}  // Material color.         
   finish  // Material finish.
   {   
      reflection {0.4}
   }
}

// Top face of box.
polygon 
{
   4, // Number of vertices.
   <-1, -1>, <1, -1>, <1, 1>, <-1, 1> // Vertex co-ordinates.  
   rotate<60.0, 0.0, 0.0> // Rotate 60 about x-axis.
   translate <0.0, 1.5, 0.0>  // Translate 1.5 up y-axis.
   pigment {Red}  // Material color.       
   finish  // Material finish. 
   {   
      reflection {0.4}
   }
}

// Bottom face of box.
polygon 
{
   4, // Number of vertices.
   <-1, -1>, <1, -1>, <1, 1>, <-1, 1> // Vertex co-ordinates.
   rotate<90.0, 0.0, 0.0> // Rotate 90 about x-axis.
   translate <0.0, -1.0, 0.0>  // Translate 1 down y-axis.
   pigment {Red}  // Material color.        
   finish  // Material finish.
   {   
      reflection {0.4}
   }
}

// Left face of box.
polygon 
{
   4, // Number of vertices.
   <-1, -1>, <1, -1>, <1, 1>, <-1, 1> // Vertex co-ordinates.
   rotate<0.0, 90.0, 0.0>  // Rotate 90 about y-axis.
   translate <1.0, 0.0, 0.0>  // Translate 1 up x-axis.
   pigment {Red}  // Material color.        
   finish  // Material finish.
   {  
      reflection {0.4}
   }
}

// Right face of box.
polygon 
{
   4, // Number of vertices.
   <-1, -1>, <1, -1>, <1, 1>, <-1, 1> // Vertex co-ordinates.
   rotate<0.0, 90.0, 0.0>  // Rotate 90 about y-axis.
   translate <-1.0, 0.0, 0.0>  // Translate 1 down x-axis.
   pigment {Red}  // Material color.      
   finish  // Material finish.
   {   
      reflection {0.4}
   }
}

// Sphere.
sphere 
{
   <0, 0, 0>, // Center co-ordinates.
   1.0 // Radius.    
   pigment { Green }  // Material color. 
   finish  // Material finish.
   {               
      specular 2.0
      reflection {0.4} 
   }
}
