package com.graphics.sg.opengles_tiger;

import android.opengl.GLES30;

public class Tiger extends Object {
    /**
     * 하나의 정점 데이터 내에 좌표, 법선, 텍스쳐좌표 데이터가 모두 들어있어서 이를 바탕으로 물체를 그리는 오브젝트.
     */
    final static int N_TIGER_FRAMES = 12;

    float vertices[][] = new float[N_TIGER_FRAMES][];
    int n_triangles[] = new int[N_TIGER_FRAMES];
    int vertex_offset[] = new int[N_TIGER_FRAMES];

    int current_index = 0;

    public void addGeometry(float[] geom_vertices) {
        int idx = current_index;

        vertices[idx] = geom_vertices;
        n_triangles[idx] = vertices[idx].length / ( 8 * 3 ); // (vert(3) + norm(3) + tex(2)) * 3(삼각형)

        if(idx == 0) {
            vertex_offset[0] = 0;
        }
        else {
            vertex_offset[idx] = vertex_offset[idx - 1] + 3 * n_triangles[idx - 1];
        }

        current_index++;
    }

    public void prepare() {
        int n_bytes_per_vertex, n_bytes_per_triangels, n_total_triangles = 0;

        n_bytes_per_vertex = 8 * 4;
        n_bytes_per_triangels = 3 * n_bytes_per_vertex;

        for(int i=0 ; i<N_TIGER_FRAMES ; i++){
            n_total_triangles += n_triangles[i];
        }

        GLES30.glGenBuffers(1, mVBO, 0);
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mVBO[0]);
        GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER, n_total_triangles * n_bytes_per_triangels, null, GLES30.GL_STATIC_DRAW);
        for(int i=0 ; i<N_TIGER_FRAMES ; i++) {
            GLES30.glBufferSubData(
                    GLES30.GL_ARRAY_BUFFER,
                    vertex_offset[i] * n_bytes_per_vertex,
                    n_triangles[i] * n_bytes_per_triangels,
                    BufferConverter.floatArrayToBuffer(vertices[i]));
        }

        for(int i=0 ; i<N_TIGER_FRAMES ; i++) {
            // graphic memory에 정점 데이터가 복사 됨. 기존의 데이터는 삭제한다.
            vertices[i] = null;
        }

        GLES30.glGenVertexArrays(1, mVAO, 0);
        GLES30.glBindVertexArray(mVAO[0]);
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mVBO[0]);
        GLES30.glVertexAttribPointer(0, 3, GLES30.GL_FLOAT, false, n_bytes_per_vertex, 0);
        GLES30.glEnableVertexAttribArray(0);
        GLES30.glVertexAttribPointer(1, 3, GLES30.GL_FLOAT, false, n_bytes_per_vertex, 3 * 4);
        GLES30.glEnableVertexAttribArray(1);
        GLES30.glVertexAttribPointer(2, 2, GLES30.GL_FLOAT, false, n_bytes_per_vertex, 6 * 4);
        GLES30.glEnableVertexAttribArray(2);

        GLES30.glBindVertexArray(0);
    }

    public void draw(int frame) {
        GLES30.glBindVertexArray(mVAO[0]);
        GLES30.glDrawArrays(GLES30.GL_TRIANGLES, vertex_offset[frame], 3 * n_triangles[frame]);
        GLES30.glBindVertexArray(0);
    }
}
