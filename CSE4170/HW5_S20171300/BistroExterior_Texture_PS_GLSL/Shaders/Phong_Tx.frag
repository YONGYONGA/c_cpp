#version 400

// #define DISPLAY_LOD

struct LIGHT {
	vec4 position; // assume point or direction in EC in this example shader
	vec4 ambient_color, diffuse_color, specular_color;
	vec4 light_attenuation_factors; // compute this effect only if .w != 0.0f
	vec3 spot_direction;
	float spot_exponent;
	float spot_cutoff_angle;
	bool light_on;
};

struct MATERIAL {
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
	vec4 emissive_color;
	float specular_exponent;
};

uniform vec4 u_global_ambient_color;
#define NUMBER_OF_LIGHTS_SUPPORTED 5
uniform LIGHT u_light[NUMBER_OF_LIGHTS_SUPPORTED];
uniform MATERIAL u_material;

uniform sampler2D u_base_texture;
uniform sampler2D u_normal_texture;
uniform sampler2D u_emissive_texture;

uniform bool u_flag_diffuse_texture_mapping = false;
uniform bool u_flag_normal_texture_mapping = false;
uniform bool u_flag_emissive_texture_mapping = false;
uniform bool u_normal_based_directX = false;

//블렌딩
uniform int u_flag_blending = 0;
uniform float u_fragment_alpha = 1.0f;

//하트이펙트
uniform bool screen_effect = false; 
//무지개 이펙트
uniform bool screen_effect2 = false; 
uniform bool rainbow = false; 

uniform float screen_frequency = 5.0f;
uniform float screen_width = 0.125f;

//추가
uniform float start_a=0.0f;
uniform float end_a=20.0f;
uniform bool moving_effect=false;

uniform bool u_flag_fog = false;

const float zero_f = 0.0f;
const float one_f = 1.0f;

in vec3 v_position_EC;
in vec3 v_normal_EC;
in vec2 v_tex_coord;

in vec2 v_position_sc;


layout (location = 0) out vec4 final_color;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(u_normal_texture, v_tex_coord).xyz * 2.0 - 1.0;
	if (u_normal_based_directX)
	    tangentNormal.z *= -1;  // for normal map based in directX

    vec3 Q1  = dFdx(v_position_EC);
    vec3 Q2  = dFdy(v_position_EC);
    vec2 st1 = dFdx(v_tex_coord);
    vec2 st2 = dFdy(v_tex_coord);

    vec3 N   = normalize(v_normal_EC);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

vec4 lighting_equation_textured(in vec3 P_EC, in vec3 N_EC, in vec4 base_color, in vec4 emissive_color) {
	vec4 color_sum;
	float local_scale_factor, tmp_float; 
	vec3 L_EC;

	color_sum = emissive_color + u_global_ambient_color * base_color;
 
	for (int i = 0; i < NUMBER_OF_LIGHTS_SUPPORTED; i++) {
		if (!u_light[i].light_on) continue;

		local_scale_factor = one_f;
		if (u_light[i].position.w != zero_f) { // point light source
			L_EC = u_light[i].position.xyz - P_EC.xyz;

			if (u_light[i].light_attenuation_factors.w  != zero_f) {
				vec4 tmp_vec4;

				tmp_vec4.x = one_f;
				tmp_vec4.z = dot(L_EC, L_EC);
				tmp_vec4.y = sqrt(tmp_vec4.z);
				tmp_vec4.w = zero_f;
				local_scale_factor = one_f/dot(tmp_vec4, u_light[i].light_attenuation_factors);
			}

			L_EC = normalize(L_EC);

			if (u_light[i].spot_cutoff_angle < 180.0f) { // [0.0f, 90.0f] or 180.0f
				float spot_cutoff_angle = clamp(u_light[i].spot_cutoff_angle, zero_f, 90.0f);
				vec3 spot_dir = normalize(u_light[i].spot_direction);

				tmp_float = dot(-L_EC, spot_dir);
				if(i==4){
					if(moving_effect){		
						if (tmp_float >= cos(radians(spot_cutoff_angle))) {
						//켰을때, 이부분이 변하기
							float angle1=radians(start_a);
							float angle2=radians(end_a);
							tmp_float = acos(tmp_float);
							if (tmp_float >=angle1 && tmp_float<=angle2) 
								tmp_float = zero_f;
						}
						else 
							tmp_float = zero_f;
						local_scale_factor *= tmp_float;
					}
					else{
						if (tmp_float >= cos(radians(spot_cutoff_angle))) {
							tmp_float = pow(tmp_float, u_light[i].spot_exponent);
						}
						else 
							tmp_float = zero_f;
						local_scale_factor *= tmp_float;	
					}
				}
				else{
					if (tmp_float >= cos(radians(spot_cutoff_angle))) {
						tmp_float = pow(tmp_float, u_light[i].spot_exponent);
					}
					else 
						tmp_float = zero_f;
					local_scale_factor *= tmp_float;
				}
			}
		}
		else {  // directional light source
			L_EC = normalize(u_light[i].position.xyz);
		}	

		if (local_scale_factor > zero_f) {				
		 	vec4 local_color_sum = u_light[i].ambient_color * u_material.ambient_color;

			tmp_float = dot(N_EC, L_EC);  
			if (tmp_float > zero_f) {  
				local_color_sum += u_light[i].diffuse_color*base_color*tmp_float;
			
				vec3 H_EC = normalize(L_EC - normalize(P_EC));
				tmp_float = dot(N_EC, H_EC); 
				if (tmp_float > zero_f) {
					local_color_sum += u_light[i].specular_color
				                       *u_material.specular_color*pow(tmp_float, u_material.specular_exponent);
				}
			}
			color_sum += local_scale_factor * local_color_sum;
		}
	}
 	return color_sum;
}

// May contol these fog parameters through uniform variables
#define FOG_COLOR vec4(0.7f, 0.7f, 0.7f, 1.0f)
#define FOG_NEAR_DISTANCE 350.0f
#define FOG_FAR_DISTANCE 700.0f
#define PI 3.14159f
void main(void) {
	vec4 base_color, emissive_color, shaded_color;
	vec3 normal;
	float fog_factor;
	//하트모양 이펙트용
	if (screen_effect) {
		float x_mod, y_mod;
		x_mod = mod(v_position_sc.x*screen_frequency, 5.0f)-2.5f;
		y_mod = mod(v_position_sc.y*screen_frequency, 5.0f)-2.5f;

		if(pow((x_mod*x_mod+y_mod*y_mod-1.0f),3.0f)-x_mod*x_mod*y_mod*y_mod*y_mod>=0){
			discard;
		}
	}
	if(screen_effect2){
		float x_mod, y_mod;
		x_mod = mod(v_position_sc.x, 1.0f);
		y_mod = mod(v_position_sc.y, 1.0f);
		float re=sin(PI*y_mod)/2;
		if(rainbow){
			if(re+0.5f<x_mod){
				discard;
			}
			else if(re+0.45f<x_mod){
				final_color=vec4(0.5f,0.0f,1.0f,1.0f);
			}
			else if(re+0.40f<x_mod){
				final_color=vec4(0.0f,0.0f,0.5f,1.0f);
			}
			else if(re+0.35f<x_mod){
				final_color=vec4(0.0f,0.0f,1.0f,1.0f);
			}
			else if(re+0.30f<x_mod){
				final_color=vec4(0.0f,1.0f,0.0f,1.0f);
			}
			else if(re+0.25f<x_mod){
				final_color=vec4(1.0f,1.0f,0.0f,1.0f);
			}
			else if(re+0.20f<x_mod){
				final_color=vec4(1.0f,0.5f,0.0f,1.0f);
			}
			else if(re+0.15f<x_mod){
				final_color=vec4(1.0f,0.0f,0.0f,1.0f);
			}
			else
				discard;
			return;
		}
		else{
			if(re+0.5f<x_mod ||re+0.15f>x_mod){
				discard;
			}

		}
	}
	if (u_flag_diffuse_texture_mapping) 
		base_color = texture(u_base_texture, v_tex_coord);
	else {
		base_color = u_material.diffuse_color;
		//printf("??");
	}

	if (u_flag_emissive_texture_mapping) 
		emissive_color = texture(u_emissive_texture, v_tex_coord);
	else 
		emissive_color = u_material.emissive_color;

	if (u_flag_normal_texture_mapping)
		normal = getNormalFromMap();
	else
		normal = v_normal_EC;
	
	shaded_color = lighting_equation_textured(v_position_EC, normalize(normal), base_color, emissive_color);
	if (u_flag_blending != 0) {
		// Back-to-Front if u_flag_blending == 1
		// Front-to-Back if u_flag_blending == 2
		//shaded_color = vec4(0.0f*shaded_color.rgb, 0.0f);

		shaded_color = vec4(u_fragment_alpha*shaded_color.rgb, u_fragment_alpha);
	}



	if (u_flag_fog) {
 	  	fog_factor = (FOG_FAR_DISTANCE - length(v_position_EC.xyz))/(FOG_FAR_DISTANCE - FOG_NEAR_DISTANCE);  		
		fog_factor = clamp(fog_factor, 0.0f, 1.0f);
		final_color = mix(FOG_COLOR, shaded_color, fog_factor);
	}
	else {
		final_color = shaded_color;	
	}
}
