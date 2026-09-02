#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "vendor/stb_image.h"

enum RenderType {
    WIREFRAME = 1,
    SHADED = 2,
    TEXTURED = 3
};

Model::Model()
{
    
}

Model::Model(string filePath, Vec3 position, float scale)
:position{position}, scale{scale}
{

    nLights = 0;
    maxNLight = 10;
    lights = new Light*[maxNLight];
    shadowMapBuffer = new bool[100 * 100];
    std::fill(shadowMapBuffer,shadowMapBuffer + (100*100),false);
    loadModel(filePath);

}

void Model::setBackfaceCulling(bool value){
    this->backfaceCulling = value;
}

bool Model::getBackfaceCulling(){
    return this->backfaceCulling;
}

void Model::loadModel(string path)
{

    printf("\nLoading model: %s\n",path.c_str());

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs
    );

    if (!scene ||
        scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode)
    {
        std::cout << "Assimp Error: "
                  << importer.GetErrorString()
                  << std::endl;
        return;
    }

    if (scene->mNumMeshes == 0)
    {
        std::cout << "Error: The model has no meshs!"
                  << std::endl;
        return;
    }

    printf(
        "Meshs found: %d\n",
        scene->mNumMeshes
    );


    // =========================================================
    // CALCULAR QUANTIDADE TOTAL DE VERTICES E FACES
    // =========================================================

    unsigned int totalVertices = 0;
    unsigned int totalFaces = 0;

    for (unsigned int m = 0; m < scene->mNumMeshes; m++)
    {
        aiMesh* mesh = scene->mMeshes[m];

        totalVertices += mesh->mNumVertices;
        totalFaces += mesh->mNumFaces;
    }


    // =========================================================
    // ALOCAR ARRAYS
    // =========================================================

    this->nVertices = totalVertices;

    this->vertices = new Vec3[nVertices];
    this->projection = new Vec3[nVertices];
    this->screenSpaceBuffer = new bool[nVertices];
    this->pontos = new Vec3[nVertices];
    this->uvs = new Vec3[nVertices];

    this->polygonCount = totalFaces;


    // =========================================================
    // ÍNDICES
    // =========================================================

    this->indexCount = totalFaces * 3;

    this->indices = new unsigned int[indexCount];


    // =========================================================
    // CARREGAR TODAS AS MESHES
    // =========================================================

    unsigned int vertexOffset = 0;
    unsigned int indexOffset = 0;


    for (unsigned int m = 0; m < scene->mNumMeshes; m++)
    {

        aiMesh* mesh = scene->mMeshes[m];

        printf(
            "Loading mesh %d: %d vertices, %d faces\n",
            m,
            mesh->mNumVertices,
            mesh->mNumFaces
        );


        // =====================================================
        // VERTICES
        // =====================================================

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            this->vertices[vertexOffset + i] = Vec3(
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
            );


            this->projection[vertexOffset + i] =
                Vec3(0.0f, 0.0f, 0.0f);


            // UV

            if (mesh->mTextureCoords[0])
            {
                this->uvs[vertexOffset + i] = Vec3(
                    mesh->mTextureCoords[0][i].x,
                    mesh->mTextureCoords[0][i].y,
                    0.0f
                );
            }
            else
            {
                this->uvs[vertexOffset + i] =
                    Vec3(0.0f, 0.0f, 0.0f);
            }
        }


        // =====================================================
        // ÍNDICES
        // =====================================================

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            if (face.mNumIndices != 3)
                continue;


            this->indices[indexOffset++] =
                face.mIndices[0] + vertexOffset;

            this->indices[indexOffset++] =
                face.mIndices[1] + vertexOffset;

            this->indices[indexOffset++] =
                face.mIndices[2] + vertexOffset;
        }


        // Próxima mesh começa depois dos vértices atuais
        vertexOffset += mesh->mNumVertices;
    }


    // =========================================================
    // TEXTURA
    // =========================================================

    // Por enquanto pegamos a textura da primeira mesh.
    // Isso mantém o funcionamento do seu sistema atual.

    aiMesh* firstMesh = scene->mMeshes[0];

    if (scene->mNumMaterials > 0)
    {
        aiMaterial* material =
            scene->mMaterials[firstMesh->mMaterialIndex];

        aiString str;

        if (material->GetTexture(
                aiTextureType_DIFFUSE,
                0,
                &str
            ) == AI_SUCCESS)
        {
            int width;
            int height;
            int nrChannels;

            unsigned char* data = nullptr;

            const aiTexture* embeddedTexture =
                scene->GetEmbeddedTexture(str.C_Str());


            // =============================================
            // TEXTURA EMBUTIDA
            // =============================================

            if (embeddedTexture)
            {
                if (embeddedTexture->mHeight == 0)
                {
                    data = stbi_load_from_memory(
                        reinterpret_cast<unsigned char*>(
                            embeddedTexture->pcData
                        ),
                        embeddedTexture->mWidth,
                        &width,
                        &height,
                        &nrChannels,
                        3
                    );
                }
            }


            // =============================================
            // TEXTURA EXTERNA
            // =============================================

            else
            {
                string directory =
                    path.substr(
                        0,
                        path.find_last_of('/')
                    );

                string texPath =
                    directory + "/" + string(str.C_Str());

                data = stbi_load(
                    texPath.c_str(),
                    &width,
                    &height,
                    &nrChannels,
                    3
                );
            }


            if (data)
            {
                this->diffuseTexture =
                    new Texture(
                        (unsigned char*)data,
                        width,
                        height
                    );

                printf(
                    "(%dx%d) difusseTexture loaded\n",
                    width,
                    height
                );
            }
            else
            {
                printf(
                    "Error loading texture: %s\n",
                    stbi_failure_reason()
                );
            }
        }
        else
        {
            printf(
                "Warning: No texture found.\n"
            );
        }
    }

    printf(
        "Model '%s' loaded - %d meshes, %d vertices, %d faces\n",
        path.c_str(),
        scene->mNumMeshes,
        this->nVertices,
        this->polygonCount
    );

}

/**
 * @brief A partir do centro e do tamanho do sólido, calcula a posição dos pontos restantes.
 * @author Jose Mateus Amaral
 */
void Model::calcular_pontos_3D()
{

    for( int i = 0 ; i < this->nVertices ; i++ ){
        this->pontos[i] = this->vertices[i];
        this->pontos[i].x = this->position.x + this->pontos[i].x * this->scale;
        this->pontos[i].y = this->position.y + this->pontos[i].y * this->scale;
        this->pontos[i].z = this->position.z + this->pontos[i].z * this->scale;
    }

}

/**
 * @brief Rotacionar os pontos do sólido utilizando matriz de rotação
 * @authors Jose Mateus Amaral, Gustavo Mittelmann, Henrique Heiderscheidt
 * 
 * @param rotacaoX Ângulo de rotação em relação ao eixo X
 * @param rotacaoY Ângulo de rotação em relação ao eixo Y
 * @param rotacaoZ Ângulo de rotação em relação ao eixo Z
 */
void Model::rotate(int rotacaoX, int rotacaoY, int rotacaoZ){

    angulo.x += rotacaoX;
    angulo.y += rotacaoY;
    angulo.z += rotacaoZ;

    float senoX = sin( rotacaoX * M_PI / 180 );
    float cossenoX = cos( rotacaoX * M_PI / 180 );
    float senoY = sin( rotacaoY * M_PI / 180 );
    float cossenoY = cos( rotacaoY * M_PI / 180 );
    float senoZ = sin( rotacaoZ * M_PI / 180 );
    float cossenoZ = cos( rotacaoZ * M_PI / 180 );
    float x, y, z;

    for( int i = 0 ; i < nVertices ; i++ ){
        
        vertices[i].x = vertices[i].x * cossenoX - vertices[i].z * senoX;
        vertices[i].z = vertices[i].z * cossenoX + vertices[i].x * senoX;
        
        y = vertices[i].y;
        z = vertices[i].z;
        vertices[i].y = y * cossenoY - z * senoY;
        vertices[i].z = z * cossenoY + y * senoY;
        
        x = vertices[i].x;
        y = vertices[i].y;
        vertices[i].y = y * cossenoZ - x * senoZ;
        vertices[i].x = x * cossenoZ + y * senoZ;
    
    }
}

void Model::setPos(float x, float y, float z){
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
}

void Model::setPos(Vec3 posicao){
    this->position = posicao;
}

void Model::setX(float x){
    this->position.x = x;
}

void Model::setY(float y){
    this->position.y = y;
}

void Model::setZ(float z){
    this->position.z = z;
}

Vec3 Model::getPos(){
    return this->position;
}

float Model::getX(){
    return this->position.x;
}

float Model::getY(){
    return this->position.y;
}

float Model::getZ(){
    return this->position.z;
}

void Model::setScale(float tamanho){
    this->scale = tamanho;
}

float Model::getScale(){
    return this->scale;
}

void Model::setLight(Light *light){
    light->attachModel(this);
    this->lights[this->nLights] = light;
    this->nLights += 1;
}

ostream & operator<< (ostream &out, const Model &p)
{
    return out;
}
