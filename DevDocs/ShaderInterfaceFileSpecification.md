Shader Interface Files (.shif) specify which vertex shader source and fragment source should be linked to form a shader, and list the uniforms
(textures, colors, floats etc.) of the shaders which should be accessible through materials created with the shif file.

Thus, the shader workflow is:

1) Write shader vertex and fragment code.
2) Write .shif file, putting links to vertex and fragment code as well as a list of all uniforms.
3) Create a RetiMaterial object with the .shif file. This automatically loads shader code.
4) Set the textures, colors etc through the RetiMaterial.
5) Use the RetiMaterial object (which automatically activates the related shader) and render the required stuff.

SHIF FILE SPECIFICATION:

Line No			Description

1			Path to Vertex shader source
2			Path to Fragment shader source
3			Number of textures
4..a			List of texture names
a+1			EOF.

