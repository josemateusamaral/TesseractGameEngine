#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

enum RenderType {
    WIREFRAME = 1,
    SHADED = 2,
    TEXTURED = 3
};

Model::Model()
{
    
}

Model::Model(string filePath, Vec3 position, double scale)
:position{position}, scale{scale}
{

    nLights = 0;
    maxNLight = 10;

    lights = new Light*[maxNLight];

    iluminacao = Vec3(1,1,0);
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

    // Ler o arquivo
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Erro do Assimp: " << importer.GetErrorString() << std::endl;
        return;
    }

    // verificar se o modelo tem mesh
    if (scene->mNumMeshes == 0) {
        std::cout << "Erro: O modelo carregou, mas nao contem nenhuma mesh!" << std::endl;
        return;
    }

    // Pegando a primeira mesh
    aiMesh* mesh = scene->mMeshes[0];

    // iniciate class control variables
    this->nVertices = mesh->mNumVertices;
    this->vertices = new Vec3[nVertices]; 
    this->projection = new Vec3[nVertices];
    this->pontos = new Vec3[nVertices];
    this->uvs = new Vec3[nVertices];
    this->polygonCount = mesh->mNumFaces;


    if(scene->mNumMaterials > 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString str;

        if(material->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS) {
            int width, height, nrChannels;
            unsigned char* data = nullptr;
            const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());

            // Textura embutida
            if (embeddedTexture) {
                
                if (embeddedTexture->mHeight == 0) {
                    data = stbi_load_from_memory(
                        reinterpret_cast<unsigned char*>(embeddedTexture->pcData),
                        embeddedTexture->mWidth,
                        &width, &height, &nrChannels, 3
                    );
                }

            // Textura externa
            } else {
                
                string directory = path.substr(0, path.find_last_of('/'));
                string texPath = directory + "/" + string(str.C_Str());
                data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 3);
            }

            if (data) {
                this->diffuseTexture = new Texture((char*)data, width, height);
                printf("Sucesso: Textura carregada (%dx%d)\n", width, height);
            } else {
                printf("Erro ao carregar textura: %s\n", stbi_failure_reason());
            }
        } else {
            printf("Aviso: Nenhuma textura encontrada para esta mesh.\n");
        }
    }



    // guardar todos os vertices e uvs em um array 
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {        
        
        // vertice
        this->vertices[i] = Vec3(
            mesh->mVertices[i].x,// * tamanho, 
            mesh->mVertices[i].y,// * tamanho, 
            mesh->mVertices[i].z// * tamanho
        );

        //projection
        this->projection[i] = Vec3(0.0f, 0.0f, 0.0f);

        // uv
        if (mesh->mTextureCoords[0]) {
            this->uvs[i] = Vec3(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y,
                0.0f
            );
        } else {
            this->uvs[i] = Vec3(0.0f, 0.0f, 0.0f);
        }
    }

    // load indixes
    this->indexCount = mesh->mNumFaces * 3;
    this->indices = new unsigned int[indexCount];

    unsigned int idx = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        
        aiFace face = mesh->mFaces[i];
        if (face.mNumIndices != 3) continue;

        this->indices[idx++] = face.mIndices[0];
        this->indices[idx++] = face.mIndices[1];
        this->indices[idx++] = face.mIndices[2];
    }

    printf("Modelo carregado: %d vertices, %d faces\n", mesh->mNumVertices, mesh->mNumFaces);

}

/**
 * @brief rendericar a esfera nos formatos Wireframe ou Shaded
 *
 * @authors Jose Mateus Amaral
 */
void Model::draw(Window &window, Camera *camera)
{

    //printf("Desenhando modelo\n");

    calcular_pontos_3D();

    camera->project(this->pontos, this->projection, this->nVertices);

    int R = 255, G = 255, B = 255;
    double angulo;

    double origem[3] = {camera->posicao.x, camera->posicao.y, camera->posicao.z};
    double a[3] = {this->position.x, this->position.y, this->position.z};
    Vec3 camToObj{origem, a};

    // indexes
    for (int i = 0; i < indexCount; i += 3)
    {

        //printf("Desenhando triangulo %d\n", i / 3);

        int i0 = indices[i];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];

        // pontos no espaço 3D
        Vec3 p0 = pontos[i0];
        Vec3 p1 = pontos[i1];
        Vec3 p2 = pontos[i2];
        double b[3] = {pontos[i0].x, pontos[i0].y, pontos[i0].z};
        double c[3] = {pontos[i1].x, pontos[i1].y, pontos[i1].z};
        double d[3] = {pontos[i2].x, pontos[i2].y, pontos[i2].z};
        Vec3 v1(b, c);
        Vec3 v2(b, d);

        // normal do triângulo
        Vec3 normal = v1.produto_vetorial(v2);

        
        

        // vetor do objeto até o triângulo
        Vec3 objToTri(a, b);

        switch(renderType)
        {
            // WIREFRAME
            case RenderType::WIREFRAME:

                // backface culling
                if (camToObj.angulo_entre_vetores(normal) > 90 || !this->backfaceCulling)
                {

                    window.drawLine(projection[i0], projection[i1]);
                    window.drawLine(projection[i1], projection[i2]);
                    window.drawLine(projection[i2], projection[i0]);
                    
                }
                break;

            // SHADED
            case RenderType::SHADED:
            {

                // backface culling
                if (camToObj.angulo_entre_vetores(normal) > 90 || !this->backfaceCulling)
                {
                    if (shadowCast)
                    {
                        angulo = normal.angulo_entre_vetores(iluminacao);
                        R = corR - ((255.0 / 180.0) * angulo);
                        G = corG - ((255.0 / 180.0) * angulo);
                        B = corB - ((255.0 / 180.0) * angulo);
                    }
                    else
                    {
                        R = corR;
                        G = corG;
                        B = corB;
                    }

                    window.drawBlankPolygon(
                        projection[i0],
                        projection[i1],
                        projection[i2],
                        R, G, B
                    );
                }
                break;
            }

            // TEXTURED
            case RenderType::TEXTURED:
            {

                // backface culling
                if (camToObj.angulo_entre_vetores(normal) > 90 || !this->backfaceCulling)
                {
                    
                    window.drawTexturedPolygon(
                        //projections
                        projection[i0],
                        projection[i1],
                        projection[i2],
                        //uvs
                        uvs[i0],
                        uvs[i1],
                        uvs[i2],
                        //texture
                        (unsigned char*)diffuseTexture->data,
                        diffuseTexture->width,
                        diffuseTexture->height,
                        //lights
                        this->lights,
                        this->nLights
                    );
                }
                break;
            }
        }
    }
    //printf("modelo desenhado\n");
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
    double x,y,z,seno,cosseno;

    for( int i = 0 ; i < nVertices ; i++ ){
        
        seno = sin( rotacaoX * M_PI / 180 );
        cosseno = cos( rotacaoX * M_PI / 180 );
        vertices[i].x = vertices[i].x * cosseno - vertices[i].z * seno;
        vertices[i].z = vertices[i].z * cosseno + vertices[i].x * seno;
        
        y = vertices[i].y;
        z = vertices[i].z;
        seno = sin( rotacaoY * M_PI / 180 );
        cosseno = cos( rotacaoY * M_PI / 180 );
        vertices[i].y = y * cosseno - z * seno;
        vertices[i].z = z * cosseno + y * seno;
        
        x = vertices[i].x;
        y = vertices[i].y;
        seno = sin( rotacaoZ * M_PI / 180 );
        cosseno = cos( rotacaoZ * M_PI / 180 );
        vertices[i].y = y * cosseno - x * seno;
        vertices[i].x = x * cosseno + y * seno;
    
    }
}

void Model::setPos(double x, double y, double z){
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
}

void Model::setPos(Vec3 posicao){
    this->position = posicao;
}

void Model::setX(double x){
    this->position.x = x;
}

void Model::setY(double y){
    this->position.y = y;
}

void Model::setZ(double z){
    this->position.z = z;
}

Vec3 Model::getPos(){
    return this->position;
}

double Model::getX(){
    return this->position.x;
}

double Model::getY(){
    return this->position.y;
}

double Model::getZ(){
    return this->position.z;
}

void Model::setScale(double tamanho){
    this->scale = tamanho;
}

double Model::getScale(){
    return this->scale;
}

void Model::setLight(Light *light){
    this->lights[this->nLights] = light;
    this->nLights += 1;
    printf("Light added. Total lights: %d\n", this->nLights);
}

ostream & operator<< (ostream &out, const Model &p)
{
    return out;
}
