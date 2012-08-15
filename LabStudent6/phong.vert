uniform int numLights;
varying vec3 normal;
varying vec4 ecPos;
  
void main( void )
{
	normal = normalize(gl_NormalMatrix * gl_Normal);
	ecPos = gl_ModelViewMatrix * gl_Vertex;
	gl_Position = ftransform();
}

