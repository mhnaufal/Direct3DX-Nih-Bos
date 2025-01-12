# Based On: ChillWin

# DirectX 11 Nih BOSSKUU

## Learning

**Vertex** = Location and properties of an point in 3D space. Location defines with 3 numerical coordinate values.

**Input Layout** = Data containing the location and properties of a vertex.

The vertex buffers for rendering was copied from System Memory into Video Memory by DirectX COM object.

## Resources

1. http://www.directxtutorial.com/LessonList.aspx?listid=11
2. https://alain.xyz/blog/raw-directx12
3. https://www.3dgep.com/learning-directx-12-1/

## Extra Notes

1. Tell the GPU that we want render a triangle (shader program)
	Rendering pipeline holds the flow of rendering with the help of
	Shader to customize how the render would work.
		a. Load and compile the Shader file
		b. Change the Shader file into Shader object
		c. Set the Shader to active
2. Create the triangle vertices
3. Stores the vertices to the video card memory
4. Tell GPU to read those values
5. Render the triangle

**Steps To Render**
_http://www.directxtutorial.com/Lesson.aspx?lessonid=11-4-5_
1. Create *shader*			: tell the GPU that we want to render an image
2. Create *vertex buffer*	: hold informations about the "image" that we want to render (position, color, etc)
3. Create *input layout*	: combine the shader & vertex buffer in order to be able to render
4. DRAW!



