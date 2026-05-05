# Logger TDD — Dynora Engine

## Documento de Arquitetura e Implementação

### 1. Visão Geral

O Logger é o subsistema de observabilidade da Dynora, responsável por registrar eventos de execução com baixo custo, boa legibilidade e previsibilidade de comportamento.

Ele tem dois objetivos complementares:

- **validação inicial da infraestrutura de logging**, já presente no código atual
- **evolução para um pipeline assíncrono, bounded e previsível**, com fila MPSC, worker thread e políticas de overflow

A implementação atual funciona como um **protótipo síncrono** que valida:

- API pública
- categorização
- níveis de log
- formatação de mensagens
- timestamps
- backend pluggable

A arquitetura alvo é um **pipeline assíncrono** com separação explícita entre:

- produtores
- fila
- consumidor
- políticas
- backends

---

### 2. Objetivos do sistema

O Logger existe para fornecer:

- visibilidade diagnóstica sobre o motor e suas aplicações
- logs estruturados com origem, nível e categoria
- suporte a depuração e análise de comportamento
- backends extensíveis
- comportamento previsível sob carga
- mínimo impacto possível nas threads críticas do motor

---

### 3. Escopo por fase

#### 3.1 Fase atual: Protótipo Síncrono

A implementação atual cobre:

- API pública mínima
- macros de log
- filtro por nível e categoria
- timestamp monotônico
- sequência incremental
- eventos formatados com buffer interno
- registro de backends
- backend de console

Limitações atuais:

- não é thread-safe
- não possui fila assíncrona
- não possui worker thread
- não possui política de overflow
- backends executam no mesmo fluxo da chamada

Essa fase existe para validar o contrato da API e o formato dos eventos.

#### 3.2 Fase alvo: logger assíncrono

A arquitetura final incluirá:

- fila bounded MPSC lock-free
- worker thread dedicada
- batching
- policy layer
- métricas internas
- shutdown controlado
- possibilidade de integração com job system

---

### 4. Princípios de projeto

#### 4.1 Baixo overhead

O logger deve ser barato quando ativo e praticamente invisível quando filtrado.

#### 4.2 Não bloqueio nas threads críticas

Threads do motor não devem ser bloqueadas pelo sistema de logging.

#### 4.3 Memória previsível

O sistema deve operar com capacidade fixa e comportamento determinístico.

#### 4.4 Separação de interesses

Filtro, captura, transporte, política e saída devem ser responsabilidades separadas.

#### 4.5 Evolução incremental

A fase atual deve evoluir para a fase alvo sem reescrever tudo do zero.

---

## PARTE I — Estado atual da implementação

---

### 5. Arquitetura atual

O fluxo atual é:
~~~mermaid
graph TD;
A[Macro de log] --> B[logger_should_emit]
B -->|aceito| C[logger_dispatch]
C --> D[Formatação]
D --> E[Backends registrados]
~~~

#### Características desta fase

- síncrono
- simples
- legível
- sem fila
- sem thread dedicada
- backend executado imediatamente

Essa fase é útil para validar o “contrato funcional” do logger.

---

### 6. API pública atual

#### 6.1 Inicialização
~~~c
void logger_init(enum DynoraLogLevel level, DynoraLogCategory category);
~~~

Define:

- nível mínimo de emissão
- máscara de categorias ativa
- contador de sequência inicializado em zero
- lista de backends zerada

#### 6.2 Configuração dinâmica

~~~c
void logger_set_level(enum DynoraLogLevel level);
void logger_set_category_mask(DynoraLogCategory category);
~~~

Permite alterar o comportamento em runtime.

#### 6.3 Filtro

~~~c
bool logger_should_emit(DynoraLogCategory category,
    enum DynoraLogLevel level);
~~~

Executa a verificação rápida de:

- categoria ativa
- nível mínimo

#### 6.4 Dispatch interno

~~~c
void logger_dispatch(DynoraLogCategory category,
    enum DynoraLogLevel level,
    const char* file,
    uint32_t line,
    const char* function,
    const char* fmt,
    ...);
~~~

Função de infraestrutura.
Não deve ser chamada diretamente pelo código do usuário.

##### Validação em Debug

Na build de debug, o logger valida invariantes críticas usando `assert`:

- `level < DYNORA_LEVEL_COUNT`
- `fmt != NULL`

Essas validações têm como objetivo detectar uso incorreto da API durante desenvolvimento.

Em build de release (`NDEBUG` definido), essas validações não são executadas, e o logger falha de forma segura (early return).

#### 6.5 Macros públicas

As macros são a interface real de uso.

~~~c
#define DYNORA_LOG(cat, level, fmt, ...)
#define DYNORA_LOG_DEBUG(cat, fmt, ...)
#define DYNORA_LOG_INFO(cat, fmt, ...)
#define DYNORA_LOG_WARNING(cat, fmt, ...)
#define DYNORA_LOG_ERROR(cat, fmt, ...)
#define DYNORA_LOG_FATAL(cat, fmt, ...)
~~~

Essas macros capturam:

- arquivo
- linha
- função
- nível
- categoria

e chamam o logger com o menor custo possível do lado do usuário.

---

### 7. Tipos públicos

#### 7.1 Níveis de log

~~~c
enum DynoraLogLevel {
    DYNORA_LEVEL_DEBUG = 0,
    DYNORA_LEVEL_INFO,
    DYNORA_LEVEL_WARNING,
    DYNORA_LEVEL_ERROR,
    DYNORA_LEVEL_FATAL,
};
~~~

##### Regra semântica
A severidade cresce com o valor numérico.

Isso significa:

- `DEBUG` é o menos severo
- `FATAL` é o mais severo

Essa ordem é importante para o filtro.

`DYNORA_LEVEL_COUNT` é um valor sentinela, não um nível válido.
`level >= logger_level` significa “nível aceito”.

#### 7.2 Categoria

~~~c
typedef uint32_t DynoraLogCategory;
~~~
A categoria é uma máscara de bits.

Categorias atuais:

~~~c
#define DYNORA_LOG_RENDER
#define DYNORA_LOG_AUDIO
#define DYNORA_LOG_PHYSICS
#define DYNORA_LOG_ECS
#define DYNORA_LOG_IO
#define DYNORA_LOG_GENERAL
~~~

E máscaras utilitárias:

~~~c
#define DYNORA_LOG_NONE
#define DYNORA_LOG_ALL
~~~

##### Regra semântica

Uma categoria é aceita se o bit estiver presente na máscara ativa.

#### 7.3 Evento de log

~~~c
typedef struct DynoraLogEvent {
    uint64_t timestamp;
    uint64_t sequence;
    const char* file;
    const char* function;
    void* user_data;
    char message[DYNORA_LOG_MESSAGE_MAX];
    uint32_t line;
    DynoraLogCategory category;
    uint8_t level;
} DynoraLogEvent;
~~~

##### Interpretação dos campos

- `timestamp`: timestamp monotônico em nanosegundos (não representa tempo de calendário)
- `sequence`: ordem incremental dos eventos
- `file`: origem do log
- `function`: função de origem
- `user_data`: campo reservado para extensões futuras
- `message`: mensagem formatada em buffer fixo
- `line`: linha de origem
- `category`: categoria de emissão
- `level`: severidade

##### Observação importante

No estado atual, `DynoraLogEvent` existe apenas durante a execução de `backend->write(...)`.
Backends não devem guardar ponteiros para o evento ou para seus campos sem copiar os dados.

- A mensagem pode ser truncada se exceder `DYNORA_LOG_MESSAGE_MAX`
- O buffer é sempre terminado com `\0`

---

## PARTE II — Análise do código atual

---

### 8. O que o Código Atual já Faz Bem

#### 8.1 O filtro é simples e barato

A função `logger_should_emit(...)` faz o que precisa fazer com custo mínimo.

#### 8.2 A mensagem já é materializada no produtor

A formatação acontece antes da escrita no backend, o que simplifica muito a implementação inicial.

#### 8.3 A estrutura do evento já é autocontida

O evento já carrega o essencial para saída imediata.

#### 8.4 O backend é plugável

`logger_add_backend(...)` permite registrar sinks diferentes.

#### 8.5 O console backend valida o pipeline

O backend de console é perfeito como prova de conceito.

---

### 9. Limitações do Código Atual

#### 9.1 Não é thread-safe

O estado global é compartilhado sem sincronização.

Isso afeta:

- `counter`
- `backend_count`
- `level`
- `category_mask`
- lista de backends

#### 9.2 Não existe fila

O logger atual não faz desacoplamento entre produção e consumo.

#### 9.3 Não existe worker thread

O fluxo é totalmente síncrono.

#### 9.4 Não existe política de overflow

Como não há fila, não há saturação controlada.

### 9.5 Não existe shutdown controlado

A finalização ainda não é modelada como estado explícito.

---

### 10. Interpretação Correta da Implementação Atual

> um logger síncrono de validação, usado para provar a API, a formatação, os níveis, as categorias e a integração com backends.

---

## PARTE III — Arquitetura Alvo

---

### 11. Arquitetura Final Desejada

O fluxo alvo será:

~~~mermaid
graph TD;
ET[Engine Threads] --> LF[Log Filter]
LF --> LQ[Log Queue MPSC]
LQ --> LWT[Logger Worker Thread]
LWT --> PL[Policy Layer]
PL --> BT[Backends]
~~~

---

### 12. Produtores

Produtores são todas as threads do motor que podem emitir logs:

- game thread
- render thread
- physics thread
- audio thread
- I/O thread
- streaming thread
- outras threads do core ou de sistemas

##### Responsabilidades

- executar filtro rápido
- construir o evento
- enfileirar sem bloquear
- nunca falar com backend diretamente

##### Regra essencial

Produtores nunca devem travar em espera ativa prolongada para logging.

---

### 13. Log Filter

O filtro existe antes da criação final do evento.

###### Critérios

- categoria ativa
- nível mínimo aceito

##### Comportamento

Se o evento for rejeitado:

- não há formatação
- não há enqueue
- não há custo significativo além da checagem de bits e nível

---

### 14. Log Queue

#### 14.1 Modelo

A fila será:

- MPSC: múltiplos produtores, um consumidor
- bounded: capacidade fixa
- lock-free para produtores
- FIFO no consumo

#### 14.2 Estrutura recomendada

A implementação ideal para a fase alvo é um ring buffer circular com:

- índice de produção
- índice de consumo
- slots pré-alocados
- contadores atômicos

#### 14.3 Motivos da escolha

- memória previsível
- custo constante
- boa localidade de cache
- boa adequação ao modelo de engine
- baixo overhead por evento

#### 14.4 Política em saturação

Se a fila estiver cheia, a política de overflow decide o que fazer.

---

### 15. Logger Worker Thread

A worker thread será o único consumidor da fila.

##### Responsabilidades

- retirar eventos em lote
- aplicar política
- encaminhar para backends
- atualizar métricas
- realizar flush e shutdown ordenado

##### Modelo geral

~~~c
while (running || draining) {
    pop_batch(events);
    process(events);
    dispatch(events);
}
~~~

##### Garantias

- preserva a ordem de consumo
- trabalha com lotes parciais
- não bloqueia produtoras
- encerra de forma previsível

---

### 16. Batching

O batching existe para reduzir overhead.

##### Benefícios

- melhor cache locality
- menos sincronização
- menos chamadas repetitivas em backend
- potencial ganho em I/O

##### Regras

- eventos podem ser consumidos em grupos
- um batch pode ser parcial
- o backend pode receber chamadas em sequência ou em lote, conforme a versão

---

### 17. Policy Layer

A policy layer decide o comportamento sob pressão, overflow e desligamento.

##### Responsabilidades

- definir retenção sob carga
- decidir descarte seletivo
- preservar logs críticos
- controlar emissão de eventos sintéticos
- orientar flush no shutdown

##### Política básica recomendada para a fase inicial

Quando a fila saturar:

- preservar `ERROR` e `FATAL` sempre que possível
- permitir descarte de `DEBUG` e `INFO`
- permitir descarte de `WARNING` apenas em saturação severa
- contabilizar tudo que foi perdido

##### Evento sintético de overflow

Opcionalmente, quando a fila se recuperar, o sistema pode emitir um evento resumindo a perda de logs.

Exemplo conceitual:

- `27 logs descartados por overflow`

Isso melhora a observabilidade do próprio logger.

---

### 18. Backends

Backends são os sinks de saída.

###### Exemplos

- console
- arquivo
- overlay de debug
- telemetria
- rede
- futura integração com job system

##### Contrato conceitual
~~~c
void (*write)(const DynoraLogEvent* event, void* user_data);
~~~

##### Responsabilidades

- receber eventos
- produzir saída no destino
- não alterar os dados do evento
- não fazer filtragem
- não assumir ownership do evento

##### Modelo da fase inicial

Na fase final do logger assíncrono, os backends serão chamados pela worker thread.

##### Modelo futuro

Em um segundo momento, a execução dos backends pode ser delegada ao job system do core, sem quebrar o contrato dos produtores.

---

## PARTE IV — Decisões Técnicas Fundamentais

---

### 19. Modelo da Fila

##### Decisão

A fila será um MPSC lock-free bounded ring buffer.

##### Justificativa

Esse modelo é o mais adequado para:

- múltiplas threads produtoras
- um consumidor dedicado
- comportamento previsível
- baixo overhead
- boa adequação a motor de jogo

##### Invariantes

- a fila tem capacidade fixa
- o produtor nunca bloqueia indefinidamente
- o consumidor é único
- a ordem de consumo é preservada

---

### 20. Estratégia de Formatação

##### Decisão para a fase atual

A formatação acontece no produtor, antes da fila.

##### Justificativa

Isso simplifica a implementação inicial e permite validar a API rapidamente.

##### Evolução futura

A formatação poderá migrar para a worker thread, permitindo:

- captura de argumentos crus
- menor custo no produtor
- melhor distribuição do trabalho

##### Regra de documentação

A documentação deve dizer explicitamente que essa mudança faz parte da evolução, não da fase atual.

---

### 21. Overflow Policy

##### Objetivo

Nunca bloquear o motor por causa de logs.

##### Proposta para a fase inicial

Quando a fila estiver cheia:

- incrementa contador de descarte
- descarta eventos de baixa severidade primeiro
- mantém logs críticos sempre que possível
- não tenta retry infinito
- não trava thread produtora

##### Contadores sugeridos

- total descartado por overflow
- total descartado por shutdown
- pico de ocupação
- ocupação atual
- total enfileirado
- total processado

##### Recomendação prática

No começo, a política pode ser simples e conservadora.
Complexidade excessiva aqui tende a gerar bug antes de benefício.

---

### 22. Shutdown

##### Objetivo

Garantir encerramento ordenado e previsível.

##### Máquina de estados sugerida

- `RUNNING`
- `STOP_ACCEPTING`
- `DRAINING`
- `STOPPED`

##### Fluxo

1. parar de aceitar novos eventos
2. sinalizar encerramento
3. drenar a fila existente
4. flush dos backends
5. encerrar worker
6. marcar o sistema como finalizado

##### Regra importante

Eventos emitidos após o início de `STOP_ACCEPTING` devem ser descartados explicitamente, nunca ignorados de forma silenciosa.

##### Observação

Prometer “nenhum log perdido” em cenário de concorrência total é mais forte do que um logger bounded pode garantir sem custo adicional.
O contrato correto é:

- durante shutdown normal, os eventos já aceitos são drenados
- novos eventos após a transição de parada não são aceitos

---

### 23. Timestamp

##### Decisão

O timestamp é monotônico em nanosegundos e armazenado em `uint64_t`.

##### Implementação atual

- Windows: `QueryPerformanceCounter`
- Linux/Unix: `clock_gettime(CLOCK_MONOTONIC)`

##### Interpretação correta

Esse timestamp serve para:

- ordenação relativa
- profiling
- medição de intervalo

Ele não representa data e hora de calendário.

---

### 24. Sequência

##### Decisão

sequence é um contador global crescente.

##### Uso

- ajuda a ordenar eventos
- ajuda a detectar perdas
- ajuda a depurar concorrência
- auxilia métricas e rastreio

##### Observação

Na arquitetura final, esse valor deve ser produzido de forma segura em concorrência.

---

### 25. Métricas Internas

As métricas devem existir desde cedo, porque elas ajudam a validar a própria arquitetura.

##### Métricas recomendadas

- ocupação atual da fila
- pico de ocupação
- total enfileirado
- total processado
- descartados por overflow
- descartados por shutdown

##### Por que isso é importante

Sem métricas, o logger vira uma caixa preta.
Com métricas, você enxerga se:

- a fila está pequena demais
- o backend está lento
- há overflow frequente
- o shutdown está perdendo eventos
- o sistema está saudável

---

## PARTE V — Contrato da API Pública

---

### 26. Inicialização

~~~c
void logger_init(enum DynoraLogLevel level, DynoraLogCategory category);
~~~

##### Contrato

- define nível inicial
- define máscara inicial
- zera contadores
- prepara o estado do logger

---

### 27. Configuração Dinâmica

~~~c
void logger_set_level(enum DynoraLogLevel level);
void logger_set_category_mask(DynoraLogCategory category);
~~~

##### Contrato

- podem ser usados em runtime
- na fase atual, não são thread-safe
- na fase final, devem ser protegidos por mecanismos adequados ou por modelo de publicação seguro

---

### 28. Filtro Rápido

~~~c
bool logger_should_emit(DynoraLogCategory category,
                        enum DynoraLogLevel level);
~~~

##### Contrato

Retorna verdadeiro apenas se:

- a categoria estiver habilitada
- o nível for suficiente

Essa função é parte do caminho quente e precisa ser barata.

---

### 29. Dispatch Interno

~~~c
void logger_dispatch(DynoraLogCategory category,
                     enum DynoraLogLevel level,
                     const char* file,
                     uint32_t line,
                     const char* function,
                     const char* fmt,
                     ...);
~~~

##### Contrato

- não é API de uso direto do usuário
- existe para suportar as macros
- é o ponto onde o evento é construído

---

### 30. Macros Públicas

~~~c
#define DYNORA_LOG(cat, level, fmt, ...)
#define DYNORA_LOG_DEBUG(cat, fmt, ...)
#define DYNORA_LOG_INFO(cat, fmt, ...)
#define DYNORA_LOG_WARNING(cat, fmt, ...)
#define DYNORA_LOG_ERROR(cat, fmt, ...)
#define DYNORA_LOG_FATAL(cat, fmt, ...)
~~~

##### Contrato

As macros:

- capturam contexto automático
- executam o filtro
- chamam o dispatch apenas se necessário

Isso mantém a API do usuário limpa e consistente.

---

## PARTE VI — Backends na Fase Atual e Futura

---

### 31. Backend Console Atual

O backend de console atual é síncrono e simples.

##### Papel dele

- validar o fluxo de eventos
- testar formatação
- permitir inspeção visual rápida

##### Limitação

Ele não representa ainda o backend da arquitetura final, pois roda no mesmo fluxo da chamada.

---

### 32. Evolução dos Backends

##### Fase atual

- backends sincronamente invocados pela função de dispatch

##### Fase final
- backends chamados pelo worker thread

##### Fase posterior

- integração com job system do core
- possibilidade de executar alguns backends de forma paralela
- sem alterar o contrato dos produtores

---

## PARTE VIII — Escopo Atual, Recursos Adiados e Evolução

---

### 34. Escopo Atual

A implementação atual inclui:

- macros de log
- filtro por nível
- filtro por categoria
- evento com buffer fixo
- timestamp monotônico
- sequência incremental
- backend plugável
- backend console de validação

### 35. Recursos Adiados

Ainda não fazem parte da fase atual:

- fila MPSC
- worker thread
- overflow policy formal
- batching real
- métricas completas
- shutdown em máquina de estados
- buffers locais por thread
- deferred formatting
- integração com job system
- telemetria remota

### 36. Trabalhos Futuros

#### Fase 2

- fila bounded MPSC
- worker thread
- batching
- overflow controlado
- métricas internas

#### Fase 3

- integração com job system
- otimizações de backend
- possíveis buffers locais por thread

#### Fase 4

- logging distribuído
- telemetria remota
- integração com profiling avançado
