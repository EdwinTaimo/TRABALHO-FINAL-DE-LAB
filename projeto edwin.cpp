#include <iostream>
#include <string>
#include <fstream> 
#include <cstdlib> 

using namespace std;

#ifndef nullptr
#define nullptr NULL
#endif

// ============================================================================
// ESTRUTURA: Estudante
// ============================================================================
struct Estudante {
    int id;
    string nome;
    float nota;
    float rendaFamiliar;
    float pontuacaoPrioridade; 

    void calcularPrioridade() {
        float fatorRenda = (rendaFamiliar > 0) ? (10000.0f / rendaFamiliar) : 100.0f;
        if (fatorRenda > 50.0f) fatorRenda = 50.0f; 
        this->pontuacaoPrioridade = (nota * 0.6f) + (fatorRenda * 0.4f);
    }
};

// ============================================================================
// ESTRUTURA: NodoBST
// ============================================================================
struct NodoBST {
    Estudante* estudante; 
    NodoBST* esquerda;    
    NodoBST* direita;     

    NodoBST(Estudante* est) : estudante(est), esquerda(nullptr), direita(nullptr) {}
};

// ============================================================================
// CLASSE: ArvoreBST (Índice Secundário por ID)
// ============================================================================
class ArvoreBST {
private:
    NodoBST* raiz;

    NodoBST* inserirRec(NodoBST* nodo, Estudante* est) {
        if (nodo == nullptr) return new NodoBST(est);
        if (est->id < nodo->estudante->id)
            nodo->esquerda = inserirRec(nodo->esquerda, est);
        else if (est->id > nodo->estudante->id)
            nodo->direita = inserirRec(nodo->direita, est);
        return nodo;
    }

    Estudante* pesquisarRec(NodoBST* nodo, int id) {
        if (nodo == nullptr || nodo->estudante->id == id) 
            return (nodo != nullptr) ? nodo->estudante : nullptr;
        if (id < nodo->estudante->id)
            return pesquisarRec(nodo->esquerda, id);
        return pesquisarRec(nodo->direita, id);
    }

    NodoBST* encontrarMinimo(NodoBST* nodo) {
        while (nodo && nodo->esquerda != nullptr) nodo = nodo->esquerda;
        return nodo;
    }

    NodoBST* removerRec(NodoBST* nodo, int id) {
        if (nodo == nullptr) return nodo;

        if (id < nodo->estudante->id)
            nodo->esquerda = removerRec(nodo->esquerda, id);
        else if (id > nodo->estudante->id)
            nodo->direita = removerRec(nodo->direita, id);
        else {
            if (nodo->esquerda == nullptr) {
                NodoBST* temp = nodo->direita;
                delete nodo;
                return temp;
            } else if (nodo->direita == nullptr) {
                NodoBST* temp = nodo->esquerda;
                delete nodo;
                return temp;
            }
            NodoBST* temp = encontrarMinimo(nodo->direita);
            nodo->estudante = temp->estudante;
            nodo->direita = removerRec(nodo->direita, temp->estudante->id);
        }
        return nodo;
    }

    void exibirEmOrdem(NodoBST* nodo) {
        if (nodo != nullptr) {
            exibirEmOrdem(nodo->esquerda);
            cout << "ID: " << nodo->estudante->id 
                 << " | Nome: " << nodo->estudante->nome 
                 << " | Prioridade: " << nodo->estudante->pontuacaoPrioridade << endl;
            exibirEmOrdem(nodo->direita);
        }
    }

    void gravarFicheiroRec(NodoBST* nodo, ofstream &arq) {
        if (nodo != nullptr) {
            gravarFicheiroRec(nodo->esquerda, arq);
            arq << nodo->estudante->id << ";" << nodo->estudante->nome << ";"
                << nodo->estudante->nota << ";" << nodo->estudante->rendaFamiliar << "\n";
            gravarFicheiroRec(nodo->direita, arq);
        }
    }

    void calcularEstatisticasRec(NodoBST* nodo, float &somaNotas, int &total, int &vulneraveis) {
        if (nodo != nullptr) {
            calcularEstatisticasRec(nodo->esquerda, somaNotas, total, vulneraveis);
            somaNotas += nodo->estudante->nota;
            total++;
            if (nodo->estudante->rendaFamiliar <= 3000) vulneraveis++;
            calcularEstatisticasRec(nodo->direita, somaNotas, total, vulneraveis);
        }
    }

public:
    ArvoreBST() : raiz(nullptr) {}

    // Getter necessário para podermos aceder à raiz real de forma segura ao sincronizar
    NodoBST* getRaiz() { return raiz; }

    void inserir(Estudante* est) { raiz = inserirRec(raiz, est); }
    Estudante* pesquisar(int id) { return pesquisarRec(raiz, id); }
    void remover(int id) { raiz = removerRec(raiz, id); }
    
    void listarTodos() {
        if (raiz == nullptr) cout << "Nenhum estudante registado no indice.\n";
        else exibirEmOrdem(raiz);
    }

    void gravarFicheiro(ofstream &arq) { gravarFicheiroRec(raiz, arq); }

    void obterEstatisticas(float &somaNotas, int &total, int &vulneraveis) {
        somaNotas = 0; total = 0; vulneraveis = 0;
        calcularEstatisticasRec(raiz, somaNotas, total, vulneraveis);
    }
};

// ============================================================================
// CLASSE: MaxHeap (Fila de Prioridade Máxima)
// ============================================================================
class MaxHeap {
private:
    Estudante** heap; 
    int capacidade;   
    int tamanho;      

    void trocar(int i, int j) {
        Estudante* temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }

    void subirHeap(int indice) {
        while (indice > 0) {
            int pai = (indice - 1) / 2;
            if (heap[indice]->pontuacaoPrioridade > heap[pai]->pontuacaoPrioridade) {
                trocar(indice, pai);
                indice = pai;
            } else break;
        }
    }

    void descerHeap(int indice) {
        int maior = indice;
        int esquerda = 2 * indice + 1;
        int direita = 2 * indice + 2;

        if (esquerda < tamanho && heap[esquerda]->pontuacaoPrioridade > heap[maior]->pontuacaoPrioridade)
            maior = esquerda;
        if (direita < tamanho && heap[direita]->pontuacaoPrioridade > heap[maior]->pontuacaoPrioridade)
            maior = direita;

        if (maior != indice) {
            trocar(indice, maior);
            descerHeap(maior);
        }
    }

public:
    MaxHeap(int cap) {
        capacidade = cap;
        tamanho = 0;
        heap = new Estudante*[capacidade];
    }

    ~MaxHeap() { delete[] heap; }

    void limpar() { tamanho = 0; }

    bool inserir(Estudante* est) {
        if (tamanho == capacidade) return false;
        heap[tamanho] = est;
        subirHeap(tamanho);
        tamanho++;
        return true;
    }

    Estudante* removerMax() {
        if (tamanho <= 0) return nullptr;
        Estudante* raizMax = heap[0];
        heap[0] = heap[tamanho - 1];
        tamanho--;
        descerHeap(0);
        return raizMax; 
    }

    bool removerPorId(int id) {
        int indice = -1;
        for (int i = 0; i < tamanho; i++) {
            if (heap[i]->id == id) { indice = i; break; }
        }
        if (indice == -1) return false;

        heap[indice] = heap[tamanho - 1];
        tamanho--;
        if (indice < tamanho) {
            subirHeap(indice);
            descerHeap(indice);
        }
        return true;
    }

    Estudante* espreitarMax() { return (tamanho <= 0) ? nullptr : heap[0]; }
    int getTamanho() { return tamanho; }
};

// ============================================================================
// FUNÇÕES DE PERSISTÊNCIA E HISTÓRICO
// ============================================================================

void atualizarFicheiro(ArvoreBST &arvore) {
    ofstream arq("estudantes.txt", ios::trunc); 
    if (arq.is_open()) {
        arvore.gravarFicheiro(arq);
        arq.close();
    }
}

void anexarAoFicheiro(Estudante* est) {
    ofstream arq("estudantes.txt", ios::app); 
    if (arq.is_open()) {
        arq << est->id << ";" << est->nome << ";" << est->nota << ";" << est->rendaFamiliar << "\n";
        arq.close();
    }
}

void registarHistorico(Estudante* est, string beneficio) {
    ofstream teste("beneficiados.txt", ios::out | ios::in);
    if (!teste.is_open()) {
        ofstream criar("beneficiados.txt");
        criar.close();
    }
    teste.close();

    ofstream arq("beneficiados.txt", ios::app);
    if (arq.is_open()) {
        arq << "ID: " << est->id << " | Nome: " << est->nome 
            << " | Beneficio: " << beneficio << " | Score: " << est->pontuacaoPrioridade << "\n";
        arq.close();
        cout << "[SISTEMA] Historico registado em 'beneficiados.txt'!\n";
    } else {
        cout << "[ERRO] Falha ao aceder ao ficheiro beneficiados.txt\n";
    }
}

// Auxiliar recursivo para varrer a árvore e recolocar todos no Heap de forma correta
void reconstruirHeapRec(NodoBST* nodo, MaxHeap &fila) {
    if (nodo != nullptr) {
        reconstruirHeapRec(nodo->esquerda, fila);
        fila.inserir(nodo->estudante); // Insere o estudante de volta na fila de prioridade
        reconstruirHeapRec(nodo->direita, fila);
    }
}

// CORREÇÃO: Passa agora a raiz real mapeada da árvore de forma correta
void sincronizarHeap(MaxHeap &fila, ArvoreBST &arvore) {
    fila.limpar();
    reconstruirHeapRec(arvore.getRaiz(), fila); 
}

void carregarDadosOuCriarPadrao(MaxHeap &fila, ArvoreBST &arvore) {
    ifstream arq("estudantes.txt");
    if (arq.is_open()) {
        string linha;
        while (getline(arq, linha)) {
            if (linha.empty()) continue;
            Estudante* est = new Estudante();
            size_t pos = 0;

            pos = linha.find(";");
            est->id = atoi(linha.substr(0, pos).c_str());
            linha.erase(0, pos + 1);

            pos = linha.find(";");
            est->nome = linha.substr(0, pos);
            linha.erase(0, pos + 1);

            pos = linha.find(";");
            est->nota = atof(linha.substr(0, pos).c_str());
            linha.erase(0, pos + 1);

            est->rendaFamiliar = atof(linha.c_str());
            est->calcularPrioridade();
            
            fila.inserir(est);
            arvore.inserir(est);
        }
        arq.close();
        cout << "[SISTEMA] Base de dados carregada a partir de 'estudantes.txt'.\n";
    } else {
        cout << "[SISTEMA] Criando novo ficheiro com estudantes padrao...\n";
        string nomes[] = {
            "Edwin Taimo", "Ancha Francisco", "Carlos Manuel", "Delfina Joao", "Eunice Bernardo",
            "Dercio Filipe Chipura", "Daniel Armando", "Joana de Assuncao Uelemo", "Kelven Jossamo",
            "Leyla da Gloria", "Alberto Alface", "Caetano Sande"
        };
        int ids[] = {105, 101, 104, 102, 103, 106, 107, 108, 109, 110, 111, 112};
        float notas[] = {16.5, 14.0, 18.2, 12.5, 15.0, 14.5, 15.8, 17.0, 13.2, 16.1, 12.0, 14.2};
        float rendas[] = {4500, 2500, 12000, 1500, 6000, 3200, 5000, 2200, 1800, 4100, 2900, 3500};

        for (int i = 0; i < 12; i++) {
            Estudante* est = new Estudante();
            est->id = ids[i]; est->nome = nomes[i]; est->nota = notas[i]; est->rendaFamiliar = rendas[i];
            est->calcularPrioridade();
            fila.inserir(est); arvore.inserir(est);
        }
        atualizarFicheiro(arvore);
    }
}

// ============================================================================
// FUNÇÃO PRINCIPAL (MENU)
// ============================================================================
int main() {
    int maxEstudantes = 100;
    int stockComputadores = 30; 
    int totalEntregues = 0;
    MaxHeap filaPrioridade(maxEstudantes);
    ArvoreBST indiceBST;

    carregarDadosOuCriarPadrao(filaPrioridade, indiceBST);

    int opcao;
    do {
        cout << "\n=========================================\n";
        cout << "  SISTEMA DE ALOCACAO DE BOLSAS E VAGAS\n";
        cout << "  Stock de Computadores Disponiveis: " << stockComputadores << "\n";
        cout << "=========================================\n";
        cout << "1. Inscrever Estudante (Insercao)\n";
        cout << "2. Chamar Proximo Beneficiado (Bolsa de Estudo)\n";
        cout << "3. Programa: \"Um Estudante, Um Computador\"\n";
        cout << "4. Ver Estudante no Topo da Fila\n";
        cout << "5. Pesquisar Estudante por ID (Busca BST)\n";
        cout << "6. Listar Todos Ordenados por ID (BST Em-Ordem)\n";
        cout << "7. Atualizar Notas/Renda de Estudante\n";
        cout << "8. Abastecer Stock de Computadores\n";
        cout << "9. Painel Estatistico e Relatorios\n";
        cout << "10. Cancelar Inscricao (Remover do Sistema)\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1: {
                Estudante* novo = new Estudante();
                cout << "Digite o ID do estudante: "; cin >> novo->id;
                if (indiceBST.pesquisar(novo->id) != nullptr) {
                    cout << "Erro: ID ja existe!\n"; delete novo; break;
                }
                cin.ignore();
                cout << "Nome Completo: "; getline(cin, novo->nome);
                cout << "Nota Media Academicista: "; cin >> novo->nota;
                cout << "Renda Familiar Mensal: "; cin >> novo->rendaFamiliar;
                novo->calcularPrioridade();

                if (filaPrioridade.inserir(novo)) {
                    indiceBST.inserir(novo);
                    anexarAoFicheiro(novo); 
                    cout << "\nEstudante inscrito e salvo com sucesso!\n";
                } else { cout << "Erro: Heap Cheio.\n"; delete novo; }
                break;
            }
            case 2: {
                Estudante* contemplado = filaPrioridade.removerMax();
                if (contemplado != nullptr) {
                    cout << "\n*** BOLSA ATRIBUIDA A: " << contemplado->nome << " ***\n";
                    registarHistorico(contemplado, "Bolsa de Estudo");
                    indiceBST.remover(contemplado->id);
                    atualizarFicheiro(indiceBST); 
                } else cout << "\nFila vazia.\n";
                break;
            }
            case 3: {
                if (stockComputadores <= 0) { cout << "\nStock esgotado!\n"; break; }
                Estudante* proximo = filaPrioridade.espreitarMax();
                if (proximo != nullptr) {
                    cout << "\nCandidato na vez: " << proximo->nome << " | Score: " << proximo->pontuacaoPrioridade << "\n";
                    char conf; cout << "Confirmar entrega? (S/N): "; cin >> conf;
                    if (conf == 'S' || conf == 's') {
                        filaPrioridade.removerMax();
                        stockComputadores--; totalEntregues++;
                        registarHistorico(proximo, "Computador Portatil");
                        indiceBST.remover(proximo->id);
                        atualizarFicheiro(indiceBST); 
                        cout << ">> PORTATIL ENTREGUE! <<\n";
                    }
                } else cout << "\nNenhum estudante na fila.\n";
                break;
            }
            case 4: {
                Estudante* topo = filaPrioridade.espreitarMax();
                if (topo != nullptr) cout << "\nNo topo: " << topo->nome << " (" << topo->pontuacaoPrioridade << ")\n";
                else cout << "\nFila vazia.\n";
                break;
            }
            case 5: {
                int idBusca; cout << "ID a pesquisar: "; cin >> idBusca;
                Estudante* res = indiceBST.pesquisar(idBusca);
                if (res != nullptr) cout << "\nEncontrado: " << res->nome << " | Nota: " << res->nota << " | Renda: " << res->rendaFamiliar << "\n";
                else cout << "\nNao encontrado.\n";
                break;
            }
            case 6:
                cout << "\n--- Lista Geral por ID (BST) ---\n";
                indiceBST.listarTodos();
                break;
            case 7: {
                int idMod; cout << "Digite o ID do estudante a atualizar: "; cin >> idMod;
                Estudante* res = indiceBST.pesquisar(idMod);
                if (res != nullptr) {
                    cout << "Estudante selecionado: " << res->nome << "\n";
                    cout << "Nova Nota: "; cin >> res->nota;
                    cout << "Nova Renda Familiar: "; cin >> res->rendaFamiliar;
                    res->calcularPrioridade();
                    
                    // CORREÇÃO AQUI: Sincroniza corretamente usando a nova função corrigida
                    sincronizarHeap(filaPrioridade, indiceBST);
                    
                    atualizarFicheiro(indiceBST);
                    cout << "\n[SUCESSO] Dados atualizados e Fila de Prioridade reordenada!\n";
                } else cout << "\nEstudante nao encontrado.\n";
                break;
            }
            case 8: {
                int qtd; cout << "Quantidade de computadores a adicionar ao stock: "; cin >> qtd;
                if (qtd > 0) { stockComputadores += qtd; cout << "Stock atualizado para " << stockComputadores << " unidades.\n"; }
                break;
            }
            case 9: {
                float somaNotas = 0; int total = 0, vul = 0;
                indiceBST.obterEstatisticas(somaNotas, total, vul);
                cout << "\n============ PAINEL ESTATISTICO ============\n";
                cout << "Total de estudantes em espera: " << total << "\n";
                cout << "Media Academica Geral: " << (total > 0 ? (somaNotas / total) : 0) << "\n";
                cout << "Estudantes em Vulnerabilidade Social (Renda <= 3000 MT): " << vul << "\n";
                cout << "Computadores entregues nesta sessao: " << totalEntregues << "\n";
                cout << "============================================\n";
                break;
            }
            case 10: {
                int idRem; cout << "Digite o ID do estudante a remover do sistema: "; cin >> idRem;
                if (indiceBST.pesquisar(idRem) != nullptr) {
                    filaPrioridade.removerPorId(idRem);
                    indiceBST.remover(idRem);
                    atualizarFicheiro(indiceBST);
                    cout << "\nInscricao cancelada e removida com sucesso.\n";
                } else cout << "\nID nao encontrado.\n";
                break;
            }
            case 0: cout << "Encerrando...\n"; break;
            default: cout << "Opcao invalida!\n";
        }
    } while (opcao != 0);

    return 0;
}