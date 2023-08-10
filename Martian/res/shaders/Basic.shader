    #shader vertex

    #version 400 core

    layout(location = 0) in vec2 position;
    layout(location = 1) in vec2 TexCoord;
    layout(location = 2) in float index;

    uniform mat4 u_MVP;

    out vec2 v_TexCoord;
    out float v_index;

    void main()
    {
        vec4 pos = vec4(position, 1, 1);
        gl_Position =  u_MVP * pos;
        v_TexCoord = TexCoord;
        v_index = index;
    }


    #shader fragment

    #version 400 core


    in vec2 v_TexCoord; 
    in float v_index;
    layout(location = 0) out vec4 color;

    uniform sampler2D u_Texture;

    void main()
    {
        if (v_index == 1)
            color = vec4(0.23, 0.293, 0.293, 1.0);
        else if (v_index == 0)
            color = texture(u_Texture, v_TexCoord);
        else
            color = vec4(1.0, 1.0, 1.0, 1.0);

    }
