package com.graphics.sg.opengles_tiger;

import android.content.Context;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;

import java.text.DecimalFormat;

import javax.microedition.khronos.opengles.GL10;

public class GLES30Renderer implements GLSurfaceView.Renderer{

    private Context mContext;

    private Axis mAxis;
    private Floor mFloor;
    private Tiger mTiger;
    private box mBox;
    private Cow mCow;
    public float fovy = 45.0f;
    public float ratio = 1.0f;

    // Animation frame index
    final static int N_TIGER_FRAMES = 12;
    int cur_frame_tiger = 0;
    float rotation_angle_tiger = 0;
    float tim;

    public float[] mMVPMatrix = new float[16];
    public float[] mProjectionMatrix = new float[16];
    public float[] mModelViewMatrix = new float[16];
    public float[] mModelMatrix = new float[16];
    public float[] mViewMatrix = new float[16];
    public float[] mModelViewInvTrans = new float[16];

    // Texture
    final static int TEXTURE_ID_FLOOR = 0;
    final static int TEXTURE_ID_TIGER = 1;

    // OpenGL Handles
    private PhongShadingProgram mPhongShadingProgram;
    private SimpleProgram mSimpleProgram;

    public GLES30Renderer(Context context) {
        mContext = context;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, javax.microedition.khronos.egl.EGLConfig config) {
        GLES30.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        GLES30.glEnable(GLES30.GL_DEPTH_TEST);

        // matrix, offset, eye xyz, center xyz, top xyz.
        // View Matrix 설정. 고정 된 카메라이기 때문에 한 번만 호출한다.
        Matrix.setLookAtM(mViewMatrix, 0, 300f, 300f, 300f, 0f, 0f, 0f, 0f, 1.0f, 0f);

        // GL Program을 생성.
        // phong shading을 수행하는 프로그램.
        mPhongShadingProgram = new PhongShadingProgram(
                AssetReader.readFromFile("phong.vert", mContext),
                AssetReader.readFromFile("phong.frag", mContext) );
        mPhongShadingProgram.prepare();
        mPhongShadingProgram.initLightsAndMaterial();
        mPhongShadingProgram.initFlags();
        mPhongShadingProgram.set_up_scene_lights(mViewMatrix);

        // 축을 그릴 때 사용하는 프로그램.
        mSimpleProgram = new SimpleProgram(
                AssetReader.readFromFile("simple.vert", mContext),
                AssetReader.readFromFile("simple.frag", mContext) );
        mSimpleProgram.prepare();

        // 화면에 그릴 오브젝트들을 생성.
        // 축 오브젝트 로드.
        mAxis = new Axis();
        mAxis.prepare();

        mBox=new box();
        mBox.prepare();
        // 바닥 오브젝트 로드.
        mFloor = new Floor();
        mFloor.prepare();
        mFloor.setTexture(AssetReader.getBitmapFromFile("grass_tex.jpg", mContext), TEXTURE_ID_FLOOR);


        // Tiger 오브젝트 로드.
        int nBytesPerVertex = 8 * 4;        // 3 for vertex, 3 for normal, 2 for texcoord, 4 is sizeof(float)
        int nBytesPerTriangles = nBytesPerVertex * 3;

        // 12개에 해당하는 각 오브젝트들을 모두 로드해서 Tiger 객체에 추가한다.
        mTiger = new Tiger();
        for(int i=0 ; i<N_TIGER_FRAMES ; i++) {
            DecimalFormat dfm = new DecimalFormat("00");
            String num = dfm.format(i);
            String filename = "Tiger_" + num + "_triangles_vnt.geom";
            mTiger.addGeometry(AssetReader.readGeometry(filename, nBytesPerTriangles, mContext));
        }
        mTiger.prepare();
        mTiger.setTexture(AssetReader.getBitmapFromFile("tiger_tex.jpg", mContext), TEXTURE_ID_TIGER);
        // Tiger 오브젝트 로드.
        // 12개에 해당하는 각 오브젝트들을 모두 로드해서 Tiger 객체에 추가한다.
/*        mCow = new Cow();

        String filename = "Cow_triangles_vn.geom";
        mCow.addGeometry(AssetReader.readGeometry(filename, nBytesPerTriangles, mContext));

        mCow.prepare();
*/
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        int pId;
        int timestamp = getTimeStamp();
        mPhongShadingProgram.set_up_scene_lights(mViewMatrix);
        cur_frame_tiger = timestamp % N_TIGER_FRAMES;
        rotation_angle_tiger += (Math.random()*4)-1.8;
        tim=timestamp%360-180;
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT | GLES30.GL_DEPTH_BUFFER_BIT);

        mPhongShadingProgram.set_lights1();
        mPhongShadingProgram.set_lights0();

        Matrix.perspectiveM(mProjectionMatrix, 0, fovy, ratio, 0.1f, 2000.0f);

        // Simple Program을 이용해서 축을 그린다.
        mSimpleProgram.use();
        pId = mSimpleProgram.getProgramID();

        // Axis 그리기 영역.
        Matrix.setIdentityM(mModelMatrix, 0);
        Matrix.scaleM(mModelMatrix, 0, 50f, 50f, 50f);
        Matrix.multiplyMM(mModelViewMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mModelViewMatrix, 0);

        mAxis.draw(pId, mMVPMatrix);

        // 생성한 glProgram을 이용해서 오브젝트들을 화면에 그린다.
        mPhongShadingProgram.use();

        GLES30.glUniform1i(mPhongShadingProgram.locFlagFog, mPhongShadingProgram.mFlagFog);
        GLES30.glUniform1i(mPhongShadingProgram.locFlagTextureMapping, mPhongShadingProgram.mFlagTextureMapping);
//
        Matrix.setIdentityM(mModelMatrix, 0);
        Matrix.translateM(mModelMatrix, 0, 100f, 90f-(float)Math.sin(tim)*10.0f, -100f);
        Matrix.scaleM(mModelMatrix, 0, 30f, 30f+(float)Math.sin(tim)*10.0f, 30f);

        Matrix.multiplyMM(mModelViewMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mModelViewMatrix, 0);
        Matrix.invertM(mModelViewInvTrans, 0, mModelViewMatrix, 0);
        Matrix.transposeM(mModelViewInvTrans, 0, mModelViewInvTrans, 0);

        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewProjectionMatrix, 1, false, mMVPMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrix, 1, false, mModelViewMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrixInvTrans, 1, false, mModelViewInvTrans, 0);


        mPhongShadingProgram.setUpMaterialBox();
        mBox.draw();

        GLES30.glUniform1i(mPhongShadingProgram.locFlagFog, mPhongShadingProgram.mFlagFog);
        GLES30.glUniform1i(mPhongShadingProgram.locFlagTextureMapping, mPhongShadingProgram.mFlagTextureMapping);
//
        Matrix.setIdentityM(mModelMatrix, 0);
        Matrix.translateM(mModelMatrix, 0, -100f, 70f+(float)Math.sin(tim)*10.0f, -100f);
        Matrix.scaleM(mModelMatrix, 0, 30f, 30f+(float)Math.sin(tim)*10.0f, 30f);

        Matrix.multiplyMM(mModelViewMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mModelViewMatrix, 0);
        Matrix.invertM(mModelViewInvTrans, 0, mModelViewMatrix, 0);
        Matrix.transposeM(mModelViewInvTrans, 0, mModelViewInvTrans, 0);


        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewProjectionMatrix, 1, false, mMVPMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrix, 1, false, mModelViewMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrixInvTrans, 1, false, mModelViewInvTrans, 0);


        mPhongShadingProgram.setUpMaterialBox();
        mBox.draw();

        GLES30.glUniform1i(mPhongShadingProgram.locFlagFog, mPhongShadingProgram.mFlagFog);
        GLES30.glUniform1i(mPhongShadingProgram.locFlagTextureMapping, mPhongShadingProgram.mFlagTextureMapping);
//
        Matrix.setIdentityM(mModelMatrix, 0);
        Matrix.translateM(mModelMatrix, 0, 0.0f, 100.0f, -100f);
        Matrix.scaleM(mModelMatrix, 0, 10f, 100f, 10f);

        Matrix.multiplyMM(mModelViewMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mModelViewMatrix, 0);
        Matrix.invertM(mModelViewInvTrans, 0, mModelViewMatrix, 0);
        Matrix.transposeM(mModelViewInvTrans, 0, mModelViewInvTrans, 0);


        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewProjectionMatrix, 1, false, mMVPMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrix, 1, false, mModelViewMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrixInvTrans, 1, false, mModelViewInvTrans, 0);


        mPhongShadingProgram.setUpMaterialBox();
        mBox.draw();
        // Model Matrix 설정.
        Matrix.setIdentityM(mModelMatrix, 0);
        Matrix.translateM(mModelMatrix, 0, -250f, 0f, 250f);
        Matrix.scaleM(mModelMatrix, 0, 500f, 500f, 500f);
        Matrix.rotateM(mModelMatrix, 0, -90.0f, 1f, 0f, 0f);
        Matrix.multiplyMM(mModelViewMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mModelViewMatrix, 0);
        Matrix.invertM(mModelViewInvTrans, 0, mModelViewMatrix, 0);
        Matrix.transposeM(mModelViewInvTrans, 0, mModelViewInvTrans, 0);

        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewProjectionMatrix, 1, false, mMVPMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrix, 1, false, mModelViewMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrixInvTrans, 1, false, mModelViewInvTrans, 0);

        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, mFloor.mTexId[0]);
        GLES30.glUniform1i(mPhongShadingProgram.locTexture, 1);

        mPhongShadingProgram.setUpMaterialFloor();
        mFloor.draw();

        Matrix.setIdentityM(mModelMatrix, 0);
        Matrix.translateM(mModelMatrix, 0, -25f, 25f, 25f);
        Matrix.rotateM(mModelMatrix, 0, 90.0f, 0f, 0f, 1f);
        Matrix.scaleM(mModelMatrix, 0, 250f, 50f, 25f);
        Matrix.rotateM(mModelMatrix, 0, -90.0f, 1f, 0f, 0f);
        Matrix.multiplyMM(mModelViewMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mModelViewMatrix, 0);
        Matrix.invertM(mModelViewInvTrans, 0, mModelViewMatrix, 0);
        Matrix.transposeM(mModelViewInvTrans, 0, mModelViewInvTrans, 0);

        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewProjectionMatrix, 1, false, mMVPMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrix, 1, false, mModelViewMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrixInvTrans, 1, false, mModelViewInvTrans, 0);



        mPhongShadingProgram.setUpMaterialFloor();
        mFloor.draw();
//

        // Tiger 그리기 영역.
        Matrix.setIdentityM(mModelMatrix, 0);
        Matrix.rotateM(mModelMatrix, 0, -rotation_angle_tiger, 0f, 1f, 0f);
        Matrix.translateM(mModelMatrix, 0, 180.0f, 0.0f, 0.0f);
        Matrix.rotateM(mModelMatrix, 0, -90.0f, 1.0f, 0.0f, 0.0f);
        Matrix.multiplyMM(mModelViewMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mModelViewMatrix, 0);
        Matrix.transposeM(mModelViewInvTrans, 0, mModelViewMatrix, 0);
        Matrix.invertM(mModelViewInvTrans, 0, mModelViewInvTrans, 0);

        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewProjectionMatrix, 1, false, mMVPMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrix, 1, false, mModelViewMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrixInvTrans, 1, false, mModelViewInvTrans, 0);

        GLES30.glUniform1i(mPhongShadingProgram.locFlagTextureMapping, 0);

        mPhongShadingProgram.setUpMaterialTiger();

        mTiger.draw(cur_frame_tiger);

        Matrix.setIdentityM(mModelMatrix, 0);
        Matrix.rotateM(mModelMatrix, 0, -rotation_angle_tiger, 0f, 1f, 0f);
        Matrix.translateM(mModelMatrix, 0, 120.0f, 30.0f, 0.0f);
        Matrix.scaleM(mModelMatrix, 0, 0.7f, 0.7f, 0.7f);
        Matrix.rotateM(mModelMatrix, 0, -90.0f, 1.0f, 0.0f, 0.0f);
        Matrix.multiplyMM(mModelViewMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mModelViewMatrix, 0);
        Matrix.transposeM(mModelViewInvTrans, 0, mModelViewMatrix, 0);
        Matrix.invertM(mModelViewInvTrans, 0, mModelViewInvTrans, 0);

        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewProjectionMatrix, 1, false, mMVPMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrix, 1, false, mModelViewMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrixInvTrans, 1, false, mModelViewInvTrans, 0);

        GLES30.glUniform1i(mPhongShadingProgram.locFlagTextureMapping, 0);

        mPhongShadingProgram.setUpMaterialTiger();

        mTiger.draw(cur_frame_tiger);

        mTiger.draw(cur_frame_tiger);

        Matrix.setIdentityM(mModelMatrix, 0);
        Matrix.rotateM(mModelMatrix, 0, -rotation_angle_tiger, 0f, 1f, 0f);
        Matrix.translateM(mModelMatrix, 0, 70.0f, 30.0f, 0.0f);
        Matrix.scaleM(mModelMatrix, 0, 0.5f, 0.5f, 0.5f);
        Matrix.rotateM(mModelMatrix, 0, -90.0f, 1.0f, 0.0f, 0.0f);
        Matrix.multiplyMM(mModelViewMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mModelViewMatrix, 0);
        Matrix.transposeM(mModelViewInvTrans, 0, mModelViewMatrix, 0);
        Matrix.invertM(mModelViewInvTrans, 0, mModelViewInvTrans, 0);

        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewProjectionMatrix, 1, false, mMVPMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrix, 1, false, mModelViewMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrixInvTrans, 1, false, mModelViewInvTrans, 0);

        GLES30.glUniform1i(mPhongShadingProgram.locFlagTextureMapping, 0);

        mPhongShadingProgram.setUpMaterialTiger();

        mTiger.draw(cur_frame_tiger);

        /*
        // Cow 그리기 영역.
        Matrix.setIdentityM(mModelMatrix, 0);
        Matrix.rotateM(mModelMatrix, 0, tim, 0f, 1f, 0f);
        Matrix.translateM(mModelMatrix, 0, 100.0f, 0.0f, 0.0f);
        Matrix.rotateM(mModelMatrix, 0, -90.0f, 1.0f, 0.0f, 0.0f);
        Matrix.multiplyMM(mModelViewMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mModelViewMatrix, 0);
        Matrix.transposeM(mModelViewInvTrans, 0, mModelViewMatrix, 0);
        Matrix.invertM(mModelViewInvTrans, 0, mModelViewInvTrans, 0);

        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewProjectionMatrix, 1, false, mMVPMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrix, 1, false, mModelViewMatrix, 0);
        GLES30.glUniformMatrix4fv(mPhongShadingProgram.locModelViewMatrixInvTrans, 1, false, mModelViewInvTrans, 0);

        mPhongShadingProgram.setUpMaterialCow();
        mCow.draw(cur_frame_tiger);
        */
        mSimpleProgram.use();
        pId = mSimpleProgram.getProgramID();

        // Axis 그리기 영역.
        Matrix.scaleM(mMVPMatrix, 0, 20f, 20f, 20f);
        mAxis.draw(pId, mMVPMatrix);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES30.glViewport(0, 0, width, height);

        ratio = (float) width / height;

        Matrix.perspectiveM(mProjectionMatrix, 0, fovy, ratio, 0.1f, 2000.0f);
    }

    static int prevTimeStamp = 0;
    static int currTimeStamp = 0;
    static int totalTimeStamp = 0;

    private int getTimeStamp() {
        Long tsLong = System.currentTimeMillis() / 100;

        currTimeStamp = tsLong.intValue();
        if(prevTimeStamp != 0) {
            totalTimeStamp += (currTimeStamp - prevTimeStamp);
        }
        prevTimeStamp = currTimeStamp;

        return totalTimeStamp;
    }

    // CheckBox 조작 관련 함수.
    public void toggleTexture(int type, boolean onTexture) {
        int toggleVal;
        if(onTexture) toggleVal = 1;
        else          toggleVal = 0;

        if(type == 0)           // Texture
            mPhongShadingProgram.mFlagTextureMapping = toggleVal;
        else                    // Fog
            mPhongShadingProgram.mFlagFog = toggleVal;
    }

    public void setLight1() {
        mPhongShadingProgram.light[1].light_on = 1 - mPhongShadingProgram.light[1].light_on;
    }
    public void setLight0() {
        mPhongShadingProgram.light[0].light_on = 1 - mPhongShadingProgram.light[0].light_on;
    }
}
