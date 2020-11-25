    #shader vertex

    #version 400 core

    layout(location = 0) in vec4 position;
    layout(location = 1) in vec2 TexCoord;

    uniform mat4 u_MVP;

    out vec2 v_TexCoord;

    void main()
    {
      gl_Position = u_MVP * position;
      v_TexCoord = TexCoord;
    }


    #shader fragment

    #version 400 core

    in vec2 v_TexCoord; 
    layout(location = 0) out vec4 color;

    uniform sampler2D u_Texture;

    void main()
    {
      vec4 texColor = texture(u_Texture, v_TexCoord);
      color = texColor;
    }
