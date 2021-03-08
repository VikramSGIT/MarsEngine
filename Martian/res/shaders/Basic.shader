    #shader vertex

    #version 400 core

    layout(location = 0) in vec4 position;
    layout(location = 1) in vec2 TexCoord;
    layout(location = 2) in float index;

    uniform mat4 u_MVP;

    out vec2 v_TexCoord;
    out float v_index;

    void main()
    {
      gl_Position = u_MVP * position;
      v_TexCoord = TexCoord;
      v_index = index;
    }


    #shader fragment

    #version 400 core

    in vec2 v_TexCoord; 
    in float v_index;
    layout(location = 0) out vec4 color;

    void main()
    {
      color = vec4(1.0, 1.0, 1.0, 1.0);
    }
