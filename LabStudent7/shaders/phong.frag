//
// Phong shading. Fragment shader.
//
varying vec3 normal;
varying vec3 vertex;

uniform int numLights;
uniform sampler2D tex;

const vec4 AMBIENT_BLACK = vec4(0.0, 0.0, 0.0, 1.0);
const vec4 DEFAULT_BLACK = vec4(0.0, 0.0, 0.0, 0.0);

// Determine if a light has any illumination
bool isLightEnabled(in int i)
{
   return ((gl_LightSource[i].ambient  != AMBIENT_BLACK) ||
           (gl_LightSource[i].diffuse  != DEFAULT_BLACK) ||
           (gl_LightSource[i].specular != DEFAULT_BLACK));
}

// Convenience method to compute attenuation for the ith light source
// given a distance
float calculateAttenuation(in int i, in float distance)
{
    return (1.0 / (gl_LightSource[i].constantAttenuation +
                   gl_LightSource[i].linearAttenuation    * distance +
                   gl_LightSource[i].quadraticAttenuation * distance * distance));
}

// Convenience method to compute the ambient, diffuse, and specular
// contribution of directional light source i
void directionalLight(in int i, in vec3 N, in vec3 vtx, in float shininess,
                      inout vec4 ambient, inout vec4 diffuse, 
                      inout vec4 specular)
{
   // Add light source ambient
   ambient += gl_LightSource[i].ambient;
   
   // Normalize the light direction. Note: we could use a varying parameter 
   // to pass in the normalized light source direction. However, the cost of
   // doing the perspective correct interpolation may be higher than just
   // getting the vector here.
   vec3 L = normalize(gl_LightSource[i].position.xyz);
   
   // Dot product of normal and light direction
   float nDotL = dot(N, L);
   if (nDotL > 0.0)
   {
      // Add light source diffuse modulated by NDotL
      diffuse += gl_LightSource[i].diffuse * nDotL;
      
      // Construct a unit length vector from the vertex to the camera  
      vec3 V = normalize(-vtx);
      
      // Construct the halfway vector - note that we are assuming local
      // viewpoint
      vec3 H = normalize(L + V);
      
      // Find dot product of N and H
      float nDotH = dot(N, H);
      if (nDotH > 0.0)
      {
         // Add specular contribution due to this light source
         specular += gl_LightSource[i].specular * pow(nDotH, shininess);
      }
   }
}

// Convenience method to compute the ambient, diffuse, and specular
// contribution of point light source i
void pointLight(in int i, in vec3 N, in vec3 vtx, in float shininess,
                inout vec4 ambient, inout vec4 diffuse, inout vec4 specular)
{
   // Construct a vector from the vertex to the light source. Find the length for
   // use in attenuation. Normalize that vector (L)
   // distance 
   vec3 tmp = gl_LightSource[i].position.xyz - vtx;
   float dist = length(tmp);
   vec3 L = normalize(tmp);

   // Compute attenuation
   float attenuation = calculateAttenuation(i, dist);  
   
   // Attenuate the light source ambient contribution
   ambient += gl_LightSource[i].ambient * attenuation;
   
   // Determine dot product of normal with L. If < 0 the light is not 
   // incident on the front face of the surface.
   float nDotL = dot(N, L);
   if (nDotL > 0.0)
   {
      // Add diffuse contribution of this light source
      diffuse  += gl_LightSource[i].diffuse  * attenuation * nDotL;
      
      // Construct a unit length vector from the vertex to the camera  
      vec3 V = normalize(-vtx);
      
      // Construct the halfway vector
      vec3 H = normalize(L + V);
      
      // Add specular contribution (if N dot H > 0)
      float nDotH = dot(N, H);
      if (nDotH > 0.0)
         specular += gl_LightSource[i].specular * attenuation * pow(nDotH, shininess);

/**    
      // Lets use the reflection vector!
      vec3 R = reflect(-L, N);
      
      // Add specular contribution (if R dot V > 0)
      float rDotV = dot(R, V);
      if (rDotV > 0.0)
         specular += gl_LightSource[i].specular * attenuation * pow(rDotV, shininess);
**/
    }
}

void spotLight(in int i, in vec3 N, in vec3 vtx, in float shininess,
               inout vec4 ambient, inout vec4 diffuse, inout vec4 specular)
{
   // Construct a vector from the vertex to the light source. Find the length for
   // use in attenuation. Normalize that vector (L)
   // distance 
   vec3 tmp = gl_LightSource[i].position.xyz - vtx;
   float dist = length(tmp);
   vec3 L = normalize(tmp);

   // Compute attenuation
   float attenuation = calculateAttenuation(i, dist);  
      
   // Determine dot product of normal with L. If < 0 the light is not 
   // incident on the front face of the surface.
   float nDotL = dot(N, L);
   if (nDotL > 0.0)
   {
      // Get the spotlight effect
      float spotEffect = dot(normalize(gl_LightSource[i].spotDirection), -L);
       
      // See if within the cutoff angle
      if (spotEffect > gl_LightSource[i].spotCosCutoff)
      {
         attenuation *= pow(spotEffect, gl_LightSource[i].spotExponent);
            
         // Add diffuse contribution of this light source
         diffuse  += gl_LightSource[i].diffuse  * attenuation * nDotL;

         // Construct a unit length vector from the vertex to the camera  
         vec3 V = normalize(-vtx);

         // Construct the halfway vector
         vec3 H = normalize(L + V);
         
         // Add specular contribution (if N dot H > 0)
         float nDotH = dot(N, H);
         if (nDotH > 0.0)
            specular += gl_LightSource[i].specular * attenuation * pow(nDotH, shininess);

/**      
         // Lets use the reflection vector!
         vec3 R = reflect(-L, N);
         
         // Add specular contribution (if R dot V > 0)
         float rDotV = dot(R, V);
         if (rDotV > 0.0)
            specular += gl_LightSource[i].specular * attenuation * pow(rDotV, shininess);
**/
      }
      else
         attenuation = 0.0;
    }
   
   // Attenuate the light source ambient contribution (note that this can
   // be modulated by the spotlight and if outside the spotlight cutoff
   // we will have no ambient contribution. (Unclear if this is correct!)
   ambient += gl_LightSource[i].ambient * attenuation;
}

// Main fragment shader. 
void main()
{
   // Normalize the normal - as a linear interpolation of normals can result
   // in non unit-length vectors. Cannot directly modify the varying value, so
   // create a temporary variable here. 
   vec3 n = normalize(normal);
   
   // Initialize the illumination for this fragment
   vec4 ambient  = vec4(0.0);
   vec4 diffuse  = vec4(0.0);
   vec4 specular = vec4(0.0);
   vec4 textureColor;
	textureColor = texture2D(tex,gl_TexCoord[0].st);  
   
   // Iterate through all lights to determine the illumination striking this
   // pixel. Use the uniform variable numlights passed in by the application
   // to go use only lights up to the highest number specified.
   for (int i = 0; i < numLights; i++)
   {
      if (isLightEnabled(i))
      {
         if (gl_LightSource[i].position.w == 0.0)
            directionalLight(i, n, vertex, gl_FrontMaterial.shininess, ambient, diffuse, specular);
         else if (gl_LightSource[i].spotCutoff == 180.0)
            pointLight(i, n, vertex, gl_FrontMaterial.shininess, ambient, diffuse, specular);
         else
            spotLight(i, n, vertex, gl_FrontMaterial.shininess, ambient, diffuse, specular);
      }
   }

   // Note: gl_FrontLightModelProduct includes the material emission plus the
   // global ambient
   vec4 color = (textureColor!=vec4(0.0) ? textureColor : vec4(1.0)) * (gl_FrontLightModelProduct.sceneColor  +
                  (ambient  * gl_FrontMaterial.ambient) +
                  (diffuse  * gl_FrontMaterial.diffuse) +
                  (specular * gl_FrontMaterial.specular));
             
   color = clamp(color, 0.0, 1.0);

   gl_FragColor = color;
}
