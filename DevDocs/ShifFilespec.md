# This is a file specification for Shader Interface (.shif) files. 
**_Version 2_**

Shader Interface files specify how a number of parts and snippets of glsl code, including reticle's environment definition, alternatives for various stuff, and user-defined code. It also specifies the uniforms, all the alternatives for various rendering pipelines and pipelines supported in the first place. While this is a gist of the features to be added in the near future, it will be added gradually.

## Basic seperator syntax:

 -  Curly braces (`{}`) are used to specify sections. Names of sections come before the opening bracket.
 -  Box brackets (`[]`) are used to specify alternative lists. Each item in the list is a comma-seperated list of items following syntax `<name> : <value>`.
 -  Attributes within sections can iether be specified using positions, or using names, using the syntax `<name> = <value>`. Attribute definitions end with semicolons.
 -  Strings are enclosed in double quotes (`""`).
