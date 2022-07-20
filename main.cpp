#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <algorithm> 
#include <utility>

//struct que contém as dimensões dos retângulos da casa
typedef struct dimensoes{
    std::vector<int> largura;
    std::vector<int> comprimento;
    std::vector<int> area;

    int comprimentoLinha;   //guarda o comprimento do retângulo da linha atual

} dimensoes;

//struct que contém as dimensões das mesas
typedef struct dimensoesMesa{
    std::vector<int> larguraMesa;
    std::vector<int> comprimentoMesa;
    std::vector<int> areaMesa;
    
} dimensoesMesa;

//classe que contém os métodos para encontrar a melhor mesa para o melhor retângulo
class MaximoRetangulo {
public:
    int MaxAreaPorLinha(std::vector<std::vector<int>>& planta, int posLinhas, int numColunas);
    void AreaMaxima(std::vector<std::vector<int>>& planta, int linhas, int colunas);
    void MaiorMesa(dimensoesMesa mesas, int posicaoMesas);

    dimensoes retangulo;    //armazena as dimensões de cada retângulo
    int saidaArea, saidaComprimento, saidaLargura;  //armazena a maior área, comprimento e largura da mesa que será retornada
};
 
//calcula e retorna a área máxima por linha
int MaximoRetangulo::MaxAreaPorLinha(std::vector<std::vector<int>>& planta, int posLinhas, int numColunas) {
    std::stack<int> posicao;    //armazena os valores da coluna

    int areaMaxima = 0, topo, areaDoTopo, coluna = 0;
    retangulo.comprimentoLinha = 1;

    //checa o valor da coluna atual com o valor da anterior
    while(coluna < numColunas) {

        //se o valor da coluna anterior for menor que o da atual, empilha o valor atual da coluna e adiciona em 1 o valor da coluna
        if(posicao.empty() || planta[posLinhas][posicao.top()] <= planta[posLinhas][coluna]) {
            posicao.push(coluna);
            ++coluna;
        }

        //se não, multiplica o valor atual da coluna com o valor do topo da pilha de colunas
        else {
            topo = posicao.top();
            posicao.pop();

            if(posicao.empty()) {
                areaDoTopo = planta[posLinhas][topo] * coluna;  
            }
             else {
                areaDoTopo = planta[posLinhas][topo] * (coluna - posicao.top() - 1);
            }

            //compara se a área atual é maior que a área já armazenada
            if(areaMaxima < areaDoTopo) {
                areaMaxima = areaDoTopo;
                retangulo.comprimentoLinha = planta[posLinhas][topo];   //comprimento da maior área da linha
            }
        }
    }

    //caso a pilha ainda não esteja vazia, multiplica o valor atual da coluna com o valor do topo da pilha de colunas
    while(!posicao.empty()) {
        topo = posicao.top();
        posicao.pop();

        if(posicao.empty()) {
            areaDoTopo = planta[posLinhas][topo] * coluna;
        }   
        else {
            areaDoTopo = planta[posLinhas][topo] * (coluna - posicao.top() - 1);  
        }

        //compara se a área atual é maior que a área já armazenada
        if(areaMaxima < areaDoTopo) {
            areaMaxima = areaDoTopo;
            retangulo.comprimentoLinha = planta[posLinhas][topo];   //comprimento da maior área da linha
        }
    }
    
    retangulo.comprimento.push_back(retangulo.comprimentoLinha);

    return areaMaxima;
}

//checa qual a maior área entre todas as linhas
void MaximoRetangulo::AreaMaxima(std::vector<std::vector<int>>& planta, int linhas, int colunas) {

    for(int i = 1; i < linhas; ++i) {
        for(int j = 1; j < colunas; ++j) {

            //soma o valor da posição da linha anterior com a atual
            if(planta[i][j]) {
                planta[i][j] += planta[i - 1][j];
            }
        }

        retangulo.area.push_back(MaxAreaPorLinha(planta, i, colunas));
        retangulo.largura.push_back(retangulo.area[i - 1] / retangulo.comprimento[i - 1]);  //adquire o valor da largura sendo área/comprimento
    } 
}

//ordena de forma decrescente o vetor
void Juntar(std::vector<int> &area, std::vector<int> &comprimento, std::vector<int> &largura, int ini, int meio, int fim, std::vector<int> &areaAux, std::vector<int> &comprimentoAux, std::vector<int> &larguraAux) {
    int esq = ini;
    int dir = meio;

    //checa qual o maior valor e ordena em um vetor auxiliar
    for(int i = ini; i < fim; ++i) {
        if((esq < meio) && ((dir >= fim) || (area[esq] > area[dir]))) {
            areaAux[i] = area[esq];
            comprimentoAux[i] = comprimento[esq];
            larguraAux[i] = largura[esq];

            ++esq;
        } 
        else {
            areaAux[i] = area[dir];
            comprimentoAux[i] = comprimento[dir];
            larguraAux[i] = largura[dir];

            ++dir;
        }
    }

    //depois de ordenado, o vetor principal recebe os valores ordenados
    for(int i = ini; i < fim; ++i) {
        area[i] = areaAux[i];
        comprimento[i] = comprimentoAux[i];
        largura[i] = larguraAux[i];
    }
}

//executa um merge de forma recorrente
void Merge(std::vector<int> &area, std::vector<int> &comprimento, std::vector<int> &largura, int inicio, int fim, std::vector<int> &areaAux, std::vector<int> &comprimentoAux, std::vector<int> &larguraAux) {
    
    //parada da recorrência
    if ((fim - inicio) < 2) 
        return;
    
    int meio = ((inicio + fim) / 2);
    Merge(area, comprimento, largura, inicio, meio, areaAux, comprimentoAux, larguraAux);
    Merge(area, comprimento, largura, meio, fim, areaAux, comprimentoAux, larguraAux);

    Juntar(area, comprimento, largura, inicio, meio, fim, areaAux, comprimentoAux, larguraAux); //junta tudo depois que foi aplicado a recorrência
}

//aplica o mergeSort nas dimensões, ordenando pela dimensão área
void MergeSort(std::vector<int> &area, std::vector<int> &comprimento, std::vector<int> &largura, int &tamanho) {  
    std::vector<int> areaAux(tamanho);
    std::vector<int> comprimentoAux(tamanho);
    std::vector<int> larguraAux(tamanho);

    Merge(area, comprimento, largura, 0, tamanho, areaAux, comprimentoAux, larguraAux);
}   

//encontra qual a melhor mesa para a melhor área do retângulo
void MaximoRetangulo::MaiorMesa(dimensoesMesa mesas, int posicaoMesas) {
    saidaArea = 0, saidaComprimento = 0, saidaLargura = 0;
    int menosMesas = mesas.areaMesa.size() - mesas.areaMesa.size() * 0.2;   //retira 20% de mesas, visto elas não interferirem no resultado final

    for(int i = 0; i < retangulo.area.size(); ++i) {
        for(int j = posicaoMesas; j < menosMesas; ++j) { 
            
            //compara as dimensões da mesa com as dos retângulos
            if(mesas.comprimentoMesa[j] <= retangulo.comprimento[i]) {

                if(mesas.larguraMesa[j] <= retangulo.largura[i]) {

                    if(mesas.areaMesa[j] > saidaArea || (mesas.areaMesa[j] == saidaArea && mesas.larguraMesa[j] > saidaLargura)) {

                        saidaArea = mesas.areaMesa[j];    
                        saidaComprimento = mesas.comprimentoMesa[j];
                        saidaLargura = mesas.larguraMesa[j];

                    }
                }

                //caso o comprimento da mesa seja menor ou igual que a largura do retângulo e o mesmo com a largura e comprimento, 
                //rotaciona a mesa para poder encaixar no retângulo
                else if(mesas.larguraMesa[j] <= saidaComprimento && mesas.comprimentoMesa[j] <= saidaLargura && mesas.areaMesa[j] >= saidaArea) {
                        
                    saidaArea = mesas.areaMesa[j];    
                    saidaComprimento = mesas.comprimentoMesa[j];
                    saidaLargura = mesas.larguraMesa[j];

                }
            }

            //caso o comprimento da mesa seja maior que do retângulo e a largura da mesa é menor que o comprimento do retângulo,
            //possivelmente para poder rotacionar a mesa
            else if(mesas.larguraMesa[j] <= retangulo.comprimento[i] && mesas.comprimentoMesa[j] <= retangulo.largura[i] && mesas.areaMesa[j] >= saidaArea)  {

                saidaArea = mesas.areaMesa[j];       
                saidaComprimento = mesas.comprimentoMesa[j];
                saidaLargura = mesas.larguraMesa[j];
            }    
        }
    }
}

int main(int argc, char *argv[]) {
    std::string lin, col, listaM;                               
    char carac;                                                 
    int linhas, colunas, caracter, listaMesas, compri, larg, ite, area;    
    
    std::cin >> lin >> col;
    
    linhas = std::stoi(lin);    //transforma string para inteiro
    colunas = std::stoi(col);   //
    std::vector<std::vector<int>> planta(linhas, std::vector<int>(colunas));    //vetor com os valores da planta
    
    MaximoRetangulo resultado;  //variável que contém todas as funções
    dimensoesMesa mesas;    //variável para armazenar as dimensões das mesas

    //transforma a planta do arquivo de entrada em binário
    for(int i = 0; i < linhas; ++i) {
        for(int j = 0; j < colunas; ++j) {
            std::cin >> carac;

            if(carac == '#') 
                planta[i][j] = 0;
            else 
                planta[i][j] = 1;
        }
    }

    resultado.AreaMaxima(planta, linhas, colunas);  //calcula a área máxima de todas as linhas
 
    std::cin >> listaM;
    listaMesas = std::stoi(listaM);

    //recebe via arquivo a dimensão de cada mesa
    for(int i = 0; i < listaMesas; ++i) {
        std::cin >> col >> lin;
        compri = std::stoi(col);
        larg = std::stoi(lin);

        mesas.areaMesa.push_back(compri * larg);
        mesas.comprimentoMesa.push_back(compri);
        mesas.larguraMesa.push_back(larg);
    }
         
    //ordena em ordem decrescente as áreas dos retângulos e das mesas 
    MergeSort(resultado.retangulo.area, resultado.retangulo.comprimento, resultado.retangulo.largura, linhas);
    MergeSort(mesas.areaMesa, mesas.comprimentoMesa, mesas.larguraMesa, listaMesas);

    area = resultado.retangulo.area[0];

    //recebe qual a primeira posição em que a área da mesa é menor ou igual ao do maior retângulo
    for(ite = 0;; ++ite) {
        if(mesas.areaMesa[ite] <= area)
            break;
    }

    //retorna o comprimento e largura da mesa máxima que se encaixa no retângulo
    resultado.MaiorMesa(mesas, ite);   

    //imprime as dimensões da mesa
    std::cout << resultado.saidaComprimento << " " << resultado.saidaLargura;
        
    return 0;
}