package com.graphics.sg.opengles_tiger;

import android.opengl.GLES30;

public class Floor extends Object {
    /**
     * 여러개의 Vertex Buffer Object를 이용해서 하나의 물체를 그리는 방식의 오브젝트.
     */
    float mVertices[] = {
            0.0f, 0.0f, 0.0f,   // top left
            1.0f, 0.0f, 0.0f,   // bottom left
            1.0f, 1.0f, 0.0f,   // bottom right
            0.0f, 1.0f, 0.0f }; // top right
    float mNormals[] = {
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f };
    float mTexCoords[] = {
            0.0f, 0.0f,
            4.0f, 0.0f,
            4.0f, 4.0f,
            0.0f, 4.0f };
    short mIndices[] = { 0, 1, 2, 0, 2, 3 };

    public void prepare() {
        GLES30.glGenBuffers(4, mVBO, 0);

        // 정점의 좌표 데이터를 glBuffer로 생성.
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mVBO[0]);
        GLES30.glBufferData(
                GLES30.GL_ARRAY_BUFFER,
                mVertices.length * BufferConverter.SIZE_OF_FLOAT,
                BufferConverter.floatArrayToBuffer(mVertices),
                GLES30.GL_STATIC_DRAW);

        // 정점의 법선 데이터를 glBuffer로 생성.
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mVBO[1]);
        GLES30.glBufferData(
                GLES30.GL_ARRAY_BUFFER,
                mNormals.length * BufferConverter.SIZE_OF_FLOAT,
                BufferConverter.floatArrayToBuffer(mNormals),
                GLES30.GL_STATIC_DRAW);

        // 정점의 텍스쳐 좌표값을 glBuffer로 생성.
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mVBO[2]);
        GLES30.glBufferData(
                GLES30.GL_ARRAY_BUFFER,
                mTexCoords.length * BufferConverter.SIZE_OF_FLOAT,
                BufferConverter.floatArrayToBuffer(mTexCoords),
                GLES30.GL_STATIC_DRAW);

        // 정점의 출력 인덱스 데이터를 glBuffer로 생성.
        GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, mVBO[3]);
        GLES30.glBufferData(
                GLES30.GL_ELEMENT_ARRAY_BUFFER,
                mIndices.length * BufferConverter.SIZE_OF_SHORT,
                BufferConverter.shortArrayToBuffer(mIndices),
                GLES30.GL_STATIC_DRAW);

        // VAO ID 생성.
        GLES30.glGenVertexArrays(1, mVAO, 0);
        GLES30.glBindVertexArray(mVAO[0]);

        // OpenGL context의 GL_ARRAY_BUFFER가 mVBO[0]에 해당하는 버퍼를 가리키도록 바인딩.
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mVBO[0]);
        // Vertex Attribute 중 하나를 활성화.
        GLES30.glEnableVertexAttribArray(VERTEX_POS_INDEX);
        // 활성화 된 Vertex Attribute에 GL_ARRAY_BUFFER에 있는 데이터를 전송.
        GLES30.glVertexAttribPointer(VERTEX_POS_INDEX, VERTEX_POS_SIZE, GLES30.GL_FLOAT, false, VERTEX_POS_SIZE * BufferConverter.SIZE_OF_FLOAT, 0);

        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mVBO[1]);
        GLES30.glEnableVertexAttribArray(VERTEX_NORM_INDEX);
        GLES30.glVertexAttribPointer(VERTEX_NORM_INDEX, VERTEX_NORM_SIZE, GLES30.GL_FLOAT, false, VERTEX_NORM_SIZE * BufferConverter.SIZE_OF_FLOAT, 0);

        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mVBO[2]);
        GLES30.glEnableVertexAttribArray(VERTEX_TEX_INDEX);
        GLES30.glVertexAttribPointer(VERTEX_TEX_INDEX, VERTEX_TEX_SIZE, GLES30.GL_FLOAT, false, VERTEX_TEX_SIZE * BufferConverter.SIZE_OF_FLOAT, 0);

        GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, mVBO[3]);

        // 실제로 사용하기 전에는 일단 기본 VAO로 설정해놓는다.
        GLES30.glBindVertexArray(0);
    }

    void draw() {
        GLES30.glBindVertexArray(mVAO[0]);
        GLES30.glDrawElements(GLES30.GL_TRIANGLES, mIndices.length, GLES30.GL_UNSIGNED_SHORT, 0);
        GLES30.glBindVertexArray(0);
    }
}
