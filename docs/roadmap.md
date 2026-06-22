### Regra de Versionamento

- V0.0.1 é experimental / validação
- V0.0.2 é o contrato estável do núcleo
- V0.0.3+ são apenas evoluções incrementais

---

### V0.0.1 — Protótipo de Validação Funcional (atual)

**Objetivo:** validar API, ergonomia e pipeline básico de logging.

- API pública mínima (`logger_init`, `set_level`, `set_category_mask`)
- macros de log (`DYNORA_LOG_*`)
- filtro por nível e máscara de categoria
- timestamp monotônico (dependente da plataforma)
- contador de sequência incremental
- formatação de eventos com buffer interno fixo
- interface de backend plugável (modelo síncrono)
- backend de console (implementação de referência)
- modelo single-thread (sem garantias de concorrência)

> Objetivo principal: validação da API e corretude funcional.  
> Não projetado para performance ou concorrência.

---

### V0.0.2 — Core Assíncrono (planejado)

**Objetivo:** introduzir concorrência controlada e desacoplamento básico entre produção e consumo.

- fila MPSC bounded (múltiplos produtores, consumidor único)
- dispatcher thread dedicada
- backend de console integrado ao pipeline do dispatcher
- lifecycle básico de shutdown (`RUNNING → STOP_ACCEPTING → DRAINING → STOPPED`)
- política simples de overflow (estratégia bounded de descarte)
- prevenção mínima de backpressure (producer path não bloqueante)
- preservação de ordenação via modelo single-consumer

> Foco: corretude sob concorrência, ainda não extensibilidade.  
> Os backends permanecem simples e síncronos do ponto de vista do dispatcher.

---

### V0.0.3 — Expansão de Backends (planejado)

**Objetivo:** separar execução de backends e introduzir escalabilidade de saída.

- backend de arquivo
- workers assíncronos para backends
- processamento em batch
- modos de execução de backend (`INLINE / ASYNC / BATCHED`)
- isolamento básico de backends (dispatcher não bloqueante)
- métricas aprimoradas de overflow
- primeira camada de observabilidade da saúde do logger
- filtragem em tempo de compilação

> Foco: escalabilidade das saídas, não do modelo central de concorrência.

---

### V0.0.4 — Camada de Observabilidade (planejado)

**Objetivo:** transformar o logger em fonte de telemetria da própria engine.

- suporte a logging estruturado (payloads não textuais)
- integração com sistema de métricas (profundidade da fila, taxa de descarte, latência)
- diagnóstico e classificação de overflow
- políticas de sampling e throttling
- caminho de logging crash-safe (emergency sink)
- monitoramento da saúde dos backends

> Foco: introspecção e observabilidade sistêmica.

---

### V0.0.5 — Pipeline Avançado (futuro)

**Objetivo:** evoluir para logging distribuído e integração profunda com sistemas da engine.

- logging via rede / telemetria remota
- integração com job system
- suporte a tracing entre threads
- suporte a replay / reconstrução de eventos
- streams persistentes de log
- batching avançado e compressão
- políticas adaptativas de logging (sensíveis à carga)

> Foco: integração do ecossistema e observabilidade distribuída.
