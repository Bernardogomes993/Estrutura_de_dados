#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


//Depois diminuir o tamanho
#define TAMANHO 4
//Ponteiro de estrutura
typedef struct estruturaServicos *plista;
typedef struct estruturaServicoIndividual *pEstrutura;


//Sub Programa Para Remover Enter
void removerEnter(char str[]){
    for(int i = 0 ; str[i] != '\0' ; i++){
        if(str[i] == '\n'){
            str[i] = '\0';
        }
    }
}

//Sub Programa para Construir Estrutura
time_t construirEstrutura(int hora , int min , int segundo , int dia_semana , int mes , int ano){
    struct tm tm0 = {0};
    time_t t0_seg;

    tm0.tm_hour = hora ; tm0.tm_min = min; ; tm0.tm_sec = segundo;
    tm0.tm_mday = dia_semana ; tm0.tm_mon =  mes - 1 ; tm0.tm_year= ano - 1900;

    tm0.tm_isdst = -1;
   
    t0_seg = mktime(&tm0);
    return t0_seg;
}

//Converter Tempo
void converterTempo(time_t t0 , char str[]){
    
    strcpy (str , ctime(&t0)); 
  
}




//Estrutura para data
struct estruturaData{
    int dia , mes , ano;
    int hora , minuto , segundo;

};
typedef struct estruturaData Data;

//Estrutura para registar serviços individuais
struct estruturaServicoIndividual{
    char cliente[100];
    char equipamento[100];
    char observacoes[500];

    int codigoServicoID;
    int numComponentes;



    Data dataDeInicio;
    Data dataDeFim;
    Data dataDePagamento;

    float precoComponentes[50];
    int pago , concluido;

    pEstrutura prox;
};
typedef struct estruturaServicoIndividual servicoIndividual;

//Estrutura Para Tipos Serviços
struct estruturaServicos{

    //Nomes e equipamentos

    char funcionario[100];
    char tipoServico[100];

   

  
    float precoServico;

    //estados
    int cancelado , registado;

    //Ponteiros da estrutura
    pEstrutura execucao;
    pEstrutura concluidos;
    
};

typedef struct estruturaServicos Servico;



//Sub Programa para Fazer o registo
Servico registo(){
    Servico servTmp;
    int opcao;
    char funcionario[100] ,  nomeCliente[100] , equipamento[100];
  


    servTmp.cancelado = 0;
    servTmp.registado = 0;
    servTmp.execucao = NULL;
    servTmp.concluidos = NULL;



    printf("Selecione o tipo de Servico: 1 - Reparacao\t 2 - Diagnostico\t3 - Manutencao\t4-Limpeza\n");
    scanf("%d" , &opcao);
    getchar();

    switch (opcao)
    {
    case 1:
        //Tipo de Serviço
        strcpy(servTmp.tipoServico , "Reparacao");
       
        //Preço do Serviço
        servTmp.precoServico = 5; //Preco a hora
        servTmp.registado = 1;
        break;
    case 2:
        //Tipo de Serviço
        strcpy(servTmp.tipoServico , "Diagnostico");
         servTmp.registado = 1;

        //Preço do Serviço
        servTmp.precoServico = 20.0; // Preco Fixo
        break;
    case 3:
        //Tipo de Serviço
        strcpy(servTmp.tipoServico , "Manutencao");
         servTmp.registado = 1;

        //Preço do Serviço
        servTmp.precoServico = 5; // Preco a hora
        break;
    case 4:
        //Tipo de Serviço
        strcpy(servTmp.tipoServico , "Limpeza");
         servTmp.registado = 1;
        //Preço do Serviço
        servTmp.precoServico = 15.0; // Preco Fixo
        break;
    default:
        break;
    }

   

    //Funcionario Responsavel
    printf("Introduza o nome do funcionario responsavel:\n");
    fgets(funcionario , 100 , stdin);
    removerEnter(funcionario);
    strcpy(servTmp.funcionario , funcionario);


    return servTmp;

}

//Editar dados de um tipo de serviço
void editarServico(int totalServicos , Servico servicos[]){
   
    char str[100];
    char funcionario[100] ,  nomeCliente[100] , equipamento[100];
    printf("Qual o servico que deseja editar: ");
    fgets(str , 100 , stdin);
    removerEnter(str);

    for(int i = 0 ; i < totalServicos ; i++){
        if(strcasecmp(str , servicos[i].tipoServico) == 0){

        

            printf("Digite o nome do funcionario: \n");
            fgets(funcionario , 100 , stdin);
            removerEnter(funcionario);
            strcpy(servicos[i].funcionario , funcionario);

            printf("Digite o preco do servico: \n");
            scanf("%f" , &servicos[i].precoServico);
            
        }

    }
   
}

//Função auxiliar para eliminar as listas
void eliminarListas(pEstrutura *plst){
    pEstrutura atual = *plst;
    pEstrutura proximo;

    while(atual != NULL){
        proximo = atual -> prox;
        free(atual);
        atual = proximo;
    }

    *plst = NULL;
}

//Eliminar Tipo de Serviço
void eliminarTipoServico(int *totalServicos , Servico servicos[]){
    int index = 0;
    int encontrou = 0;
    char serv[100];
    printf("Digite o tipo de servico que deseja eliminar: ");
    fgets(serv , 100 , stdin);
    removerEnter(serv);

    for(int i = 0 ; i < (*totalServicos) ; i++){
        if(strcasecmp(servicos[i].tipoServico , serv) == 0){
            index = i;
            encontrou = 1;
        }
    }
    if(encontrou){
            eliminarListas(&servicos[index].execucao);
            eliminarListas(&servicos[index].concluidos);
            for(int j = index; j < (*totalServicos) ; j++){
                servicos[j] = servicos[j + 1];
            }
            (*totalServicos)--;
            printf("O tipo de servico: %s foi eliminado\n" , serv);
    }

    else{
        printf("Servico nao encontrado!\n");
    }




}

//Cancelar Tipo de Serviço
void cancelarTipoServico(int totalServicos , Servico servicos[]){
    char serv[100];
    printf("Digite o tipo de servico que deseja cancelar: ");
    fgets(serv , 100 , stdin);
    removerEnter(serv);

    
    for(int i = 0 ; i < totalServicos ; i++){
        if(strcasecmp(servicos[i].tipoServico , serv) == 0){
            servicos[i].cancelado = 1;
        }
    }

}


int listaVazia(pEstrutura plst){
    if(plst == NULL){
        return 1;
    }
    else{
        return 0;
    }
}

void escreveLista(pEstrutura plst){
    if(listaVazia(plst)){
        printf("A lista esta vazia!\n");
    }
    else{
     
            printf("[");
            do{
               
                       printf("Codigo Servico ID: %d " , plst ->codigoServicoID);
                        printf(" Nome do Cliente: %s " ,   plst ->cliente);
                        printf("Observacoes: %s " , plst -> observacoes);
                        printf("Pago: %d " , plst -> pago);
                
             
                plst = plst -> prox;
        }while(plst != NULL);
        printf("\b]\n");
        
     
    }
}




void juntarInicio(int totalServicos , Servico servicos[]){

    char nomeClienteAux[100] , equipamentoAux[100] ,  tipoServicoAux[100];
    int codigoAux , diaAux , mesAux , horaAux , minutoAux , segundoAux , anoAux;
    int flagRegisto = 0;

    if(totalServicos > 0){
           //Se for um pedir os dados.
        printf("Digite o tipo de servico: \n");
        fgets(tipoServicoAux , 100 , stdin);
        removerEnter(tipoServicoAux);

        for(int i = 0 ; i < totalServicos ; i++){

            if(servicos[i].registado == 1 && servicos[i].cancelado == 0 && strcasecmp(servicos[i].tipoServico , tipoServicoAux) == 0){

            
                printf("Digite o nome do cliente: ");
                fgets(nomeClienteAux , 100 , stdin);
                removerEnter(nomeClienteAux);

                printf("Digite o nome do equipamento: ");
                fgets(equipamentoAux , 100 , stdin);
                removerEnter(equipamentoAux);

    
                printf("Digite o codigo de servico: ");
                scanf("%d" , &codigoAux);
                getchar();

                printf("Insira a data de inicio no formato de inteiros:  (Dia Mes hh mm ss 2026) \n");
                scanf("%d %d %d %d %d %d" , &diaAux , &mesAux , &horaAux , &minutoAux , &segundoAux ,&anoAux);
                getchar();

                pEstrutura no = (pEstrutura)malloc(sizeof(servicoIndividual));

                if(no == NULL){
                    printf("Erro! Memoria nao disponivel!");
                }
                else{
                    no -> concluido = 0;
                    no -> pago = 0;
                    no -> numComponentes = 0;
                    strcpy(no ->cliente , nomeClienteAux);
                    strcpy(no -> equipamento , equipamentoAux);
                    no -> observacoes[0] = '\0';
                    no -> codigoServicoID = codigoAux;
                    no -> dataDeInicio.dia = diaAux;
                    no -> dataDeInicio.mes = mesAux;
                    no -> dataDeInicio.hora = horaAux;
                    no -> dataDeInicio.minuto = minutoAux;
                    no -> dataDeInicio.segundo = segundoAux;
                    no -> dataDeInicio.ano = anoAux;
                    no -> prox = servicos[i].execucao;
                    servicos[i].execucao = no;

                }
                escreveLista(servicos[i].execucao);
                return;
            }
           
        }
    }
    else{
        printf("Preencha primeiro os tipos de servico!\n");
    }
}

//Concluir um servico
void concluirServico(int totalServicos , Servico servicos[]){
    char tipoServicoAux[100];
    int encontrou = 0 , codigoAux  , indice;
    double diferenca;
    pEstrutura atual = NULL;
    pEstrutura anterior = NULL;
    printf("Digite o servico que deseja procura: \n");
    fgets(tipoServicoAux , 100 , stdin);
    removerEnter(tipoServicoAux);

    for(int i = 0 ; i < totalServicos ; i++){
        if(strcasecmp(servicos[i].tipoServico , tipoServicoAux) == 0 && servicos[i].cancelado == 0){
            atual = servicos[i].execucao;
            encontrou = 1;
            indice = i;
        }
    }
    if(encontrou){

        printf("Digite o codigo do servico: \n");
        scanf("%d" , &codigoAux);
        getchar();
        while(atual != NULL && codigoAux != atual -> codigoServicoID){
            anterior = atual;
            atual = atual -> prox;
        }

        if(atual == NULL){
            printf("Nao foi possivel encontrar o servico!\n");
         
        }
        else{
            
            printf("Insira a data de fim no formato de inteiros:  (Dia Mes hh mm ss 2026) \n");
            scanf("%d %d %d %d %d %d" , &atual ->  dataDeFim.dia , &atual -> dataDeFim.mes , &atual -> dataDeFim.hora , &atual -> dataDeFim.minuto , &atual -> dataDeFim.segundo , &atual -> dataDeFim.ano);
            getchar();

            time_t ti = construirEstrutura(atual -> dataDeInicio.hora , atual -> dataDeInicio.minuto , atual -> dataDeInicio.segundo , atual -> dataDeInicio.dia , atual -> dataDeInicio.mes , atual -> dataDeInicio.ano);
            time_t tf = construirEstrutura(atual -> dataDeFim.hora , atual -> dataDeFim.minuto , atual -> dataDeFim.segundo ,  atual -> dataDeFim.dia , atual -> dataDeFim.mes , atual -> dataDeFim.ano); 
            
            diferenca = difftime(tf , ti);

            if(diferenca > 0){
                atual -> concluido = 1;
                if(anterior == NULL){
             
                    servicos[indice].execucao = atual -> prox;

                }
                
                else{
                    anterior -> prox = atual -> prox;
                }

                
            printf("Data de Conclusao: %02d/%02d/%04d %02d:%02d:%02d ",
                    atual->dataDeFim.dia,
                    atual->dataDeFim.mes,
                    atual->dataDeFim.ano,
                    atual->dataDeFim.hora,
                    atual->dataDeFim.minuto,
                    atual->dataDeFim.segundo);

                atual -> prox = servicos[indice].concluidos;
                servicos[indice].concluidos = atual;
                atual -> concluido = 1; 
                printf("Servico concluido com sucesso!\n");

                
                
            }

            else{ 
                printf("A data de conclusao nao pode ser menor que a data de inicio\n");
            }
          
        }
     
    
    }

    else{
        printf("Nao e possivel concluir o  servico!\n");
    }
   

}

//Eliminar Um Serviço
void eliminarServico(int totalServicos , Servico servicos[]){
    char tipoServicoAux[100];
    int indice , encontrou = 0 , codigoAux;
    pEstrutura atual = NULL;
    pEstrutura anterior = NULL;
    printf("Digite o tipo de servico que pretende eliminar: \n");
    fgets(tipoServicoAux , 100 , stdin);
    removerEnter(tipoServicoAux);

    for(int i = 0 ; i < totalServicos ; i++){
        if(strcasecmp(servicos[i].tipoServico , tipoServicoAux) == 0 && servicos[i].cancelado == 0){
            indice = i;
            atual = servicos[i].execucao;
            encontrou = 1;
        }
    }

    if(encontrou){
        printf("Digite o codigo do servico: \n");
        scanf("%d" , &codigoAux);
        getchar();

        while(atual != NULL && codigoAux != atual -> codigoServicoID){
            anterior = atual;
            atual = atual -> prox;
        }

        if(atual == NULL){
            printf("Nao foi possivel encontrar o servico!\n");
        }

        else{
            if(anterior == NULL){
                servicos[indice].execucao = atual -> prox;
            }
            else{
                anterior -> prox = atual -> prox;
            }

            free(atual);
        }
    }
    else{
        printf("Servico nao encontrado!\n");
    }
    escreveLista(servicos[indice].execucao);

}

//Editar Componentes 
void editarComponentes(int totalServicos , Servico servicos[]){
    char tipoServicoAux[100] ,  nomeComp[100];
    int encontrou = 0  , indice = - 1 , codigoAux , editavel = 0;
    float valor;
    pEstrutura atual = NULL;
    pEstrutura anterior = NULL;

    printf("Digite o tipo de servico que deseja editar: ");
    fgets(tipoServicoAux , 100 , stdin);
    removerEnter(tipoServicoAux);

    if(strcasecmp(tipoServicoAux , "Reparacao") != 0 && strcasecmp(tipoServicoAux , "Manutencao") != 0){
        printf("Este servico nao permite editar componentes!\n");
        return;
    }

    for(int i = 0 ; i <totalServicos ; i++){
        if(strcasecmp(tipoServicoAux , servicos[i].tipoServico) == 0){
            atual = servicos[i].execucao;
            indice = i;
            encontrou = 1;
        }
    }

    if(!encontrou){
        printf("Tipo de servico nao encontrado!\n");
        return;
    }

    printf("Digite o numero de servico que deseja editar: \n");
    scanf("%d" , &codigoAux);
    getchar();

    while(atual != NULL && codigoAux != atual -> codigoServicoID){
        atual = atual -> prox;
    }

    if(atual == NULL){
        printf("Nao e possivel encontrar o servico\n");
        return;
    }

    if(atual -> concluido == 1 || atual -> pago == 1){
        printf("Nao e possivel editar o servico\n");
        return;
    }

    if(atual -> numComponentes >= 50){
        printf("Numero máximo de componentes atingido!\n");
        return;
    }

    printf("Nome do componente substituido: \n");
    fgets(nomeComp , 100 , stdin);
    removerEnter(nomeComp);

    printf("Coloque o preco do componente: \n");
    scanf("%f" , &valor);
    getchar();

    atual -> precoComponentes[atual -> numComponentes] = valor;
    atual -> numComponentes++;

    if(atual ->observacoes[0] == '\0'){
        atual ->observacoes[0] = '\0';
    }

    char aux[100];
    sprintf(aux , "%s(%.2f)" , nomeComp , valor);
    strcat(atual->observacoes , aux);

    escreveLista(servicos[indice]. execucao);

}

//Calcular o valor a pagar
float valorPagar(int totalServicos , Servico servicos[]){
    char tipoServicoAux[100] ,  nomeComp[100];
    int encontrou = 0  , indice , codigoAux , editavel = 0;
    float valor;
     float precoServicoAux ;
    pEstrutura atual = NULL;
    pEstrutura anterior = NULL;
    printf("Digite o tipo de servico que deseja procurar: ");
    fgets(tipoServicoAux , 100 , stdin);
    removerEnter(tipoServicoAux);

    for(int i = 0 ; i < totalServicos ; i++){
        if(strcasecmp(servicos[i].tipoServico , tipoServicoAux) == 0){
            atual = servicos[i].execucao;
            encontrou = 1;
            indice = i;
        }
    }
    if(encontrou){
        printf("Digite o numero do servico para obter o valor a pagar: \n");
        scanf("%d" , &codigoAux);
        getchar();

        atual = servicos[indice].execucao;
        while(atual != NULL && atual -> codigoServicoID != codigoAux){
            atual = atual -> prox;

        }

        if(atual == NULL){
            atual = servicos[indice].concluidos;

            while(atual != NULL && atual -> codigoServicoID != codigoAux){
                atual = atual -> prox;
            }

            if(atual == NULL){
                printf("Nao foi possivel encontrar o servico!\n");
                return 0.0;
            }
            
        }
        float somaComponentes = 0.0;
        for(int i = 0 ; i < atual -> numComponentes ; i++){
            somaComponentes = somaComponentes + atual -> precoComponentes[i];
        }

        if(strcasecmp(servicos[indice].tipoServico ,"Reparacao") == 0 ||  strcasecmp(servicos[indice].tipoServico , "Manutencao") == 0){
            time_t ti = construirEstrutura(atual -> dataDeInicio.hora , atual -> dataDeInicio.minuto , atual -> dataDeInicio.segundo , atual -> dataDeInicio.dia , atual -> dataDeInicio.mes , atual -> dataDeInicio.ano);

            time_t tf;

            if(atual -> concluido == 1){
                tf = construirEstrutura(atual -> dataDeFim.hora , atual -> dataDeFim.minuto , atual -> dataDeFim.segundo , atual -> dataDeFim.dia , atual -> dataDeFim.mes , atual -> dataDeFim.ano);


            }

            else{
                tf = time(NULL);
            }

            double segundos = difftime(tf , ti);
            if(segundos < 0){
                segundos = 0;
            }
            precoServicoAux =   (((float)segundos / 3600.0) * servicos[indice].precoServico) + somaComponentes;

        }
        else{
            precoServicoAux = servicos[indice].precoServico;
        }


    }

    else{
        printf("Servico nao encontrado !\n");
    }

    return precoServicoAux;
}




//Listar quanto tempo estão a decorrer os serviços
void tempoDecorrido(int totalServicos , Servico servicos[]){
    char tipoServicoAux[100];
    int encontrou = 0 , indice;
    printf("Digite o servico que deseja procurar: ");
    fgets(tipoServicoAux , 100 , stdin);
    removerEnter(tipoServicoAux);


    for(int i = 0 ; i < totalServicos ; i++){
        if(strcasecmp(servicos[i].tipoServico , tipoServicoAux) == 0 && servicos[i].cancelado == 0){
            encontrou = 1;
            indice = i;
        }
    }

    if(encontrou){
        pEstrutura atual = servicos[indice].execucao;
         if(atual == NULL){
            printf("Nao e possivel encontrar servicos em execucao\n");
            return;
         }
         else{
            while(atual != NULL){
                time_t ti = construirEstrutura(atual -> dataDeInicio.hora , atual -> dataDeInicio.minuto , atual -> dataDeInicio.segundo , atual -> dataDeInicio.dia , atual ->dataDeInicio.mes , atual ->dataDeInicio.ano);


                time_t agora = time(NULL);

                //Tempo em segundos
                double diff = difftime(agora , ti);

                if(diff < 0 ){
                    diff = 0;
                }

                
                int horas = diff / 3600;
                int minutos = ((int)diff % 3600) / 60;
                int segundos = (int)diff % 60;
                

                printf("\nServico ID: %d\n" , atual -> codigoServicoID);
                printf("\nCliente: %s" , atual -> cliente);
                printf("\nTempo decorridos: %02d:%02d:%02d\n" , horas , minutos , segundos);

                atual = atual -> prox;

            }
         }
    }
    else{
        printf("Servico nao encontrado!\n");
    }

} 

//Contabilizar o numero de servicos a decorrer

void numeroServicos_aDecorrer(int totalServicos , Servico servicos[]){
    char tipoServicoAux[100];
    printf("Qual o tipo de servico: \n");
    fgets(tipoServicoAux , 100 , stdin);
    removerEnter(tipoServicoAux);

    int contador = 0;

    for(int i = 0 ; i < totalServicos ; i++){
        if(strcasecmp(servicos[i].tipoServico , tipoServicoAux) == 0){
            pEstrutura atual = servicos[i].execucao;
            while(atual != NULL){
             
                contador++;

                atual = atual -> prox;  
            }
        }  
        
    }
    printf("Para o servico: %s\n" , tipoServicoAux);
    printf("Estao a decorrer: %d\n" , contador);

}


//Listar os servicos concluidos

void listarConcluidos(int totalServicos , Servico servicos[]){
   
    for(int  i = 0 ; i < totalServicos ; i++){
        pEstrutura atual = servicos[i].concluidos;

        while(atual != NULL){

                printf("\nCliente: %s" , atual -> cliente);
                printf("\nServicoID: %d" , atual -> codigoServicoID);
                printf("\nObservacoes: %s" , atual -> observacoes);
                printf("\nPago: %d\n" , atual -> pago);    
                printf("\nData de Conclusao: %02d/%02d/%04d %02d:%02d:%02d\n",
                   atual->dataDeFim.dia,
                   atual->dataDeFim.mes,
                   atual->dataDeFim.ano,
                   atual->dataDeFim.hora,
                   atual->dataDeFim.minuto,
                   atual->dataDeFim.segundo);

                atual = atual -> prox;
            }
        }
         
    }

 //Numero de Servicos Concluidos
 
 int totalServicosConcluidos(int totalServicos , Servico servicos[]){
    int contador = 0;
    for(int i = 0 ; i < totalServicos ; i++){
        pEstrutura atual = servicos[i].concluidos;

        while(atual != NULL){
            contador++;
            atual = atual -> prox;
        }
    }
    return contador;
 }

//Calcular Preco de um servico
float precoServicoConcluido(Servico tipo , pEstrutura atual){
    float somaPreco = 0;
    for(int i = 0 ; i < atual -> numComponentes ; i++){
       somaPreco += atual -> precoComponentes[i];
    }

    if(strcasecmp(tipo.tipoServico , "Reparacao") == 0 || strcasecmp(tipo.tipoServico , "Manutencao") == 0){

        time_t ti = construirEstrutura(atual -> dataDeInicio.hora , atual -> dataDeInicio.minuto , atual -> dataDeInicio.segundo , atual -> dataDeInicio.dia , atual -> dataDeInicio.mes , atual -> dataDeInicio.ano);

        time_t tf = construirEstrutura(atual -> dataDeFim.hora , atual -> dataDeFim.minuto , atual -> dataDeFim.segundo , atual ->dataDeFim.dia , atual -> dataDeFim.mes , atual -> dataDeFim.ano);

        double segundos = difftime(tf , ti);


        if(segundos < 0 ){
            segundos = 0;
        }
        somaPreco += (segundos / 3600.0) * tipo.precoServico;
    }
    else{
        somaPreco += tipo.precoServico;
    }
    return somaPreco;
}

//Calculra a media dos precos dos servicos concluidos
float calcularMedia_precosServicos_Concluidos(int totalServicos , Servico servicos[] , int servicosConcluidos){
    float precoTotal = 0 ;
    for(int i = 0 ; i < totalServicos ; i++){
        pEstrutura atual = servicos[i].concluidos;

        while(atual != NULL){
            precoTotal += precoServicoConcluido(servicos[i] , atual);
            atual = atual -> prox;
        }

    }

    if(servicosConcluidos == 0){
        return 0;
    }
    else{
        return precoTotal / servicosConcluidos;
    }

}


//Para um serviço particular Calcular a duração mínima, a duração máxima e a duração média dos serviços concluídos 

void duracoesServicoConcluido(int totalServicos , Servico servicos[]){
    char tipoServicoAux[100]; 
    double maior = 0;
    double menor = 999999999;
    double media, soma = 0;
    int contador = 0;
  
    printf("Digite a tipo de servico que deseja: \n");
    fgets(tipoServicoAux , 100 , stdin);
    removerEnter(tipoServicoAux);

    pEstrutura atual = NULL;

    for(int i = 0 ; i < totalServicos ; i++){

        if(strcasecmp(servicos[i].tipoServico , tipoServicoAux) == 0){
            atual = servicos[i].concluidos;

            while(atual != NULL){
                contador++;

                time_t ti = construirEstrutura(atual -> dataDeInicio.hora , atual->dataDeInicio.minuto , atual -> dataDeInicio.segundo , atual -> dataDeInicio.dia , atual -> dataDeInicio.mes , atual -> dataDeInicio.ano);

                time_t tf = construirEstrutura(atual -> dataDeFim.hora , atual -> dataDeFim.minuto , atual -> dataDeFim.segundo , atual -> dataDeFim.dia , atual -> dataDeFim.mes , atual -> dataDeFim.ano);

                double tempoFinal = difftime(tf , ti);
                soma = soma + tempoFinal;

                if(tempoFinal > maior){
                    maior = tempoFinal;
                }

                if(tempoFinal < menor){
                    menor = tempoFinal;
                }


                atual = atual -> prox;
            }
        }

    }

    if(contador <= 0){
       media = 0;
  
    }
    else{
        media = (soma) / (double)(contador);
        printf("Maior tempo: %.2f\n" , maior);
        printf("Menor tempo: %.2f\n" , menor);
        printf("Media: %.2f\n" , media);


    }
}





//Ordenar Servicos Concluidos Por Ordem decrescente do valor pago
void ordenarServicosConcluidos(Servico *tipo){
    if(tipo -> concluidos == NULL){
        printf("Nao e possivel encontrar o servico\n");
        return;
    }

    pEstrutura i , j;

    for(i = tipo->concluidos ; i != NULL ; i = i -> prox){
        for(j = i -> prox ; j != NULL ; j = j -> prox){
            float valor_i = precoServicoConcluido(*tipo , i);
            float valor_j = precoServicoConcluido(*tipo , j);

            if(valor_i < valor_j){
              
                pEstrutura auxProx_i = i -> prox;
                pEstrutura auxProx_j = j -> prox;

             
                servicoIndividual tmp = *i;
                *i = *j;
                *j = tmp;

                i -> prox = auxProx_i;
                j -> prox = auxProx_j;

              

            }
        }
    }

}
//Duração média , máxima e minima de todos os serviços
void duracaoDeTodosServicos(int totalServicos , Servico servicos[]){
    double maior = 0;
    double menor = 9999999999;
    int totalServicosConcluidosAux = totalServicosConcluidos(totalServicos , servicos); 
    double media , contador = 0;
    pEstrutura pAtual = NULL;
    pEstrutura pAuxMaior = NULL;
    pEstrutura pAuxMenor = NULL;

    for(int i = 0 ; i < totalServicos ; i++){
        pAtual = servicos[i].concluidos;

        while(pAtual != NULL){

            time_t ti = construirEstrutura(pAtual -> dataDeInicio.hora , pAtual -> dataDeInicio.minuto , pAtual -> dataDeInicio.segundo , pAtual ->dataDeInicio.dia , pAtual -> dataDeInicio.mes , pAtual -> dataDeInicio.ano);

            time_t tf = construirEstrutura(pAtual -> dataDeFim.hora , pAtual -> dataDeFim.minuto , pAtual -> dataDeFim.segundo , pAtual -> dataDeFim.dia , pAtual -> dataDeFim.mes , pAtual -> dataDeFim.ano);

            double tempoFinal = difftime(tf , ti);
            contador = contador + tempoFinal;
            if(tempoFinal > maior){
                maior = tempoFinal;
                pAuxMaior = pAtual;
            }

            if(tempoFinal < menor){

                menor = tempoFinal;
                pAuxMenor = pAtual;
            }
            pAtual = pAtual -> prox;
        }

    }

    if(totalServicosConcluidosAux == 0){
        printf("Nao existem servicos concluidos");
        return;

    }

    else{
        media = contador /(double)(totalServicosConcluidosAux);
    }
    printf("Maior duracao: \n" );
    printf("Codigo Servico: %d\n" , pAuxMaior -> codigoServicoID);
    printf("Nome Cliente: %s\n" , pAuxMaior -> cliente);
    printf("Equipamento: %s\n" , pAuxMaior ->equipamento);
    printf("Valor: %.2f\n" , maior);

    printf("Menor Duracao: \n" );
    printf("Codigo Servico: %d\n" , pAuxMenor -> codigoServicoID);
    printf("Nome Cliente: %s\n" , pAuxMenor -> cliente);
    printf("Equipamento: %s\n" , pAuxMenor ->equipamento);
    printf("Valor: %.2f\n" , menor);


    printf("Media de duracao: %.2f\n" , media);

}

//Funcao Recursiva para contar nós
int contarNos(pEstrutura atual){
    if(atual == NULL){
        return 0;
    }

    return 1 +  contarNos(atual -> prox);

}


//Listar o numero de servicos cancelados , e indicar o numero de servicos realizados
void numeroServicosCancelados(int totalServicos , Servico servicos[]){

   
    pEstrutura pAtual2 = NULL;
    
    int contadorServicosRealizados = 0;

    for(int i = 0 ; i < totalServicos ; i++){

        if(servicos[i].cancelado == 1){
            pAtual2 = servicos[i].concluidos;
            printf("Tipo Servico: %s\n" , servicos[i].tipoServico);
            printf("Responsavel: %s\n" , servicos[i].funcionario);
            printf("Preco Servico: %.2f\n" , servicos[i].precoServico);

            contadorServicosRealizados += contarNos(pAtual2);
            printf("Numero de servicos realizados: %d\n" , contadorServicosRealizados);
        }
    }
   
  

}

//Obter numero de servicos a decorrer 
int numeroServicosDecorrer(Servico servico){
    pEstrutura atual = servico.execucao;
    int contadorServico = 0;

    contadorServico += contarNos(atual);

    return contadorServico;

}


//Ordenar os tipos de serviços por ordem decrescente do número de serviços a decorrer. (Selection Sort)
void ordenarDecrescenteTipos(int totalServicos , Servico servicos[]){
    
    int indiceMaximo;
    Servico tmp;
    for(int i = 0 ; i < totalServicos-1 ; i++){
            indiceMaximo = i;
        for(int j = i + 1 ; j < totalServicos ; j++){
            if(numeroServicosDecorrer(servicos[j]) > numeroServicosDecorrer(servicos[i])){
                indiceMaximo = j;
            }
        }

        if(indiceMaximo != i){
            tmp = servicos[i];
            servicos[i] = servicos[indiceMaximo];
            servicos[indiceMaximo] = tmp;
        }
    }

}

//Registar Pagamento
void registarPagamento(int totalServicos , Servico servicos[]){
    char tipoServicoAux[100];
    int encontrou = 0;
    int codigoAux;
    pEstrutura pAtual = NULL;
    printf("Digite o tipo de servico que deseja pagar: \n");
    fgets(tipoServicoAux , 100 , stdin);
    removerEnter(tipoServicoAux);

    for(int i=0 ; i < totalServicos ; i++){
        if(strcasecmp(servicos[i].tipoServico , tipoServicoAux) == 0){
            pAtual = servicos[i].concluidos;
            encontrou = 1;
        }
    }

    if(!encontrou){
        printf("O servico não foi encontrado!\n");
        return;
    }
    else{
        printf("Qual o codigo do servico: \n");
        scanf("%d" , &codigoAux);
        getchar();

        while(pAtual != NULL && pAtual -> codigoServicoID != codigoAux){

            pAtual = pAtual -> prox;
        }

        if(pAtual == NULL){
            printf("O servico nao foi encontrado!\n");
            return;
        }

        else{
            if(pAtual -> pago == 1){
                printf("O servico ja esta pago!\n");
                return;
            }
            else{
                int diaAux , mesAux , horaAux , minutoAux , segundoAux , anoAux;
                printf("Insira a data de pagamento (Dia Mes Hora mm ss ano): \n");
                scanf("%d %d %d %d %d %d" , &diaAux , &mesAux , &horaAux , &minutoAux , &segundoAux , &anoAux);

                time_t tConclusao = construirEstrutura(pAtual -> dataDeFim.hora , pAtual -> dataDeFim.minuto , pAtual -> dataDeFim.segundo , pAtual -> dataDeFim.dia , pAtual -> dataDeFim.mes , pAtual -> dataDeFim.ano);

                time_t tPagamento = construirEstrutura(horaAux , minutoAux , segundoAux , diaAux , mesAux , anoAux);

                if(difftime(tPagamento , tConclusao) < 0){
                    printf("Operacao Cancelada, a data de pagamento nao pode ser anterior a data de conclusao!\n");
                }

                else{
                    pAtual -> pago = 1;
                    pAtual -> dataDePagamento.dia = diaAux;
                    pAtual -> dataDePagamento.mes = mesAux;
                    pAtual -> dataDePagamento.hora = horaAux;
                    pAtual -> dataDePagamento.minuto = minutoAux;
                    pAtual -> dataDePagamento.segundo = segundoAux;
                    pAtual -> dataDePagamento.ano = anoAux;
                }
            }
        }

    }
}

//Guardar Tabela

void guardarDaddos(int totalServicos , Servico servicos[]){
    FILE *f = fopen("dados.dat" , "wb");

    if(f == NULL){
        printf("Erro ao abrir o ficheiro!");
        return;
    }

    fwrite(&totalServicos , sizeof(int) , 1 , f);

    for(int i = 0 ; i < totalServicos ; i++){

        fwrite(&servicos[i] , sizeof(Servico) , 1 , f);

        pEstrutura atual = servicos[i].execucao;

        while(atual != NULL){
            fwrite(atual , sizeof(servicoIndividual) , 1 , f);

            atual = atual -> prox;
        }

        servicoIndividual marcadorFim = {marcadorFim.codigoServicoID = -1};
        fwrite(&marcadorFim , sizeof(servicoIndividual) , 1 , f);

        atual = servicos[i].concluidos;

        while(atual != NULL){
            fwrite(atual , sizeof(servicoIndividual) , 1 , f);
            atual = atual -> prox;
        }

        fwrite(&marcadorFim , sizeof(servicoIndividual) , 1 , f);


    }

    fclose(f);
    printf("Servicos Guardados!\n");


}


//Carregar Daddos
int carregarDados(Servico servicos[]){
    FILE *f = fopen("dados.dat" , "rb");

    if(f == NULL){
        printf("Nao foi possivel abrir o ficheiro\n");
        return 0;
    }
    int totalServicos = 0;

    fread(&totalServicos , sizeof(int) , 1 , f);

    for(int i = 0 ; i<totalServicos ; i++){
        fread(&servicos[i] , sizeof(Servico) , 1 , f);

        servicos[i].execucao = NULL;
        servicos[i].concluidos = NULL;

        servicoIndividual aux;
        pEstrutura pUltimo = NULL;

        fread(&aux , sizeof(servicoIndividual) , 1 , f);

        while(aux.codigoServicoID != -1){
           

            pEstrutura novo = (pEstrutura)malloc(sizeof(servicoIndividual));

            if(novo != NULL){
                *novo = aux;
                novo -> prox = NULL;
                
                if(servicos[i].execucao == NULL){
                    servicos[i].execucao = novo;
                }
                else{
                    pUltimo -> prox = novo;
                }

                pUltimo = novo;
            }

            fread(&aux , sizeof(servicoIndividual) , 1 , f);


        }

        fread(&aux , sizeof(servicoIndividual) , 1, f);
        pUltimo = NULL;

        while(aux.codigoServicoID != -1){
           

            pEstrutura novoConcluidos = (pEstrutura)malloc(sizeof(servicoIndividual));

            if(novoConcluidos != NULL){
                *novoConcluidos = aux;
                novoConcluidos -> prox = NULL;
              

                if(servicos[i].concluidos == NULL){
                    servicos[i].concluidos = novoConcluidos;
                }
                else{
                    pUltimo -> prox = novoConcluidos;
                }

                pUltimo = novoConcluidos;
            }

            fread(&aux , sizeof(servicoIndividual) , 1, f);

        }

    }

    fclose(f);
    printf("Servicos carregados com sucesso!\n");
    return totalServicos;


}




int main(){
    
    //Variaveis para opcao , codigo e ID
    int opcao ,   codigo;
    Servico servicos[TAMANHO];
    int totalServicos = carregarDados(servicos);
    

  
    
     //Inicialização de do while
    printf("Selecione 1 para continuar 0 para Sair\n");
    scanf("%d" , &opcao);

    do{
        printf("\t2 -  Registar Tipo de Servico\n");
        printf("\t3 -  Apresentar tabela\n");
        printf("\t4 - Editar Dados de um tipo de servico\n");
        printf("\t5 - Eliminar Tipo de Servico\n");
        printf("\t6 - Cancelar um servico\n");
        printf("\t7 - Adicionar Servico\n");
        printf("\t8 - Servico Concluido\n");
        printf("\t9 - Eliminar um Servico\n");
        printf("\t10 - Editar compnentes de um servico\n");
        printf("\t11 - Obter o valor a pagar por um servico\n");
        printf("\t12 -  Listar os servicos servicos a decorrer e quanto tempo\n");
        printf("\t13 - Numero de servicos a decorrer\n");
        printf("\t14 - Listar o numero de servicos concluidos\n");
        printf("\t15 - Media dos precos dos servicos concluidos\n");
        printf("\t16 - Ordenar em ordem decrescente o valor pago de servicos concluidos\n");
        printf("\t17 - Para um servico particular, calcular a duracao maxima , minima e media dos servicos concluidos\n");
        printf("\t18 - Duracao maxima, minima e medias de todos os servicos concluidos\n");
        printf("\t19 - Listar tipos de servicos cancelados, numero de servicos realizados\n");
        printf("\t20 - Ordenar os tipos de servico por ordem decrescente do numero de servicos a decorrer\n");
        printf("\t21 - Registar Pagamento\n");

        scanf("%d" , &opcao);
        getchar();
        switch (opcao)
        {
        case 2:
                    
               if(totalServicos < TAMANHO){
                
                    Servico novo = registo();
                    int existe = 0;
                   
                                         
                    for(int i = 0 ; i < totalServicos ; i++){
                        if(strcasecmp(servicos[i].tipoServico , novo.tipoServico) == 0){
                                existe = 1; 
                        }
                       
                              
                    }

                    if(!existe){
                         servicos[totalServicos] = novo;
                      
                         totalServicos++;
                    }
                    else{
                        printf("O servico ja existe!\n");
                    }
                  
                }
                else{
                    printf("Memoria cheia!\n");
                }
            break;
        case 3:
                if(totalServicos > 0){
                    for(int i = 0 ; i < totalServicos ; i++){
                       
                        printf("Funcionario: %s\n" , servicos[i].funcionario);
                        printf("Preco do Servico: %.2f\n" , servicos[i].precoServico);
                        escreveLista(servicos[i].execucao);
                        escreveLista(servicos[i].concluidos);
                      
                    }
                }
                else{
                    printf("A tabela esta vazia!\n");
                }
              
            break;
        case 4:
              editarServico(totalServicos , servicos);
             break;
        case 5:
                eliminarTipoServico(&totalServicos , servicos);
                break;
        case 6:
            cancelarTipoServico(totalServicos , servicos);
             break;
        case 7:
            
            juntarInicio(totalServicos , servicos);
            break;
        case 8:
            concluirServico(totalServicos , servicos);
            break;
        case 9:
                eliminarServico(totalServicos , servicos);
                break;
        case 10:
                editarComponentes(totalServicos , servicos);
                //Verificar o parentesis quando imprimo
                break;
        case 11:
                float valorPagarAux = valorPagar(totalServicos , servicos);
                printf("Valor a pagar: %.2f\n" , valorPagarAux);
                break;
        case 12:
                tempoDecorrido(totalServicos , servicos);
                break;

        case 13:
                numeroServicos_aDecorrer(totalServicos , servicos);
             
                break;
        case 14:
              listarConcluidos(totalServicos , servicos);
              break;
        case 15:

            int totalServicosConcluidosAux = totalServicosConcluidos(totalServicos , servicos);

            float mediaAux = calcularMedia_precosServicos_Concluidos(totalServicos , servicos , totalServicosConcluidosAux);

             printf("Media dos servicos concluidos: %.2f\n" , mediaAux);
             break;
        case 16:
            char tipoAux[100];
            printf("Digite o tipo de servico: \n");
            fgets(tipoAux , 100 , stdin);
            removerEnter(tipoAux);

            for(int i = 0 ; i < totalServicos ; i++){
                if(strcasecmp(tipoAux , servicos[i].tipoServico) == 0){
                    ordenarServicosConcluidos(&servicos[i]);
                }
            }
            listarConcluidos(totalServicos , servicos);
            break;
        case 17:
            duracoesServicoConcluido(totalServicos , servicos);
            break;
            break;
        case 18:
            duracaoDeTodosServicos(totalServicos ,servicos);
            break;
        case 19:
            numeroServicosCancelados(totalServicos , servicos);
            break;
        case 20:
            ordenarDecrescenteTipos(totalServicos , servicos);
            break;
        case 21:
            registarPagamento(totalServicos , servicos);

        default:
            break;
        }
    }while(opcao != 0);

    guardarDaddos(totalServicos , servicos);

   
}