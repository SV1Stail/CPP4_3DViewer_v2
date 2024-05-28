#version 330 core

flat in vec3 startPos;
in vec3 vertPos;

out vec4 FragColor;

uniform vec4 ourColor;
uniform bool isVertex;
uniform bool isDashed;
uniform int shape;

uniform vec2  u_resolution;
uniform float u_dashSize;
uniform float u_gapSize;

void main() {
  // полярная система координат
  vec2 p = gl_PointCoord* 2.0 - vec2(1.0);
  float r = sqrt(dot(p,p));
  float theta = atan(p.y,p.x);

  if (!isVertex) {
    if (!isDashed) {
      FragColor = ourColor;
    } else {
        vec2  dir  = (vertPos.xy-startPos.xy) * u_resolution/2.0;
        float dist = length(dir);

        if (fract(dist / (u_dashSize + u_gapSize)) > u_dashSize/(u_dashSize + u_gapSize))
          discard;
        FragColor = ourColor;
    }
  } else if (shape == 1) {
        if(dot(p,p) > cos(theta*5))
            discard; 
        else 
            FragColor = ourColor;
    } else if (shape == 2) {
        if(dot(p,p) > 0.5*(exp(cos(theta*5)*0.75)))
            discard; 
        else 
            FragColor = ourColor;
    } else if (shape == 3) {
        if(dot(p,p)> 5.0/cos(theta-20*r))
            discard; 
        else 
            FragColor = ourColor;
    } else if (shape == 4) {
        if(dot(p,p) > r || dot(p,p) < r*0.75)
            discard; 
        else 
            FragColor = ourColor;
    } else if (shape == 5) {
        if(dot(gl_PointCoord-0.5,gl_PointCoord-0.5)>0.25)
            discard; 
        else 
            FragColor = ourColor;
    } else {
        FragColor = ourColor;
    }
}
