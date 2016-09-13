//
//  REVuModel.h
//
//  Created by DrNeuroSurg on 22.01.13.
//
//

#ifndef _REVUModel__
#define _REVUModel__

#include <iostream>
#include "Vector.h"

#include <vector>


#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"
#include "aiMesh.h"

#include "MeshHelper.h"
#include "Material.h"

#include <OpenGLES/ES2/gl.h>


#include <QCAR/Renderer.h>


class REVuModel{
    
public:
    ~REVuModel();
    REVuModel();
    REVuModel(const char * dataPath, const char * modelName);
    
    bool isValid;
    
    //EVERY TRANSFORMATION (scale, rotation, translation) WILL HAPPEN IN MODEL-SPACE !
    void ScaleTo(float x, float y, float z);        //ABSOLUTE
    void ScaleBy(float x, float y, float z);        //REALTIVE
    void ScaleUniformTo(float scale);               //ABSOLUTE
    void ScaleUniformBy(float scale);               //REALTIVE
    
    void TranslateTo(float x, float y, float z);    //ABSOLUTE
    void TranslateBy(float x, float y, float z);    //REALTIVE
    
    void RotateTo(float x, float y, float z);       //ABSOLUTE
    void RotateBy(float x, float y, float z);       //REALTIVE
    
    void LightDirection(float x, float y, float z);
    
    void Alpha(float a);                            //ALPHA FOR THE WHOLE MODEL
    void SwitchLight(bool OnOff);
    void SetMaterial(Material material);
    void SetTexture(const char * dataPath, const char * fileName);
    
    void Draw(QCAR::Matrix44F modelViewMatrix, QCAR::Matrix44F projectionMatrix);
    
    std::vector <MeshHelper> modelMeshes;
    
private:
    const aiScene* scene;
    aiString  rootPath;
    
    vec3 _scale;
    vec3 _position;
    vec3 _rotation;
    vec3 _lightDirection;
    
    int _lightOnOff;
    float _alpha;
    
    Material _material;
    bool _customMaterial;
    bool _customTexture;
    GLuint customTextureIndex;
    
    GLuint shaderProgramID;
    GLint vertexHandle;
    GLint normalHandle;
    GLint textureCoordHandle;
    GLint mvpMatrixHandle;
    GLint mvMatrixHandle;
    GLint texSampler2DHandle;
    
    GLint texCountHandle;
    GLint lightOnHandle;
    
    GLuint _LightDirection;
    GLint _AmbientMaterial;
    GLint _SpecularMaterial;
    GLint _Shininess;
    GLint _DiffuseMaterial;
    GLint _EmissiveMaterial;
    GLint _AlphaMaterial;
    
    bool initShaders();
    void loadGLResources();
    double normalizedScale; 
    void getBoundingBoxWithMinVector(struct aiVector3D* min, struct aiVector3D* max);
    void getBoundingBoxForNode(const struct aiNode* nd,  struct aiVector3D* min, struct aiVector3D* max, struct aiMatrix4x4* trafo);
    
};
#endif 
