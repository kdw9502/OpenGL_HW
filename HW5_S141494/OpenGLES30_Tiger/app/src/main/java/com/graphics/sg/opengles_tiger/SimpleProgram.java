package com.graphics.sg.opengles_tiger;

import android.opengl.GLES30;

public class SimpleProgram extends GLES30Program {

    // 각종 unifrom location들을 설정.
    int locPrimitiveColor;
    int locModelViewProjectionMatrix;

    public SimpleProgram(String vertexShaderCode, String fragmentShaderCode) {
        super(vertexShaderCode, fragmentShaderCode);
    }

    public void prepare() {
        locPrimitiveColor = GLES30.glGetUniformLocation(mId, "ModelViewProjectionMatrix");;
        locModelViewProjectionMatrix = GLES30.glGetUniformLocation(mId, "primitive_color");
    }
}
