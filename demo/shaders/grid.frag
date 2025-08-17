#version 330 core

in vec3 WorldPos;
layout(location = 0) out vec4 color;

float pristineGrid( vec2 uv, vec2 lineWidth)
{
    vec2 ddx = dFdx(uv);
    vec2 ddy = dFdy(uv);
    vec2 uvDeriv = vec2(length(vec2(ddx.x, ddy.x)), length(vec2(ddx.y, ddy.y)));
    bvec2 invertLine = bvec2(lineWidth.x > 0.5, lineWidth.y > 0.5);
    vec2 targetWidth = vec2(
      invertLine.x ? 1.0 - lineWidth.x : lineWidth.x,
      invertLine.y ? 1.0 - lineWidth.y : lineWidth.y
      );
    vec2 drawWidth = clamp(targetWidth, uvDeriv, vec2(0.5));
    vec2 lineAA = uvDeriv * 1.5;
    vec2 gridUV = abs(fract(uv) * 2.0 - 1.0);
    gridUV.x = invertLine.x ? gridUV.x : 1.0 - gridUV.x;
    gridUV.y = invertLine.y ? gridUV.y : 1.0 - gridUV.y;
    vec2 grid2 = smoothstep(drawWidth + lineAA, drawWidth - lineAA, gridUV);

    grid2 *= clamp(targetWidth / drawWidth, 0.0, 1.0);
    grid2 = mix(grid2, targetWidth, clamp(uvDeriv * 2.0 - 1.0, 0.0, 1.0));
    grid2.x = invertLine.x ? 1.0 - grid2.x : grid2.x;
    grid2.y = invertLine.y ? 1.0 - grid2.y : grid2.y;
    return mix(grid2.x, 1.0, grid2.y);
}

void main()
{
    float gridAlpha = pristineGrid(WorldPos.xz, vec2(0.01,0.01));
    vec3 gridColor = vec3(0.35,0.35,0.35);
    if (WorldPos.x < 0.02 && WorldPos.x > -0.02) {
      gridColor = vec3(1.000,0.200,0.322);
    } else if (WorldPos.z < 0.02 && WorldPos.z > -0.02) {
      gridColor = vec3(0.157,0.565,1.0);
      // gridColor = vec3(0.471,0.863,0.000); Green
    }

    color = vec4(gridColor, gridAlpha);
};