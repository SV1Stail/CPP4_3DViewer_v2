#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;

flat out vec3 startPos;
out vec3 vertPos;

void main() {
   // gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
   vec4 pos = mat_projection * mat_view * mat_model * vec4(aPos, 1.0); // после перемножения получим эффект вращения и смещения
   gl_Position = pos;
   vertPos     = pos.xyz / pos.w;
   startPos    = vertPos;
}
