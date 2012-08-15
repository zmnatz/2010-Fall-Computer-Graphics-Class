
varying vec3 normal;
varying vec3 vertex;

// Simple shader for Phong (per-pixel) shading. The fragment shader will
// do all the work. We need to pass per-vertex normals to the fragment
// shader. We also will transform the vertex into view coordinates so 
// the fragment shader can interpolate view coordinates.
void main()
{
   // Calculate the normal
   normal = normalize(gl_NormalMatrix * gl_Normal);

   // Transform the vertex position to view coordinates
   vertex = vec3(gl_ModelViewMatrix * gl_Vertex);

   // Transform the vertex position
   gl_Position = ftransform();
   
   gl_TexCoord[0]  = gl_MultiTexCoord0;
}
