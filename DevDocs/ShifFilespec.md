# This is a file specification for Shader Interface (.shif) files. 
**_Version 2_**

Shader Interface files specify how a number of parts and snippets of glsl code, including reticle's environment definition, alternatives for various stuff, and user-defined code. It also specifies the uniforms, all the alternatives for various rendering pipelines and pipelines supported in the first place. While this is a gist of the features to be added in the near future, it will be added gradually.

Shif files are agnostic to whitespace formatting. Ignored characters are: Space (`' '`), Horizontal Tab (`'\t'`), Vertical Tab (`'\v'`), New Line (`'\n'`), Carriage Return (`'\r'`) and Form Feed (`'\f'`).

## Basic seperator syntax:

 -  Curly braces (`{}`) are used to specify sections. Names of sections come before the opening bracket, enclosed in round brackets (`()`).
 -  Box brackets (`[]`) are used to specify alternative lists. Each item in the list is a comma-seperated list of items following syntax `<name> : <value>`.
 -  Attributes within sections can be ordered or unordered lists using the syntax `<name> = <value>;`or the syntax `<name>;`. Attribute definitions end with semicolons.
 -  Strings are enclosed in double quotes (`""`).

## Sections:

### Section TransformData:

Details about which transfrom values to be passed. Value List:

 -  Name `PassMVP`. Possible values: `0` or `1`. If `1`, indicates that MVP matrix needs to be passed through `mat4 mvp_matrix`. Default 1.
 -  Name `PassModel`. Possible values: `0` or `1`. If `1`, indicates that model matrix needs to be passed through `mat4 model_matrix`. Default 0.

### Section LightingData:

This section contains details regarding the lighitng model and pipeline the shader belongs to. Value list:

 -  Name `PassLightPos`. Possible values: `0` or `1`. If `1`, it indicates that light positions must be passed to the shader through `vec3 lights[i].pos`. Default 1.
 -  Name `PassLightColor`. Possible values: `0` or `1`. If `1`, it indicates that light color (strength multiplied) must be passed to the shader through `vec3 lights[i].color`.  Default 1.
 -  Name `PassKl`. Possible values: `0` or `1`. If `1`, indicates that Kl values must be passed through `float lights[i].kl`. Default 1.
 -  Name `PassKp`. Possible values: `0` or `1`. If `1`, idnicates that Kq values must be passed through `float lights[i].kq`. Default 1.
 -  Name `MaxLights`. Integer values. Indicates the number of total lights the shader's uniform light buffer has.

### Section TexLists:

List of texture uniform names, in double quotes (`""`), seperated by semicolons.

### Section Code:

Specification for code files.

 -  Name `VertexShader`. String. Path relative to .shif file to vertex shader code in .shdr files. Must be specified.
 -  Name `FragmentShader`. String. Path relative to .shif to fragment shader code in .shdr files. Must be specified.