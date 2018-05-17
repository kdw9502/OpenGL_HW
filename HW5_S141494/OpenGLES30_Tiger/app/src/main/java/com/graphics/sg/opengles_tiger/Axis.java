package com.graphics.sg.opengles_tiger;

import android.opengl.GLES30;

public class Axis extends Object {
    /**
     * 하나의 정점 데이터를 사용해서 여러개의 glDrawArray()를 이용하는 방식으로 물체를 그리는 오브젝트.
     */
    float mVertices[] = {
            // X 축.
            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            // Y 축.
            0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            // Z 축.
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f
    };
    float mColors[][] = {
            { 1.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f }
    };

    public void prepare(){
        GLES30.glGenBuffers(1, mVBO, 0);

        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mVBO[0]);
        GLES30.glBufferData(
                GLES30.GL_ARRAY_BUFFER,
                mVertices.length * BufferConverter.SIZE_OF_FLOAT,
                BufferConverter.floatArrayToBuffer(mVertices),
                GLES30.GL_STATIC_DRAW);
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, 0);

        GLES30.glGenVertexArrays(1, mVAO, 0);

        GLES30.glBindVertexArray(mVAO[0]);
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mVBO[0]);
        GLES30.glVertexAttribPointer(0, 3, GLES30.GL_FLOAT, false, 0, 0);
        GLES30.glEnableVertexAttribArray(0);
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, 0);
        GLES30.glBindVertexArray(0);
    }

    public void draw(int pId, float[] mvpMatrix) {
        int loc_mvp_matrix = GLES30.glGetUniformLocation(pId, "ModelViewProjectionMatrix");
        GLES30.glUniformMatrix4fv(loc_mvp_matrix, 1, false, mvpMatrix, 0);

        int loc_primitive_color = GLES30.glGetUniformLocation(pId, "primitive_color");

        GLES30.glLineWidth(3.0f);

        GLES30.glBindVertexArray(mVAO[0]);
        GLES30.glUniform3fv(loc_primitive_color, 1, BufferConverter.floatArrayToBuffer(mColors[0]));
        GLES30.glDrawArrays(GLES30.GL_LINES, 0, 2);
        GLES30.glUniform3fv(loc_primitive_color, 1, BufferConverter.floatArrayToBuffer(mColors[1]));
        GLES30.glDrawArrays(GLES30.GL_LINES, 2, 2);
        GLES30.glUniform3fv(loc_primitive_color, 1, BufferConverter.floatArrayToBuffer(mColors[2]));
        GLES30.glDrawArrays(GLES30.GL_LINES, 4, 2);
        GLES30.glBindVertexArray(0);

        GLES30.glLineWidth(1.0f);
    }
}
