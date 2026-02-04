#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

void main()
{
    // Show UVs as color (U = red, V = green)
    if (TexCoords.x < 0.0 || TexCoords.x > 1.0 || TexCoords.y < 0.0 || TexCoords.y > 1.0)
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red for out-of-bounds UVs
    else
        FragColor = vec4(TexCoords, 0.0, 1.0); // Gradient for valid UVs
}
