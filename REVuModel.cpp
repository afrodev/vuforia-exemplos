//
//  REVuModel.cpp
//  Created by DrNeurosurg on 22.01.13.
//
//

#include "REVuModel.h"
#include "NFreeImage.h"
#include "MeshHelper.h"
#include "Material.h"


//FROM ASSIMP
#include "aiConfig.h"
#include "assimp.hpp"
#include "aiScene.h"
#include "aiPostProcess.h"
#include "aiMesh.h"
#include "aiVector3D.h"
#include "aiVersion.h"


//FROM QCAR
#include "ShaderUtils.h"


#define MAKESTRING(x) #x
#import "Shaders/SimpleLightning.fsh"
#import "Shaders/SimpleLightning.vsh"

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)


#define kInitalRoation 0.0

REVuModel::REVuModel()
{
    
	scene = NULL;
    
    //DEFAULTS
    _scale = vec3(1.0 , 1.0, 1.0);
    _position = vec3(0.0, 0.0, 0.0);
    _rotation = vec3(0.0, 0.0, 0.0);
    _lightDirection = vec3(0.0, 0.0, -1.0);
    _lightOnOff = 1;
    _alpha =1.0;
    _customTexture = false;
    _customMaterial = false;
    
}


void REVuModel::getBoundingBoxForNode(const struct aiNode* nd,  struct aiVector3D* min, struct aiVector3D* max, struct aiMatrix4x4* trafo)
{
    struct aiMatrix4x4 prev;
	unsigned int n = 0, t;
    
	prev = *trafo;
	aiMultiplyMatrix4(trafo,&nd->mTransformation);
    
	for (; n < nd->mNumMeshes; ++n){
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t){
        	struct aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp,trafo);
            
            
			min->x = aisgl_min(min->x,tmp.x);
			min->y = aisgl_min(min->y,tmp.y);
			min->z = aisgl_min(min->z,tmp.z);
            
			max->x = aisgl_max(max->x,tmp.x);
			max->y = aisgl_max(max->y,tmp.y);
			max->z = aisgl_max(max->z,tmp.z);
		}
	}
    
	for (n = 0; n < nd->mNumChildren; ++n){
		this->getBoundingBoxForNode(nd->mChildren[n], min, max, trafo);
	}
    
	*trafo = prev;
}

void REVuModel::getBoundingBoxWithMinVector(struct aiVector3D* min, struct aiVector3D* max)
{
	struct aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);
    
	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;
    
    getBoundingBoxForNode(scene->mRootNode, min, max, &trafo);
}

REVuModel::REVuModel(const char *dataPath, const char * modelName)
{

    rootPath.Set(dataPath);
    rootPath.Append("/");
    aiString  modelPath = rootPath;
    modelPath.Append("/");
    modelPath.Append(modelName);
    
    scene = NULL;
    isValid = false;
    
    //DEFAULTS
    _scale = vec3(1.0 , 1.0, 1.0);
    _position = vec3(0.0, 0.0, 0.0);
    _rotation = vec3(kInitalRoation, 0.0, 0.0);
    _lightDirection = vec3(0.0, 0., -1.0); //FRONT & ABOVE
    _alpha =1.0;
    _customTexture = false;
    _customMaterial = false;

    Assimp::Importer* importer =  new Assimp::Importer();
    
    printf("\nASSIMP Version %i.%i.%i\n", aiGetVersionMajor(), aiGetVersionMinor(),aiGetVersionRevision());
    printf("FREEIMAGE Version %s \n\n", FreeImage_GetVersion());
    
    scene = importer->ReadFile(modelPath.data,aiProcessPreset_TargetRealtime_MaxQuality);    
    if(!scene || !initShaders())
        isValid = false;
    else
    {
        aiVector3D scene_min, scene_max;
        
        getBoundingBoxWithMinVector(&scene_min, &scene_max);
        
        normalizedScale = scene_max.x-scene_min.x;
        normalizedScale = aisgl_max(scene_max.y - scene_min.y,normalizedScale);
        normalizedScale = aisgl_max(scene_max.z - scene_min.z,normalizedScale);
        normalizedScale = 1.f / normalizedScale * 255; //????
        
        _scale = vec3(normalizedScale , normalizedScale, normalizedScale);
        loadGLResources();
        isValid = true;
    }
}



bool REVuModel::initShaders()
{
    // OpenGL 2 initialisation
    shaderProgramID = ShaderUtils::createProgramFromBuffer(SimpleLightningVertexShader , SimpleLightningFragmentShader);
    
    if (0 < shaderProgramID)
    {
        vertexHandle = glGetAttribLocation(shaderProgramID, "vertexPosition");
        normalHandle = glGetAttribLocation(shaderProgramID, "vertexNormal");
        textureCoordHandle = glGetAttribLocation(shaderProgramID, "vertexTexCoord");
        mvpMatrixHandle = glGetUniformLocation(shaderProgramID, "modelViewProjectionMatrix");
        mvMatrixHandle = glGetUniformLocation(shaderProgramID, "modelViewMatrix");
        texSampler2DHandle  = glGetUniformLocation(shaderProgramID,"texSampler2D");
        
        _LightDirection = glGetUniformLocation(shaderProgramID,"LightDirection");
        _AmbientMaterial = glGetUniformLocation(shaderProgramID,"AmbientMaterial");
        _SpecularMaterial = glGetUniformLocation(shaderProgramID,"SpecularMaterial");
        _DiffuseMaterial = glGetUniformLocation(shaderProgramID,"DiffuseMaterial"); //UNUSED
        _EmissiveMaterial = glGetUniformLocation(shaderProgramID,"EmissiveMaterial");
        _Shininess  = glGetUniformLocation(shaderProgramID,"Shininess");
        _AlphaMaterial = glGetUniformLocation(shaderProgramID, "Alpha");
        texCountHandle = glGetUniformLocation(shaderProgramID, "texCount");
        lightOnHandle = glGetUniformLocation(shaderProgramID, "LightOn");
        return true;
    }
    else {
        return false;
    }
}

void REVuModel::Alpha(float a)
{
    _alpha = a;
}

void REVuModel::ScaleTo(float x, float y, float z)
{
    _scale = vec3(normalizedScale * x, normalizedScale * y, normalizedScale * z);
}

void REVuModel::ScaleBy(float x, float y, float z)
{
    _scale = vec3(_scale.x + x, _scale.y + y, _scale.z + z);
}

void REVuModel::ScaleUniformTo(float scale)
{
    _scale = vec3(normalizedScale * scale, normalizedScale * scale, normalizedScale * scale);
}

void REVuModel::ScaleUniformBy(float scale)
{
    _scale = vec3(_scale.x + scale, _scale.y + scale, _scale.z + scale);
}

void REVuModel::TranslateTo(float x, float y, float z)
{
    _position = vec3(x, y, z);
}

void REVuModel::TranslateBy(float x, float y, float z)
{
    _position = vec3(_position.x + x, _position.y + y, _position.z + z);
}

void REVuModel::RotateTo(float x, float y, float z)
{
    _rotation = vec3( x, y, z);
}

void REVuModel::RotateBy(float x, float y, float z)
{
    _rotation = vec3( _rotation.x + x, _rotation.y + y, _rotation.z + z);
}

void REVuModel::LightDirection(float x, float y, float z)
{
    _lightDirection = vec3(x, y, z);
}


void REVuModel::SwitchLight(bool OnOff)
{
    if(OnOff)
        _lightOnOff =1;
    else
        _lightOnOff =0;
}

void REVuModel::SetMaterial(Material material)
{
    _material = material;
    _customMaterial = true;
    
}

void REVuModel::SetTexture(const char * dataPath, const char * fileName)
{
    _customTexture = false;
    
    rootPath.Set(dataPath);
    rootPath.Append("/");
    aiString  filePath = rootPath;
    filePath.Append("/");
    filePath.Append(fileName);
    
    NFreeImage texImage;
    
    if(texImage.Load(filePath.data))
    {
        
        glGenTextures( 1, &customTextureIndex );
        glBindTexture(GL_TEXTURE_2D, customTextureIndex);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texImage.Width(), texImage.Height(),
                     0, GL_RGBA, GL_UNSIGNED_BYTE,(GLvoid*) texImage.Bits() );
        
        _customTexture = true;

        printf("Texture %s successfull loaded !\n",fileName);
    }

}

void REVuModel::loadGLResources()
{
    modelMeshes.reserve(scene->mNumMeshes);
    
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        // current mesh we are introspecting
        aiMesh* mesh = scene->mMeshes[i];
        
        // the current meshHelper we will be populating data into.
        MeshHelper meshHelper;
        
        meshHelper.mesh = mesh;
        
        /*
         Ns 64
         Kd 0.700 0.700 0.700
         Ks 1.000 1.000 1.000
         Ka 0.700 0.700 0.700
         */
        // set the mesh's default values.
        aiColor4D dcolor = aiColor4D(0.7f, 0.7f, 0.7f, 1.0f);
        meshHelper.diffuseColor = dcolor;
        
        aiColor4D scolor = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
        meshHelper.specularColor = scolor;
        
        aiColor4D acolor = aiColor4D(0.7f, 0.7f, 0.7f, 1.0f);
        meshHelper.ambientColor = acolor;
        
        aiColor4D ecolor = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);
        meshHelper.emissiveColor = ecolor;
        

        // Handle material info
        aiMaterial* mtl = scene->mMaterials[mesh->mMaterialIndex];
        
        // Load Textures
        aiString texName;
        aiString texPath = rootPath;
        meshHelper.hasTexture = false;
        if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texName))
        {
            meshHelper.diffuseColor = aiColor4D(0.8f, 0.8f, 0.8f,  1.0f);
            texPath.Append(texName.data);
            printf("Loading Texture %s\n",texPath.data);
            NFreeImage texImage;
            
            if(texImage.Load(texPath.data))
            {
                
                glGenTextures( 1, &meshHelper.textureIndex );
                glBindTexture(GL_TEXTURE_2D, meshHelper.textureIndex);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texImage.Width(), texImage.Height(),
                             0, GL_RGBA, GL_UNSIGNED_BYTE,(GLvoid*) texImage.Bits() );
                
                meshHelper.hasTexture = true;
                
                printf("Texture %s successfull loaded !\n",texPath.data);
            }
            else
                printf("ERROR loading Texture %s \n",texPath.data);
        }
        
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &dcolor))
            meshHelper.diffuseColor = dcolor;
        
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &scolor))
            meshHelper.specularColor = scolor;
        
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &acolor))
            meshHelper.ambientColor = acolor;
        
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &ecolor))
            meshHelper.emissiveColor = ecolor;
        
        float value;
        if (AI_SUCCESS == mtl->Get(AI_MATKEY_SHININESS, value))
        {
            meshHelper.shininess = value;
        } else
        {
            meshHelper.shininess = 64;
        }
        
        
        //ALPHABLEND
        int blendMode;
		if(AI_SUCCESS == aiGetMaterialInteger(mtl, AI_MATKEY_BLEND_FUNC, &blendMode))
        {
			if(blendMode==aiBlendMode_Default)
            {
                //TODO:
			}
		}
        
        // Culling
        unsigned int max = 1;
        int two_sided;
        if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
            meshHelper.twoSided = true;
        else
            meshHelper.twoSided = false;

        //########## END MATERIALS
        
        meshHelper.indexBuffer.reserve(mesh->mNumFaces * 3);
        for (unsigned j = 0; j < mesh->mNumFaces; j++)
        {
            for (unsigned k = 0; k < mesh->mFaces[j].mNumIndices; k++)
            {
                meshHelper.indexBuffer.push_back(mesh->mFaces[j].mIndices[k]);
            }
        }
        meshHelper.numIndices = meshHelper.indexBuffer.size();
        
        modelMeshes.push_back(meshHelper);
    }
    
}



void REVuModel::Draw(QCAR::Matrix44F modelViewMatrix, QCAR::Matrix44F projectionMatrix)
{
    if(!scene)  //NOTHING TO DO ....
        return;
    
    QCAR::Matrix44F modelViewProjection;
    
    ShaderUtils::translatePoseMatrix(_position.x, _position.y, _position.z, &modelViewMatrix.data[0]);
    ShaderUtils::scalePoseMatrix( _scale.x,  _scale.y,  _scale.z, &modelViewMatrix.data[0]);
    
    ShaderUtils::rotatePoseMatrix(_rotation.x, 1.0, 0.0, 0.0, &modelViewMatrix.data[0]);
    ShaderUtils::rotatePoseMatrix(_rotation.y, 0.0, 1.0, 0.0, &modelViewMatrix.data[0]);
    ShaderUtils::rotatePoseMatrix(_rotation.z, 0.0, 0.0, 1.0, &modelViewMatrix.data[0]);
    
    ShaderUtils::multiplyMatrix(&projectionMatrix.data[0], &modelViewMatrix.data[0], &modelViewProjection.data[0]);
    
    glUseProgram(shaderProgramID);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE, (const GLfloat*)&modelViewProjection.data[0]);
    glUniformMatrix4fv(mvMatrixHandle, 1, GL_FALSE, (const GLfloat*)&modelViewMatrix.data[0]);
    
    //############################# THE MESHES ####################################################
    
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        
        // current mesh we are introspecting     
        MeshHelper meshhelper =  modelMeshes.at(i);
        
        if (meshhelper.mesh->mPrimitiveTypes != GL_TRIANGLES)
            continue;
        
        // VERTICES
        glVertexAttribPointer(vertexHandle, 3, GL_FLOAT, 0, sizeof(aiVector3D), meshhelper.mesh->mVertices );
        if (meshhelper.hasTexture)
            glVertexAttribPointer(textureCoordHandle, 2, GL_FLOAT, 0, sizeof(aiVector3D), meshhelper.mesh->mTextureCoords[0]);
        
        glVertexAttribPointer(normalHandle, 3, GL_FLOAT, 0, sizeof(aiVector3D), meshhelper.mesh->mNormals);
        
        glEnableVertexAttribArray(vertexHandle);
        glEnableVertexAttribArray(normalHandle);
        glEnableVertexAttribArray(textureCoordHandle);
        
        //WE HAVE TO DISABLE IT IN THE SHADER
        glUniform1i(texCountHandle, 0);
        
        //TEXTURE
        if (meshhelper.hasTexture)
        {
            glUniform1i(texCountHandle, 1); //ENABLE TEXTURES IN THE SHADER
            glActiveTexture(GL_TEXTURE0);
            if(!_customTexture)
                glBindTexture(GL_TEXTURE_2D, meshhelper.textureIndex);
            else
                glBindTexture(GL_TEXTURE_2D, customTextureIndex);
        }
        else
        {
            //MATERIAL PROPERTIES
            
            //Diffuse material
            if(!_customMaterial)
                  glUniform3f(_DiffuseMaterial, meshhelper.diffuseColor.r, meshhelper.diffuseColor.g, meshhelper.diffuseColor.b);
            else
                glUniform3fv(_DiffuseMaterial, 1,_material.Diffuse());

            
            // Specular material
            if(!_customMaterial)
                glUniform3f(_SpecularMaterial, meshhelper.specularColor.r, meshhelper.specularColor.g, meshhelper.specularColor.b);
            else
                glUniform3fv(_SpecularMaterial, 1,_material.Specular());
            
            // Ambient material
            if(!_customMaterial)
                glUniform3f(_AmbientMaterial, meshhelper.ambientColor.r, meshhelper.ambientColor.g, meshhelper.ambientColor.b);
            else
                glUniform3fv(_AmbientMaterial, 1,_material.Ambient());
            
            // Emissive material
            if(!_customMaterial)
                glUniform3f(_EmissiveMaterial, meshhelper.emissiveColor.r, meshhelper.emissiveColor.g, meshhelper.emissiveColor.b);
            else
                glUniform3fv(_EmissiveMaterial, 1,_material.Emissive());
            
            // Shininess
            if(!_customMaterial)
                glUniform1f(_Shininess, meshhelper.shininess);
            else
                glUniform1f(_Shininess, _material.Shininess());
        }
       
        
        //ALPHA
        glUniform1f(_AlphaMaterial, _alpha);
        
        //LIGHT
        glUniform3fv(_LightDirection, 1, _lightDirection.Pointer());
        glUniform1i(lightOnHandle, _lightOnOff);
        
        //ROCK'N ROLL
        glDrawElements(meshhelper.mesh->mPrimitiveTypes,  meshhelper.indexBuffer.size(), GL_UNSIGNED_INT, &meshhelper.indexBuffer[0]);
        
        ShaderUtils::checkGlError("REVuModel DRAW");
        
        glDisableVertexAttribArray(vertexHandle);
        glDisableVertexAttribArray(normalHandle);
        glDisableVertexAttribArray(textureCoordHandle);
        
        
        //###########################################################################################
    }
        
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    
}

