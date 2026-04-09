//#include "../formas/forma.h"
//#include "../pontos/ponto3.h"
//#include "../pontos/ponto.h"
#include "model.h"
//#include "Vec3.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

/**
 * @brief Construtor do sólido geométrico Esfera
 * 
 * @param posicao posição do centro no R3
 * @param tamanho Raio da esfera
 * @param lod Quantidade de pontos, para distorção
 *
 * @authors Jose Mateus Amaral
 */
Model::Model(Ponto3 posicao, double tamanho, int lod)
:Forma(posicao,tamanho),LOD{lod}
{
    iluminacao = Vec3(1,1,0);
    faixas = LOD / 2 + 1;
    quantidadePontos = LOD * faixas;
    
}

Model::Model(Ponto3 posicao,int lod)
:Forma(posicao,1),LOD{lod}
{
    iluminacao = Vec3(1,1,0);
    faixas = LOD / 2 + 1;
    quantidadePontos = LOD * faixas;
}

void Model::calcular_pontos_base(){

}

void Model::setBackface_Culling(bool value){
    this->backface_culling = value;
}

bool Model::getBackface_Culling(){
    return this->backface_culling;
}

void Model::loadModel(string path)
{

    printf("chegando aqui 1...\n"); // <-- Adicionado \n

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
    this->quantidadePontos = mesh->mNumVertices;

    printf("chegando aqui...");

    pontos_base = new Ponto3[quantidadePontos]; 
    //uvs = new Ponto[quantidadePontos];

    // guardar todos os vertices e uvs em um array 
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {        
        pontos_base[i] = Ponto3(
            mesh->mVertices[i].x * tamanho, 
            mesh->mVertices[i].y * tamanho, 
            mesh->mVertices[i].z * tamanho
        );

        // UV
        /*if (mesh->mTextureCoords[0]) {
            uvs[i] = Ponto(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        } else {
            uvs[i] = Ponto(0.0f, 0.0f);
        }*/
    }
    
    

    // Carregar Polígonos
    this->polygonCount = mesh->mNumFaces;

    

    // Cada face tem 3 índices (triângulos)
    this->indexCount = mesh->mNumFaces * 3;

    indices = new unsigned int[indexCount];

    unsigned int idx = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        // Garantia extra (embora Triangulate já faça isso)
        if (face.mNumIndices != 3) continue;

        indices[idx++] = face.mIndices[0];
        indices[idx++] = face.mIndices[1];
        indices[idx++] = face.mIndices[2];
    }

}

/**
 * @brief rendericar a esfera nos formatos Wireframe ou Shaded
 *
 * @authors Jose Mateus Amaral, Monique
 */
void Model::desenhar(Window &window)
{
    /*
    // calcular pontos do R2 calculando primeiramente os pontos do R3
    calcular_pontos_3D();
    projecao = camera->projetar(pontos,quantidadePontos);

    // posicao do ponto ao lado do ponto atual
    int index;

    // tipo de renderização
    int type = 0;

    // cor dos poligonos
    int R = 255;
    int G = 255;
    int B = 255;

    // angulo entre 
    double angulo;

    // listas usadas para a criação de vetores
    double a[3] = {posicao.x,posicao.y,posicao.z};
    double b[3];
    double c[3];

    // vetor da camera até o objeto
    double origem[3] = {0,0,0};
    Vec3 camToObj{origem,a};

    // percorrer a lista de pontos da projeção e renderiza-los no modo WIREFRAME ou SHADED
    for(int j = 0; j < quantidadePontos; j++)
    {

        // calcular posição do proximo ponto que sera ligado ao ponto atual
        if( ( ( j + 1 ) % LOD ) == 0 ) {
            index = j - LOD + 1;
        }
        else{
            index = j + 1;
        }

        switch(renderType){

            // renderizar usando o modo wireframe
            case 1:
                window.desenha( projecao[j],projecao[index]);
                if( j > LOD - 1 ){
                    window.desenha( projecao[j], projecao[ j - LOD ]);
                }  
                break;

            // renderizar usando o modo shaded
            case 2:
                
                // vertice B do poligono
                b[0] = pontos[j].x;
                b[1] = pontos[j].y;
                b[2] = pontos[j].z;

                //verticar C do poligono
                c[0] = pontos[index].x;
                c[1] = pontos[index].y;
                c[2] = pontos[index].z;

                // vetores do centro do objeto até o poligono
                Vec3 objToponto1(a,b);
                Vec3 objToponto2(a,c);

                // calcular a cor do poligono usando o angulo entre o vetor normal do poligono e o vetor de iluminação
                if(camToObj.angulo_entre_vetores(objToponto1) > 90 && camToObj.angulo_entre_vetores(objToponto2) > 90){
                    if( j > LOD - 1 ){

                        // calcular a cor do poligono usando o algulo entre o vetor de iluminação e o vetor normal do poligono
                        if(comSombra){
                            angulo = ( objToponto1.angulo_entre_vetores(iluminacao) + objToponto2.angulo_entre_vetores(iluminacao) ) / 2 ;
                            R = corR - (( 255 / 180 ) * angulo);
                            G = corG - (( 255 / 180 ) * angulo);
                            B = corB - (( 255 / 180 ) * angulo);
                        }

                        // usar a cor normal sem calcular a iluminação
                        else{
                            R = corR;
                            G = corG;
                            B = corB;
                        }

                        //desenhar poligonos
                        window.desenhar_poligono(projecao[j],projecao[j - LOD],projecao[j - LOD + 1],R,G,B);
                        window.desenhar_poligono(projecao[j],projecao[j - 1],projecao[j - LOD],R,G,B);
                    }
                }
                break;       
        }
    }*/

    calcular_pontos_3D();
    projecao = camera->projetar(pontos, quantidadePontos);

    int R = 255, G = 255, B = 255;
    double angulo;

    double origem[3] = {0,0,0};
    double a[3] = {posicao.x, posicao.y, posicao.z};
    Vec3 camToObj{origem, a};

    // 🔥 Percorrer TRIÂNGULOS (não vértices)
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
            // 🟢 WIREFRAME
            case 1:

                // backface culling
                if (camToObj.angulo_entre_vetores(normal) > 90 || !this->backface_culling)
                {

                    window.desenha(projecao[i0], projecao[i1]);
                    window.desenha(projecao[i1], projecao[i2]);
                    window.desenha(projecao[i2], projecao[i0]);
                    
                }
                break;

            // 🔵 SHADED
            case 2:
            {

                // backface culling
                if (camToObj.angulo_entre_vetores(normal) > 90 || !this->backface_culling)
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
        }
    }

}

ostream & operator<< (ostream &out, const Model &p)
{
    return out;
}
