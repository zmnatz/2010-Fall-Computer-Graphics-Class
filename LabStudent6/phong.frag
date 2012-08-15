uniform int numLights;
varying vec3 normal;  
varying vec4 ecPos;

void main()
{
	//Start with ambient light
	vec4 color = gl_LightModel.ambient * gl_FrontMaterial.ambient;
	
	//Loop through lights
	for(int i=0;i<numLights;i++){
		//Light is directional if w=0
		if(gl_LightSource[i].position.w==0.0){
			/* compute the dot product between normal and ldir */
			float NdotL = max(dot(normal,normalize(vec3(gl_LightSource[i].position))),0.0);
			if (NdotL > 0.0) {
				color += gl_FrontMaterial.diffuse * gl_LightSource[i].diffuse * NdotL;
				float NdotHV = max(dot(normal,normalize(gl_LightSource[i].halfVector.xyz)),0.0);
				color += gl_FrontMaterial.specular * gl_LightSource[i].specular * 
						pow(NdotHV, gl_FrontMaterial.shininess);
			}
		} else {
			//Light has a set position
			vec3 aux = vec3(gl_LightSource[i].position-ecPos);
			float dist = length(aux);
			vec3 lightDir = normalize(aux);
			/* compute the dot product between normal and normalized lightdir */
			float NdotL = max(dot(normal,normalize(lightDir)),0.0);
			if (NdotL > 0.0) {
				//Spotlight if spotCutoff<100 (value is between 0,90 or is 180
				if(gl_LightSource[i].spotCutoff<100.0){
					float spotEffect = dot(normalize(gl_LightSource[i].spotDirection), normalize(-lightDir));
					if (spotEffect > gl_LightSource[i].spotCosCutoff) {
						spotEffect = pow(spotEffect, gl_LightSource[i].spotExponent);
						float att = spotEffect / (gl_LightSource[i].constantAttenuation +
								gl_LightSource[i].linearAttenuation * dist +
								gl_LightSource[i].quadraticAttenuation * dist * dist);
						color += att * (gl_FrontMaterial.diffuse * gl_LightSource[i].diffuse
											* NdotL + gl_FrontMaterial.ambient * gl_LightSource[i].ambient);
						vec3 halfVector = normalize(gl_LightSource[i].halfVector.xyz);
						float NdotHV = max(dot(normal,halfVector),0.0);
						color += att * gl_FrontMaterial.specular * gl_LightSource[i].specular * 
										pow(NdotHV,gl_FrontMaterial.shininess);
					}
				}else{
					//Standard position light
					float att = 1.0 / (gl_LightSource[i].constantAttenuation +
							gl_LightSource[i].linearAttenuation * dist +
							gl_LightSource[i].quadraticAttenuation * dist * dist);
					color += att * (gl_FrontMaterial.diffuse * gl_LightSource[i].diffuse * NdotL + 
									gl_FrontMaterial.ambient * gl_LightSource[i].ambient);
					vec3 halfVector = normalize(gl_LightSource[i].halfVector.xyz);
					float NdotHV = max(dot(normal,halfVector),0.0);
					color += att * gl_FrontMaterial.specular * gl_LightSource[i].specular * 
									pow(NdotHV,gl_FrontMaterial.shininess);
				}
			}
		}
	}
	gl_FragColor = color;
}