#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define TAMANHO 100


/*Adicionar se foi pago e a data de pagamento , depois de concluido*/

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

//Estrutura Para Serviços
struct estruturaServicos{

    //Nomes e equipamentos
    char cliente[100];
    char funcionario[100];
    char equipamento[100];
    char tipoServico[100];
    char observacoes[500];
  

    //Datas
    Data dataDeInicio;
    Data dataDeFim;
    Data dataDePagamento;


    //Codigos
    int codigoID;
    int arquivado;

    int numComponentes;
    float precoServico , precoComponentes[10]; 

    //estados
    int pago , concluido;
    
};

typedef struct estruturaServicos Servico;

void setarCabecalhos(Servico servico) {
    FILE *f;
    char buffer[100];
    sprintf(buffer, "%s.csv", servico.tipoServico);

    f = fopen(buffer , "r");
    if(f == NULL){
        f = fopen(buffer , "w");
        if(f != NULL){
            fprintf(f , "Codigo; Data de Inicio; Data de Fim; Cliente\n");
            fclose(f);
        }
    }

    else{
        fclose(f);
    }
   
}

//Sub Programa para Fazer o registo
Servico registo(int proximoID){
    Servico servTmp;
    int opcao;
    char funcionario[100] ,  nomeCliente[100] , equipamento[100];
    servTmp.numComponentes = 0;
    servTmp.pago = 0;
    servTmp.concluido = 0;
    servTmp.arquivado = 0;
  
    servTmp.codigoID = proximoID;
    strcpy(servTmp.observacoes , "");

    printf("Selecione o tipo de Servico: 1 - Reparacao\t 2 - Diagnostico\t3 - Manutencao\t4-Limpeza\n");
    scanf("%d" , &opcao);

    switch (opcao)
    {
    case 1:
        //Tipo de Serviço
        strcpy(servTmp.tipoServico , "Reparacao");
       
        //Preço do Serviço
        servTmp.precoServico = 5; //Preco a hora
        break;
    case 2:
        //Tipo de Serviço
        strcpy(servTmp.tipoServico , "Diagnostico");

        //Preço do Serviço
        servTmp.precoServico = 20.0; // Preco Fixo
        break;
    case 3:
        //Tipo de Serviço
        strcpy(servTmp.tipoServico , "Manutencao");

        //Preço do Serviço
        servTmp.precoServico = 5; // Preco a hora
        break;
    case 4:
        //Tipo de Serviço
        strcpy(servTmp.tipoServico , "Limpeza");

        //Preço do Serviço
        servTmp.precoServico = 15.0; // Preco Fixo
        break;
    default:
        break;
    }

    //Inserir a data de inicio 
    printf("Insira a data de inicio no formato de inteiros:  (Dia Mes hh mm ss 2026) \n");
    scanf("%d %d %d %d %d %d" , &servTmp.dataDeInicio.dia , &servTmp.dataDeInicio.mes , &servTmp.dataDeInicio.hora , &servTmp.dataDeInicio.minuto , &servTmp.dataDeInicio.segundo , &servTmp.dataDeInicio.ano);
    getchar();

    //Inserir o nome do cliente
    printf("Insira o nome do cliente: \n");
    fgets(nomeCliente , 100 , stdin);
    removerEnter(nomeCliente);
    strcpy(servTmp.cliente , nomeCliente);

    //Inserir o nome do equipamento
    printf("Insira o nome do equipamento: \n");
    fgets(equipamento , 100 , stdin);
    removerEnter(equipamento);
    strcpy(servTmp.equipamento , equipamento);

    //Funcionario Responsavel
    printf("Introduza o nome do funcionario responsavel:\n");
    fgets(funcionario , 100 , stdin);
    removerEnter(funcionario);
    strcpy(servTmp.funcionario , funcionario);


    return servTmp;

}


//Sub Programa Para Listar o Ficheiro (Debbug)
void listarFicheiro(){
    FILE *f;
    Servico s;
    f = fopen("Servicos.dat" , "rb");
    if(f == NULL){
        printf("Nao e possivel abrir o ficheiro\n");
    }
    else{
        while(fread(&s , sizeof(Servico) , 1 , f) == 1){
          
            printf("Codigo: %d\tCliente: %s\tServico: %s\tPreco do Servico: %.2f\n" , s.codigoID , s.cliente , s.tipoServico , s.precoServico);
            printf("Concluido: %d\n" , s.concluido);
            for(int i = 0 ; i < s.numComponentes ; i++){
                printf("Preco de Componentes: %.2f\n" , s.precoComponentes[i]);
            }
            printf("Observacoes: %s\n" , s.observacoes);
           
        }
    }
    fclose(f);
}


//Sub Programa para reescrever no ficheiro
void guardarTabelaUnica(Servico ativos[], int totalAtivos , Servico concluidos[] , int totalConluidos){
    FILE *f;
    f = fopen("Servicos.dat" , "wb");
    if(f != NULL){
       
        fwrite(ativos , sizeof(Servico) , totalAtivos , f);
        fwrite(concluidos , sizeof(Servico) , totalConluidos , f);
       

       fclose(f);
    }
  
}


//Sub Programa de Concluido
void servicoConcluido(Servico servicos[] , int total , int codigo){
    time_t ti;
    time_t tf;
    double diferenca;
    //Logica das datas
    for(int i = 0 ; i < total ; i++){

        if(servicos[i].codigoID == codigo){
            if(servicos[i].concluido == 1){
                printf("Valor ja atribuido!\n");   
            }

            else{
                    printf("Insira a data de fim no formato de inteiros:  (Dia Mes hh mm ss 2026) \n");
                    scanf("%d %d %d %d %d %d" , &servicos[i].dataDeFim.dia , &servicos[i].dataDeFim.mes , &servicos[i].dataDeFim.hora , &servicos[i].dataDeFim.minuto , &servicos[i].dataDeFim.segundo , &servicos[i].dataDeFim.ano);
                    getchar();

                    time_t ti = construirEstrutura(servicos[i].dataDeInicio.hora , servicos[i].dataDeInicio.minuto , servicos[i].dataDeInicio.segundo , servicos[i].dataDeInicio.dia , servicos[i].dataDeInicio.mes , servicos[i].dataDeInicio.ano);
                    time_t tf = construirEstrutura(servicos[i].dataDeFim.hora , servicos[i].dataDeFim.minuto , servicos[i].dataDeFim.segundo , servicos[i].dataDeFim.dia , servicos[i].dataDeFim.mes , servicos[i].dataDeFim.ano);

                    diferenca = difftime(tf , ti);

                    if(diferenca > 0){
                        servicos[i].concluido = 1;
                        printf("Servico Concluido Com Sucesso!\n");
                    }
                    else{
                        printf("Coloque a data de fim superior a data de inicio\n");
                    }
            }
        }
    }
  
}




//Sub Programa para Escrever em CSV
void escreverCSV_ficheiro(Servico servico) {
   
    setarCabecalhos(servico); 
    time_t inicio , fim;
    FILE *f;
    char extensao[100] , tempoInicio[100] , tempoFim[100];

    inicio =  construirEstrutura(servico.dataDeInicio.hora , servico.dataDeInicio.minuto , servico.dataDeInicio.segundo , servico.dataDeInicio.dia , servico.dataDeInicio.mes , servico.dataDeInicio.ano);
    

    fim =  construirEstrutura(servico.dataDeFim.hora , servico.dataDeFim.minuto , servico.dataDeFim.segundo , servico.dataDeFim.dia , servico.dataDeFim.mes , servico.dataDeFim.ano);
    
    converterTempo(inicio , tempoInicio);
    converterTempo(fim , tempoFim);
    
    sprintf(extensao, "%s.csv", servico.tipoServico);
    
    f = fopen(extensao, "a");
    if (f != NULL) {
      
        removerEnter(tempoInicio);
        removerEnter(tempoFim);
        
        fprintf(f, "%d; %s; %s; %s\n", servico.codigoID , tempoInicio , tempoFim , servico.cliente);
            
        fclose(f);
    }

    
   
}

//Sub Programa para escreverCSV
void escreverCSV(Servico servicos[], int *total , Servico servicosConcluidos[] , int *totalConcluidos){

    char str[100] , dataFim[100] , dataInicio[100];
    int contador = 0;
    printf("Insira o tipo de servico: \n");
    fgets(str , 100 , stdin);
    removerEnter(str);
    
    for(int i = 0 ; i < (*total) ; i++){
        if(strcasecmp (str ,  servicos[i].tipoServico) == 0 && servicos[i].concluido == 1 && servicos[i].arquivado == 0){
                
           if((*totalConcluidos) < TAMANHO){

                escreverCSV_ficheiro(servicos[i]);
                servicos[i].arquivado = 1;
                servicosConcluidos[*totalConcluidos] = servicos[i];
                (*totalConcluidos)++;
                
                for(int j = i ; j < (*total) - 1 ; j++){
                    servicos[j] = servicos[j + 1];
                }

                (*total)--;
                i--;
                contador++;
           }
           else{
                printf("Arquivo cheio!\n");
           }
        }
    }

    if(contador > 0){
        printf("Sucesso!\n");
    }
    else{
        printf("Nenhum Servico encontrado!\n");
    }
 
}

//Sub Programa Para Eliminar Um Serviço
void eliminarServico(Servico servicos[] , int *total , int codigo){
    int encontrou = 0;
    for(int i = 0 ; i < *total ; i++){
        if(servicos[i].codigoID == codigo && servicos[i].concluido == 0){ 
            encontrou = 1;
            for(int j = i ; j < (*total) - 1 ; j++){
                servicos[j] = servicos[j + 1];
            }
            (*total)--;
        }
    }
    if(encontrou){
        printf("Servico eliminado\n");
    }
    else{
        printf("Nao foi possivel eliminar o servico\n!");
    }

}

//Editar Componentes
void editarComponentes(Servico servicos[] , int total , int codigo){
   
    float valor;
    char nomeComp[50];
    int editavel = 0;
    
    for(int i = 0 ; i < total ; i++){
        
        if(servicos[i].codigoID == codigo && servicos[i].concluido == 0 && servicos[i].pago == 0){
            editavel = 1;
            if(servicos[i].numComponentes >= 10){
                printf("Limite de componentes atingido!\n");
                return;
            }
            printf("Nome do componente substiuido: \n");
            fgets(nomeComp , 50 , stdin);
            removerEnter(nomeComp);
         

            printf("Coloque o preco do componente: \n");
            scanf("%f" , &valor);
            getchar();

            
            servicos[i].precoComponentes[servicos[i].numComponentes] = valor;
            servicos[i].numComponentes++;
            

            char aux[100];
            sprintf(aux , "%s(%.2f)" , nomeComp , valor);
            strcat(servicos[i].observacoes , aux);
            printf("Componentes Registadas com sucesso!\n");
        }
      
    }
    if(!editavel){
        printf("Nao e possivel editar componentes!\n");
    }
}

float precoTotal_componentes(Servico servico){
    float total = 0;
    for(int i = 0 ; i < servico.numComponentes ; i++){
        total += servico.precoComponentes[i];
    }

    return total;
}
//Calcular Servico Individual
float calcularServicoIndividual(Servico s){
    if(strcasecmp(s.tipoServico , "Limpeza") == 0 || strcasecmp(s.tipoServico , "Diagnostico") == 0){
        return s.precoServico;
    }

    if(s.concluido == 1){
        time_t incio = construirEstrutura(s.dataDeInicio.hora , s.dataDeInicio.minuto , s.dataDeInicio.segundo , s.dataDeInicio.dia , s.dataDeInicio.mes , s.dataDeInicio.ano);

        time_t fim = construirEstrutura(s.dataDeFim.hora , s.dataDeFim.minuto , s.dataDeFim.segundo , s.dataDeFim.dia , s.dataDeFim.mes , s.dataDeFim.ano);

        double segundos = difftime(fim , incio);
        float horas = (float) segundos / 3600.0;
        if(horas < 0){
            horas = 0;
        }
        else{
            float precoComponentes = precoTotal_componentes(s);
            return (horas * s.precoServico) + precoComponentes;
        }
    }
    return 0;
}
//Sub Programa Para Preço Total
float precoTotal(Servico servico[] , int total , Servico servicosConcluidos[] , int totalConcluidos , int codigo){

    for(int i = 0 ; i < total ; i++){
        if(servico[i].codigoID == codigo){
           return calcularServicoIndividual(servico[i]);
        }
    }

    for(int i = 0 ; i < totalConcluidos ; i++){
        if(servicosConcluidos[i].codigoID == codigo){
           return calcularServicoIndividual(servicosConcluidos[i]);
        }
    }
    return -1;
}

//Listar Servicos Por Cliente
void listarServicosCliente(Servico servicos[] , int total , Servico concluidos[] , int totalConcluidos){
    char nome[100];
    int encontrou = 0;
    printf("Introduza o nome do cliente: \n");
    fgets(nome , 100 , stdin);
    removerEnter(nome);
  
   for(int i = 0 ; i < total ; i++){
        
        if(strcasecmp(nome ,servicos[i].cliente) == 0){
            printf("ID: %d | Equipamento: %s | Servico: %s|Estado: Nao Concluido\n" , servicos[i].codigoID , servicos[i].equipamento , servicos[i].tipoServico);
            encontrou = 1;
        }
   }

   for(int i = 0 ; i < totalConcluidos ; i++){
        if(strcasecmp(nome , concluidos[i].cliente) == 0){
            printf("ID: %d | Equipamento: %s | Servico: %s |Estado: Concluido\n" , concluidos[i].codigoID , concluidos[i].equipamento , concluidos[i].tipoServico);
            encontrou = 1;
        }
   }

   if(encontrou == 0){
        printf("Cliente nao encontrado!\n");
   }

}

//Sub Programa Para Listar Serviços Por Ano
void listarServicosAno(Servico servicos[] , int total , Servico concluidos[] , int totalConlcuidos){
    time_t fim;
    int anoPesquisado , encontrados = 0;
    char  dataFim[100];
    printf("Digite o ano que deseja pesquisar:\n");
    scanf("%d" , &anoPesquisado);
    //getchar();
    
    for(int i = 0 ; i < total ; i++){
            
        if(servicos[i].dataDeFim.ano == anoPesquisado && servicos[i].concluido == 1){

            fim = construirEstrutura(servicos[i].dataDeFim.hora , servicos[i].dataDeFim.minuto , servicos[i].dataDeFim.segundo , servicos[i].dataDeFim.dia , servicos[i].dataDeFim.mes ,servicos[i].dataDeFim.ano);

            converterTempo(fim , dataFim);
            removerEnter(dataFim);

            printf("Codigo: %d | Cliente: %s | Equipamento: %s | Data de Fim: %s \n" , servicos[i].codigoID , servicos[i].cliente , servicos[i].equipamento , dataFim);
            encontrados++;
        }
      
    }

    for(int i = 0 ; i < totalConlcuidos ; i++){

        if(concluidos[i].dataDeFim.ano == anoPesquisado){
            fim = construirEstrutura(concluidos[i].dataDeFim.hora , concluidos[i].dataDeFim.minuto , concluidos[i].dataDeFim.segundo , concluidos[i].dataDeFim.dia , concluidos[i].dataDeFim.mes ,concluidos[i].dataDeFim.ano);

            converterTempo(fim , dataFim);
            removerEnter(dataFim);

            printf("Codigo: %d | Cliente: %s | Equipamento: %s | Fim: %s \n" , concluidos[i].codigoID , concluidos[i].cliente , concluidos[i].equipamento , dataFim);
            encontrados++;
        }
    }
    if(encontrados == 0){
        printf("Nenhum servico concluido encontrado para o ano %d.\n", anoPesquisado);
    } 

    else {
        printf("Total de registos encontrados: %d\n", encontrados);
    }
  
}

//Sub Programa Para Alterar Preço de Serviços
void atualizarPrecoServico(Servico servicos[] , int total){
    char servicoProcurado[100];
    float novoPreco;
    int encontrou = 0;
    printf("Digite o tipo de servico que deseja atualizar (Reparacao , Diagnostico , Limpeza , Manutencao): \n");
    getchar();
   
    fgets(servicoProcurado , 100 , stdin);
    removerEnter(servicoProcurado);

    printf("Introduza o novo preco para o servico de %s\n" , servicoProcurado);
    scanf("%f" , &novoPreco);

    for(int i = 0 ; i < total ; i++){
        if(strcmp (servicoProcurado , servicos[i].tipoServico) == 0){
            servicos[i].precoServico = novoPreco;
            encontrou = 1;
        }
    }
    if(encontrou){
        printf("Preco do servico atualizado!\n");
    }
    else{
        printf("Nao e possivel atualizar o servico!\n");
    }
}


//Sub Programa Para Listar os Servicos a decorrer
void listarServicos_decorrer(Servico servicos[] , int total){
    char servicoProcurado[100];
    
    printf("Digite o tipo de servico que deseja procurar (Reparacao , Diagnostico , Limpeza , Manutencao): \n");
    fgets(servicoProcurado , 100 , stdin);
    removerEnter(servicoProcurado);

    for(int i = 0 ; i < total ; i++){
        if(strcmp(servicoProcurado , servicos[i].tipoServico) == 0 && servicos[i].concluido == 0){
            printf("Codigo: %d | Cliente: %s | Equipamento: %s \n" , servicos[i].codigoID , servicos[i].cliente , servicos[i].equipamento);
        }
    }

}

//Sub Programa Listar Servicos Por Tipo
void listarServicos_tipo(Servico servicos[] , int total , Servico concluidos[] , int totalConcluidos){
     char servicoProcurado[100];
     int encontrou = 0;
    printf("Digite o tipo de servico que deseja procurar (Reparacao , Diagnostico , Limpeza , Manutencao): \n");
    fgets(servicoProcurado , 100 , stdin);
    removerEnter(servicoProcurado);

    for(int i = 0; i < total ; i++){
        if(strcasecmp(servicoProcurado , servicos[i].tipoServico) == 0 && servicos[i].concluido == 1){
            printf("Codigo: %d | Cliente: %s | Equipamento: %s \n" , servicos[i].codigoID , servicos[i].cliente , servicos[i].equipamento);
            encontrou = 1;
        }
       
    }

    for(int i = 0 ; i < totalConcluidos ; i++){
        if(strcasecmp(servicoProcurado , concluidos[i].tipoServico) == 0){
            printf("Codigo: %d | Cliente: %s | Equipamento: %s \n" , concluidos[i].codigoID , concluidos[i].cliente , concluidos[i].equipamento);
            encontrou = 1;
        }
        
    }
    if(!encontrou){
        printf("Servico Nao Encontrado!\n");
    }
}

//Ordem Alfabetica - Bubble Sort
void ordemAlfabetica(Servico servicos[] , int total){
    Servico aux;
    for(int i = 0 ; i < total - 1 ; i++){
        for(int j = 0 ; j < total-i-1 ; j++){
            if(strcasecmp(servicos[j].cliente , servicos[j + 1].cliente) > 0){
               aux = servicos[j];
               servicos[j] = servicos[j + 1];
               servicos[j + 1] = aux;
            }
        }
    }
}

//Ordem Crescente das datas de conclusão (Algoritmo Insertion Linear) // Servicos Ativos
void ordemCrescenteDatas(Servico servicos[] , int total){
    time_t tempoAuxiliar , tempoAuxiliarDois;
    Servico aux , auxdois;
    int  j ;
    
            for(int i = 1 ; i <= total - 1 ; i++){

                    aux = servicos[i]; 
                    auxdois = servicos[i - 1]; 

                    tempoAuxiliar = construirEstrutura(aux.dataDeFim.hora , aux.dataDeFim.minuto , aux.dataDeFim.segundo , aux.dataDeFim.dia , aux.dataDeFim.mes , aux.dataDeFim.ano);

                    tempoAuxiliarDois = construirEstrutura(auxdois.dataDeFim.hora , auxdois.dataDeFim.minuto , auxdois.dataDeFim.segundo , auxdois.dataDeFim.dia , auxdois.dataDeFim.mes , auxdois.dataDeFim.ano);

                    j = i;

                    while (j > 0 && tempoAuxiliarDois > tempoAuxiliar) 
                    {
                        servicos[j] = servicos[j - 1]; 
                        j = j-1; 
                    }
                servicos[j] = aux; 
        
            }
        
    
    printf("Datas ordenadas com sucesso!\n");
}


void duracaoServicosConcluidos(Servico servicosConcluidos[] , int totalConcluidos , Servico servicos[] , int total){ 
    time_t inicio , fim , segundos;
    time_t min = -1 , max = 0;
    int codigoMaximo = -1 , codigoMinimo = -1;
    int contadorGeral = 0;
    long long int somaSegundos = 0;

    for(int i = 0 ; i< total ; i++){
        if(servicos[i].concluido ==  1){
            inicio = construirEstrutura(servicos[i].dataDeInicio.hora , servicos[i].dataDeInicio.minuto , servicos[i].dataDeInicio.segundo , servicos[i].dataDeInicio.dia , servicos[i].dataDeInicio.mes , servicos[i].dataDeInicio.ano);

            fim = construirEstrutura(servicos[i].dataDeFim.hora , servicos[i].dataDeFim.minuto , servicos[i].dataDeFim.segundo , servicos[i].dataDeFim.dia , servicos[i].dataDeFim.mes , servicos[i].dataDeFim.ano);

            segundos = difftime(fim , inicio);

            if(contadorGeral == 0 || segundos < min){
                min = segundos;
                codigoMinimo = servicos[i].codigoID;
            }

            if(segundos > max){
                max  = segundos;
                codigoMaximo = servicos[i].codigoID;
            }
            somaSegundos = somaSegundos + segundos;
            contadorGeral++;
        }
       
    }

    for(int i = 0 ; i< totalConcluidos ; i++){
        
            inicio = construirEstrutura(servicosConcluidos[i].dataDeInicio.hora , servicosConcluidos[i].dataDeInicio.minuto , servicosConcluidos[i].dataDeInicio.segundo , servicosConcluidos[i].dataDeInicio.dia , servicosConcluidos[i].dataDeInicio.mes , servicosConcluidos[i].dataDeInicio.ano);

            fim = construirEstrutura(servicosConcluidos[i].dataDeFim.hora , servicosConcluidos[i].dataDeFim.minuto , servicosConcluidos[i].dataDeFim.segundo , servicosConcluidos[i].dataDeFim.dia , servicosConcluidos[i].dataDeFim.mes , servicosConcluidos[i].dataDeFim.ano);

            segundos = difftime(fim , inicio);

            if(contadorGeral == 0 || segundos < min){
                min = segundos;
                codigoMinimo = servicosConcluidos[i].codigoID;
            }

            if(segundos > max){
                max  = segundos;
                codigoMaximo = servicosConcluidos[i].codigoID;
            }
            somaSegundos = somaSegundos + segundos;
            contadorGeral++;
    }

    if(contadorGeral > 0){
        printf("Codigo Maximo: %d Tempo Maximo: %lld segundos\n" , codigoMaximo , max);
        printf("Codigo Minimo: %d Tempo Minimo: %lld segundos\n" , codigoMinimo , min);
        printf("Media em segundos: %.2f\n" , (float)(somaSegundos) / (float)(contadorGeral));
    }
    else{
        printf("Nenhum servico encontrado");
    }
 
}

void duracaoServicosTipo(Servico servicosConcluidos[] , int totalConcluidos , Servico servicos[] , int total){
    time_t min = -1  , max = 0 , inicio , fim , segundos;

    char servicoPesquisado[100];
    long long int somaSegundos = 0;
    float media;
    int codigoMax = -1 , codigoMin = -1 , contadorGeral = 0;
    printf("Coloque o tipo de servico que deseja pesquisar: \n");
    fgets(servicoPesquisado , 100 , stdin);
    removerEnter(servicoPesquisado);
    for(int i = 0 ; i < total ; i++){
        if(strcasecmp(servicos[i].tipoServico , servicoPesquisado) == 0 && servicos[i].concluido == 1){
            inicio = construirEstrutura(servicos[i].dataDeInicio.hora , servicos[i].dataDeInicio.minuto , servicos[i].dataDeInicio.segundo , servicos[i].dataDeInicio.dia , servicos[i].dataDeInicio.mes , servicos[i].dataDeInicio.ano);

            fim = construirEstrutura(servicos[i].dataDeFim.hora , servicos[i].dataDeFim.minuto , servicos[i].dataDeFim.segundo , servicos[i].dataDeFim.dia , servicos[i].dataDeFim.mes , servicos[i].dataDeFim.ano);

            segundos = difftime(fim , inicio);

            if(contadorGeral == 0 || segundos < min){
                min = segundos;
                codigoMin = servicos[i].codigoID;
            }

            if(segundos > max){
                max = segundos;
                codigoMax = servicos[i].codigoID;
            }

            contadorGeral++;
            somaSegundos = somaSegundos + segundos;
        }
    }

    for(int i = 0 ; i < totalConcluidos ; i++){
        if(strcasecmp(servicosConcluidos[i].tipoServico , servicoPesquisado) == 0){
            inicio = construirEstrutura(servicosConcluidos[i].dataDeInicio.hora , servicosConcluidos[i].dataDeInicio.minuto , servicosConcluidos[i].dataDeInicio.segundo , servicosConcluidos[i].dataDeInicio.dia , servicosConcluidos[i].dataDeInicio.mes , servicosConcluidos[i].dataDeInicio.ano);

            fim = construirEstrutura(servicosConcluidos[i].dataDeFim.hora , servicosConcluidos[i].dataDeFim.minuto , servicosConcluidos[i].dataDeFim.segundo , servicosConcluidos[i].dataDeFim.dia , servicosConcluidos[i].dataDeFim.mes , servicosConcluidos[i].dataDeFim.ano);

            segundos = difftime(fim , inicio);

            if(contadorGeral == 0 || segundos < min){
                min = segundos;
                codigoMin = servicosConcluidos[i].codigoID;
            }

            if(segundos > max){
                max = segundos;
                codigoMax = servicosConcluidos[i].codigoID;
            }

            contadorGeral++;
            somaSegundos = somaSegundos + segundos;
        }
    }

    if(contadorGeral > 0){
        printf("Codigo Maximo: %d Tempo Maximo: %lld segundos\n" , codigoMax , max);
        printf("Codigo Minimo: %d Tempo Minimo: %lld segundos\n" , codigoMin , min);
        printf("Media em segundos: %.2f\n" , (float)(somaSegundos) / (contadorGeral));
    }
    else{
        printf("Servico nao encontrado!\n");
    }
}

//Servico Pago

void servicoPago(Servico servicos[] , int total){
    time_t fim , dataPagamento , segundos;
    int dia , mes , hora , minuto , segundo , ano;
    
    int codigo;
    char data[100];
    int encontrou = 0;
    printf("Introduza o codigo: \n");
    scanf("%d" , &codigo);
    getchar();
    for(int i = 0 ; i < total ; i++){
        if(servicos[i].codigoID == codigo && servicos[i].concluido == 1 && servicos[i].pago == 0){

                encontrou = 1;
                printf("Coloque a data de pagamento no formato de inteiros (Dia Mes hh mm ss 2026)\n");

                scanf("%d %d %d %d %d %d" , &dia , &mes , &hora , &minuto , &segundo , &ano);
                dataPagamento = construirEstrutura(hora , minuto , segundo , dia , mes , ano);

                fim = construirEstrutura(servicos[i].dataDeFim.hora , servicos[i].dataDeFim.minuto , servicos[i].dataDeFim.segundo , servicos[i].dataDeFim.dia , servicos[i].dataDeFim.mes , servicos[i].dataDeFim.ano);

                segundos = difftime(dataPagamento , fim);

                if(segundos >= 0){
                     
                    servicos[i].pago = 1;
                    printf("O servico: %s com o codigo: %d ,  foi dado como pago\n" , servicos[i].tipoServico , servicos[i].codigoID);
                    servicos[i].dataDePagamento.dia = dia;
                    servicos[i].dataDePagamento.mes = mes;
                    servicos[i].dataDePagamento.hora = hora;
                    servicos[i].dataDePagamento.minuto = minuto;
                    servicos[i].dataDePagamento.segundo = segundo;
                    servicos[i].dataDePagamento.ano = ano;
                }
                else{
                    printf("Erro a data de pagamento nao pode ser anterior a data de conclusao\n");
                }     
        }
    }
    if(encontrou == 0){
        printf("Conclua o servico primeiro\n");
    }

}


//Sub Programa Para Gerar ID´s 
int obterMaiorId(){

    int maior = 0;
    FILE *f;
    Servico s;
    f = fopen("Servicos.dat" , "rb");
    if(f == NULL){
        printf("Nao e possivel abrir o ficheiro\n");
    }
    else{
        while(fread(&s, sizeof(Servico) , 1 , f) == 1){
            if(s.codigoID > maior){
                maior = s.codigoID;
            }
        }
         fclose(f);
    }
   
    return maior;
}


int main(){
  

    FILE *f;
    //Variaveis para opcao , codigo e ID
    int opcao ,   codigo;
    int ultimoIDGravado = obterMaiorId() + 1;
    int totalServicos = 0 , totalConcluidos = 0;
    Servico servicos[TAMANHO];
    Servico servicosConcluidos[TAMANHO];

    f = fopen("Servicos.dat" , "rb");
    if(f != NULL){
     
        Servico tmp;
        while (fread(&tmp , sizeof(Servico) , 1 , f) == 1)
        { 
           if(tmp.arquivado == 0){
                servicos[totalServicos++] = tmp;
           }
           else{
            servicosConcluidos[totalConcluidos++] = tmp;
           }
             
        }
         fclose(f);
    }
  


    //Inicialização de do while
    printf("Selecione 1 para continuar 0 para Sair\n");
    scanf("%d" , &opcao);
    do{
       
        printf("Selecione o tipo de opcao : 2 - Registo\t3 - Servico Concluido\t4 - Relatorio Mensal  \t5 - Apresentar\n");
        printf("6 - Eliminar Um servico\t 7-Editar Componentes\t 8-Preco Total do Servico\t 9-Listar Servicos Por Cliente\n");
        printf("10 - Listar Servicos Concluidos Por Ano\t 11 - Atualizar preco de servico\t 12 - Listar Servicos a decorrer\n");
        printf("13 - Listar Servicos Concluidos Por Tipo\t14 - Ordenar Clientes Por Ordem Alfabetica\n");
        printf("15 - Ordem Crescente das Datas de Conclusão\t 16 - Duracao Minima , Maxima e Media\n");
        printf("17 - Duracao Maxima , Minima e Media por Tipo de Servico\n");
        printf("18 - Dar Servico Pago\n");
        scanf("%d" , &opcao);
        getchar();

        switch (opcao)
        {

            //Case 2 Para Registo de Serviços
            case 2:
                if(totalServicos < TAMANHO){
                    Servico novo = registo(ultimoIDGravado);
                    servicos[totalServicos] = novo;
                    ultimoIDGravado++;
                    totalServicos++;
                }
                else{
                    printf("Memoria cheia!\n");
                }
            break;
            //Case 3 Para Dar Serviço Concluido
            case 3:
                printf("Introduza o codigo: \n");
                scanf("%d" , &codigo);
                getchar();
                servicoConcluido(servicos , totalServicos , codigo);
                break;
            //Case 4 Para Relatorio Mensal Ficheiro CSV
            case 4:
               
                escreverCSV(servicos , &totalServicos , servicosConcluidos , &totalConcluidos);
                break;
            
            //Apresentar
            case 5:
                printf("Ficheiro: \n");
                listarFicheiro();
                printf("\n");
                printf("Estrutura Ativos: \n");
                
                for(int i = 0 ; i < totalServicos ; i++){
                    printf("Cliente: %s\n" , servicos[i].cliente);
                    printf("Codigo: %d\n" , servicos[i].codigoID);
                    printf("Tipo de Servico: %s\n" , servicos[i].tipoServico);
                    printf("Concluidos: %d\n" , servicos[i].concluido);
                    printf("Pago: %d\n" , servicos[i].pago);
                    
                    printf("Observacoes: %s\n" , servicos[i].observacoes);
                   
                }

                 
                printf("\n");
                printf("Estrutura Concluidos: \n");
                for(int i = 0 ; i < totalConcluidos ; i++){
                    printf("Cliente: %s\n" , servicosConcluidos[i].cliente);
                    printf("Concluidos: %d\n" , servicosConcluidos[i].concluido);
                    printf("Tipo de Servico: %s\n" , servicosConcluidos[i].tipoServico);
                    printf("Pago: %d\n" , servicosConcluidos[i].pago);
                    printf("\n");
                }
                    
                
            //Eliminar Serviços
            break;
            case 6:
                printf("Insira o codigo do servico que deseja eliminar:\n");
                scanf("%d" , &codigo);
                eliminarServico(servicos , &totalServicos , codigo);
                break;
            //Adicionar Componentes
            case 7:
                printf("Insira o codigo para adicionar componentes:\n");
                scanf("%d" , &codigo);
                getchar();
                editarComponentes(servicos , totalServicos , codigo);
                break;
            //Obter Preço Total
            case 8:
                printf("Insira o codigo para o preco total: \n");
                scanf("%d" , &codigo);
                getchar();
                float total = precoTotal(servicos , totalServicos , servicosConcluidos , totalConcluidos , codigo);
                if(total > 0){
                    printf("Valor total a pagar: %.2f\n" , total);

                }
                else{
                    printf("Erro servico nao encontrado!\n");
                }
                break;
            //Listar Servicos Por Cliente
            case 9:
                listarServicosCliente(servicos , totalServicos , servicosConcluidos , totalConcluidos);
                break;

            //Listar Serviços Concluidos Por Ano
            case 10:
                listarServicosAno(servicos , totalServicos , servicosConcluidos , totalConcluidos);
                break;

            //Alterar Preços De Serviços
            case 11:
                atualizarPrecoServico(servicos , totalServicos);
                break;
            //Listar Servicos A Decorrer Por tipo de Serviço
            case 12:
                listarServicos_decorrer(servicos , totalServicos);
                break;
            //Listar Servicos Concluidos Por Tipo
            case 13:
                listarServicos_tipo(servicos , totalServicos , servicosConcluidos , totalConcluidos);
                break;
            //Ordenar Clientes Por Ordem Alfabetica
            case 14:
                ordemAlfabetica(servicos , totalServicos);
                for(int i = 0 ; i < totalServicos ; i++){
                    printf("Cliente: %s\n" , servicos[i].cliente);
                }
                break;
            //Ordem Crescente das Datas Concluidas
            case 15:
                ordemCrescenteDatas(servicos , totalServicos);
                break;
            case 16:
                duracaoServicosConcluidos(servicosConcluidos , totalConcluidos , servicos , totalServicos);
                break;
            case 17:
                duracaoServicosTipo(servicosConcluidos , totalConcluidos , servicos , totalServicos);
                break;
            case 18:
                servicoPago(servicos , totalServicos);
                break;
            default:
                break;
        }

        printf("Selecione 1 para contunuar 0 para Sair\n");
        scanf("%d" , &opcao);
        getchar();

    }while(opcao != 0);
    guardarTabelaUnica(servicos , totalServicos , servicosConcluidos , totalConcluidos); 
}

