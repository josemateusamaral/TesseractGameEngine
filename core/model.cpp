#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

enum RenderType {
    WIREFRAME = 1,
    SHADED = 2,
    TEXTURED = 3
};

Model::Model(string filePath, Ponto3 posicao, double tamanho)
:posicao{posicao}, tamanho{tamanho}
{
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
    this->quantidadePontos = mesh->mNumVertices;
    pontos_base = new Ponto3[quantidadePontos]; 
    uvs = new Ponto[quantidadePontos];
    this->polygonCount = mesh->mNumFaces;



    if(scene->mNumMaterials > 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString str;

        if(material->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS) {
            int width, height, nrChannels;
            unsigned char* data = nullptr;

            // Verifica se é uma textura embutida (embedded)
            const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());

            if (embeddedTexture) {
                // Textura comprimida (png/jpg) dentro do arquivo
                if (embeddedTexture->mHeight == 0) {
                    data = stbi_load_from_memory(
                        reinterpret_cast<unsigned char*>(embeddedTexture->pcData),
                        embeddedTexture->mWidth, // mWidth aqui é o tamanho em bytes
                        &width, &height, &nrChannels, 3
                    );
                }
            } else {
                // Textura externa
                string directory = path.substr(0, path.find_last_of('/'));
                string texPath = directory + "/" + string(str.C_Str());
                data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 3);
            }

            if (data) {
                this->texture = new Texture((char*)data, width, height);
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
        pontos_base[i] = Ponto3(
            mesh->mVertices[i].x * tamanho, 
            mesh->mVertices[i].y * tamanho, 
            mesh->mVertices[i].z * tamanho
        );

        // uv
        if (mesh->mTextureCoords[0]) {
            uvs[i] = Ponto(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        } else {
            uvs[i] = Ponto(0.0f, 0.0f);
        }
    }

    // load indixes
    this->indexCount = mesh->mNumFaces * 3;
    indices = new unsigned int[indexCount];

    unsigned int idx = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        
        aiFace face = mesh->mFaces[i];
        if (face.mNumIndices != 3) continue;

        indices[idx++] = face.mIndices[0];
        indices[idx++] = face.mIndices[1];
        indices[idx++] = face.mIndices[2];
    }

    printf("Modelo carregado: %d vertices, %d faces\n", mesh->mNumVertices, mesh->mNumFaces);

}

/**
 * @brief rendericar a esfera nos formatos Wireframe ou Shaded
 *
 * @authors Jose Mateus Amaral, Monique
 */
void Model::draw(Window &window)
{

    calcular_pontos_3D();
    projecao = camera->projetar(pontos, quantidadePontos);

    int R = 255, G = 255, B = 255;
    double angulo;

    double origem[3] = {0,0,0};
    double a[3] = {posicao.x, posicao.y, posicao.z};
    Vec3 camToObj{origem, a};

    // Percorrer TRIÂNGULOS
    for (int i = 0; i < indexCount; i += 3)
    {

        int i0 = indices[i];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];

        // pontos no espaço 3D
        Ponto3 p0 = pontos[i0];
        Ponto3 p1 = pontos[i1];
        Ponto3 p2 = pontos[i2];

        double b[3] = {p0.x, p0.y, p0.z};
        double c[3] = {p1.x, p1.y, p1.z};
        double d[3] = {p2.x, p2.y, p2.z};

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

                    window.desenha(projecao[i0], projecao[i1]);
                    window.desenha(projecao[i1], projecao[i2]);
                    window.desenha(projecao[i2], projecao[i0]);
                    
                }
                break;

            // SHADED
            case RenderType::SHADED:
            {

                // backface culling
                if (camToObj.angulo_entre_vetores(normal) > 90 || !this->backfaceCulling)
                {
                    if (comSombra)
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

                    window.desenhar_poligono(
                        projecao[i0],
                        projecao[i1],
                        projecao[i2],
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

                    window.desenhar_poligono_texturizado(
                        projecao[i0],
                        projecao[i1],
                        projecao[i2],
                        uvs[i0],
                        uvs[i1],
                        uvs[i2],
                        (unsigned char*)texture->data,
                        texture->width,
                        texture->height
                    );
                }
                break;
            }
        }
    }

}

/**
 * @brief A partir do centro e do tamanho do sólido, calcula a posição dos pontos restantes.
 * @author Jose Mateus Amaral, Monique
 */
void Model::calcular_pontos_3D()
{
    double px = posicao.x-camera->posicao.x;
    double py = posicao.y-camera->posicao.y;
    double pz = posicao.z-camera->posicao.z;
    double t = tamanho;

    Ponto3* pontosTemp = (Ponto3*)malloc(sizeof(Ponto3) * quantidadePontos);
    for(int i = 0 ; i < quantidadePontos ; i++ ){
        pontosTemp[i] = pontos_base[i];
        pontosTemp[i].x = px + pontosTemp[i].x * t;
        pontosTemp[i].y = py + pontosTemp[i].y * t;
        pontosTemp[i].z = pz + pontosTemp[i].z * t;
    }

    pontos = pontosTemp;
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

    for( int i = 0 ; i < quantidadePontos ; i++ ){
        
        x = pontos_base[i].x;
        z = pontos_base[i].z;
        seno = sin( rotacaoX * M_PI / 180 );
        cosseno = cos( rotacaoX * M_PI / 180 );
        pontos_base[i].x = x * cosseno - z * seno;
        pontos_base[i].z = z * cosseno + x * seno;
        
        y = pontos_base[i].y;
        z = pontos_base[i].z;
        seno = sin( rotacaoY * M_PI / 180 );
        cosseno = cos( rotacaoY * M_PI / 180 );
        pontos_base[i].y = y * cosseno - z * seno;
        pontos_base[i].z = z * cosseno + y * seno;
        
        x = pontos_base[i].x;
        y = pontos_base[i].y;
        seno = sin( rotacaoZ * M_PI / 180 );
        cosseno = cos( rotacaoZ * M_PI / 180 );
        pontos_base[i].y = y * cosseno - x * seno;
        pontos_base[i].x = x * cosseno + y * seno;
    
    }
}

void Model::setPos(double x, double y, double z){
    this->posicao.x = x;
    this->posicao.y = y;
    this->posicao.z = z;
}

void Model::setPos(Ponto3 posicao){
    this->posicao = posicao;
}

void Model::setX(double x){
    this->posicao.x = x;
}

void Model::setY(double y){
    this->posicao.y = y;
}

void Model::setZ(double z){
    this->posicao.z = z;
}

Ponto3 Model::getPos(){
    return this->posicao;
}

void Model::setScale(double tamanho){
    this->tamanho = tamanho;
}

double Model::getScale(){
    return this->tamanho;
}

ostream & operator<< (ostream &out, const Model &p)
{
    return out;
}
