#version 420 core
layout (location = 0) in vec3 inputVertexPosition;
layout (location = 1) in vec3 inputVertexColor;
layout (location = 3) in vec3 inputVertexNormal;

out vec3 vertexWorldPosition;
out vec3 vertexColor;
out vec3 vertexNormal;

uniform mat4 view;
uniform mat4 projection;

struct Transform
{
  vec4 position;
  vec4 axis_angle;
};
uniform Transform transform;


vec4 quat_from_axis_angle(vec3 axis, float angle)
{ 
	vec4 qr;
	float half_angle = (angle * 0.5f) * 3.14159f / 180.f;
	qr.x = axis.x * sin(half_angle);
	qr.y = axis.y * sin(half_angle);
	qr.z = axis.z * sin(half_angle);
	qr.w = cos(half_angle);
	return qr;
}

vec4 quat_conj(vec4 q)
{ 
	return vec4(-q.x, -q.y, -q.z, q.w); 
}
  
vec4 quat_mult(vec4 q1, vec4 q2)
{ 
	vec4 qr;
	qr.x = (q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y);
	qr.y = (q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x);
	qr.z = (q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w);
	qr.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);
	return qr;
}

vec3 rotate_vertex_position(vec3 position, vec3 axis, float angle)
{ 
	vec4 qr = quat_from_axis_angle(axis, angle);
	vec4 qr_conj = quat_conj(qr);
	vec4 q_pos = vec4(position.x, position.y, position.z, 0.f);
	
	vec4 q_tmp = quat_mult(qr, q_pos);
	qr = quat_mult(q_tmp, qr_conj);
	
	return vec3(qr.x, qr.y, qr.z);
}

void main()
{
    vec3 pos = rotate_vertex_position(inputVertexPosition, transform.axis_angle.xyz, transform.axis_angle.w);
    pos += transform.position.xyz;
    vertexColor = inputVertexColor;
    vertexNormal = inputVertexNormal;  
    
    gl_Position = projection * view  * vec4(pos, 1.f);
    vertexWorldPosition = pos;
}
