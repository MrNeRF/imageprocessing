#version 420 core
layout (location = 0) in vec3 inputVertexPosition;
layout (location = 1) in vec3 inputVertexColor;
layout (location = 3) in vec3 inputVertexNormal;

out vec3 vertexWorldPosition;
out vec3 vertexColor;
out vec3 vertexNormal;

uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalRotationMatrix;

struct Transform
{
  vec4 position;
  vec4 qOrientation;
  vec4 qconjOrientation;
};
uniform Transform transform;

vec4 quat_mult(vec4 q1, vec4 q2)
{ 
	vec4 qr;
	qr.x = (q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y);
	qr.y = (q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x);
	qr.z = (q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w);
	qr.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);
	return qr;
}

vec3 rotate_vertex_position(vec3 position, vec4 qr, vec4 qr_conj)
{ 
	vec4 q_pos = vec4(position.x, position.y, position.z, 0.f);
	
	vec4 q_tmp = quat_mult(qr, q_pos);
	qr = quat_mult(q_tmp, qr_conj);
	
	return vec3(qr.x, qr.y, qr.z);
}

void main()
{
    vec3 pos = rotate_vertex_position(inputVertexPosition, transform.qOrientation.xyzw, transform.qconjOrientation.xyzw);
    pos += transform.position.xyz;
    vertexColor = inputVertexColor;
    vertexNormal = transpose(inverse(normalRotationMatrix)) * inputVertexNormal;  
    
    gl_Position = projection * view  * vec4(pos, 1.f);
    vertexWorldPosition = pos;
}
