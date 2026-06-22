# Dynora Engine — Core Rules

**Status:** Active  
**Version:** 0.0.1  
**Last Updated:** 2026-05-22  
**Owner:** André Gustavo

---

## 1. Propósito

Este documento define as regras globais de engenharia da Dynora Engine.

Ele existe para manter consistência entre subsistemas, evitar decisões contraditórias e preservar a direção arquitetural do projeto ao longo do tempo.

As regras aqui descritas se aplicam a todos os módulos, subsistemas, backends, ferramentas e extensões da engine, salvo exceção documentada de forma explícita em uma ADR ou SPEC.

---

## 2. Papel deste documento

`core_rules.md` define as leis arquiteturais da Dynora.

Ele responde a perguntas como:

- o que nunca pode ser violado;
- quais princípios são globais;
- quais restrições se aplicam a todo o sistema;
- quais decisões locais precisam obedecer a contratos superiores.

Este documento não define implementação nem comportamento detalhado de subsistemas. Ele define limites.

---

## 3. Hierarquia documental

```text
CORE RULES
  ↓
ADR
  ↓
SPEC
  ↓
IMPLEMENTATION
```

### Regras da hierarquia

- Camadas superiores prevalecem sobre inferiores.
- ADRs devem respeitar as Core Rules.
- SPECs definem comportamento dentro dos limites arquiteturais.
- O código deve seguir a SPEC vigente.

---

## 4. Classificação de estabilidade

Nem tudo na Dynora tem o mesmo nível de maturidade. Para evitar confusão entre ideia, contrato e decisão consolidada, a documentação e as APIs devem ser classificadas por estabilidade.

### Níveis

- **Experimental** — ainda em validação, sujeito a mudanças frequentes.
- **Internal** — usado internamente pela engine, mas não exposto como contrato público.
- **Stable** — comportamento esperado e suportado.
- **Frozen** — contrato preservado com mudanças mínimas.
- **Deprecated** — ainda existe, mas está em processo de remoção.

Recursos experimentais não devem ser tratados como compromisso permanente.

---

## 5. Princípios fundamentais

### 5.1 Modularidade explícita

A engine deve ser composta por sistemas independentes e cooperativos.

- cada subsistema deve ter responsabilidade clara;
- dependências entre módulos devem ser explícitas;
- acoplamento desnecessário deve ser evitado;
- módulos devem poder evoluir sem exigir reescrita do núcleo.

### 5.2 Previsibilidade

O comportamento da engine deve ser previsível em execução, memória e ciclo de vida.

- custos relevantes devem ser conhecidos;
- comportamentos sob carga devem ser definidos;
- bloqueios implícitos devem ser evitados;
- efeitos colaterais não documentados não são aceitáveis.

### 5.3 Evolução incremental

A Dynora deve evoluir por etapas pequenas e auditáveis.

- cada versão deve validar um contrato claro;
- mudanças amplas devem ser divididas em fases;
- o projeto não deve depender de uma reescrita total;
- a arquitetura deve permitir crescimento sem ruptura.

### 5.4 Performance como requisito localizado

Performance é requisito em caminhos críticos identificados por profiling, SPEC ou decisão arquitetural formal.

- hot paths devem ser identificados;
- custos recorrentes devem ser controlados;
- alocações desnecessárias devem ser evitadas;
- operações bloqueantes não devem aparecer em caminhos críticos sem justificativa formal.

### 5.5 Contratos explícitos

Toda regra importante deve ser documentada.

- o comportamento do sistema deve ser verificável;
- ownership, lifetime e threading devem ser explícitos quando relevantes;
- ambiguidade contratual deve ser eliminada;
- suposições implícitas não são aceitas como contrato.

---

## 6. Regras globais de projeto

### 6.1 Memory bounded sempre que possível

Sistemas centrais da engine devem preferir memória bounded e comportamento previsível.

- estruturas centrais devem ter capacidade definida quando isso for viável;
- crescimento ilimitado deve ser evitado em subsistemas críticos;
- uso de heap deve ser justificado quando ocorrer em hot path;
- saturação deve ter comportamento definido.

### 6.2 Sem bloqueio em hot path

Caminhos críticos não devem bloquear por I/O, sincronização excessiva ou dependências externas.

- threads principais da engine não devem ficar presas em espera desnecessária;
- operações lentas devem ser isoladas quando possível;
- sistemas de observabilidade, telemetria e escrita em disco não devem interferir no fluxo crítico sem contrato explícito.

### 6.3 Ownership explícito

Toda transferência de dados sensível deve ter regras claras de ownership.

- deve ficar claro quem cria, quem usa e quem libera;
- referências temporárias não devem ser armazenadas sem garantia de lifetime;
- estruturas compartilhadas devem declarar claramente seu modelo de posse;
- código que retém dados deve copiar ou serializar quando necessário.

### 6.4 Concorrência explícita

Toda concorrência deve ser modelada e documentada.

- não assumir thread-safety por padrão;
- não compartilhar estado mutável sem estratégia definida;
- modelos MPSC, SPSC, lock-free ou mutex-based devem ser escolhidos conscientemente;
- a ausência de sincronização deve ser considerada parte do contrato.

### 6.5 Falha deve ser explícita

Falhas não devem ser silenciosas quando houver alternativa segura de observação.

- erro ignorado é dívida técnica;
- comportamento indefinido ou ambíguo deve ser evitado;
- quando algo falhar, o sistema deve falhar de forma previsível ou degradar de forma documentada;
- subsistemas críticos devem expor sinais claros de erro, overflow ou rejeição.

---

## 7. Regras de arquitetura de subsistemas

### 7.1 Separação entre decisão e implementação

Decisões arquiteturais pertencem a ADRs. Implementação pertence ao código e à SPEC.

- código não deve carregar decisões não registradas;
- SPEC não deve explicar alternativas rejeitadas em excesso;
- ADR não deve substituir SPEC;
- documentação de sistema não deve virar repositório informal de ideias.

### 7.2 Cada sistema tem uma responsabilidade principal

- o logger observa e registra;
- o ECS organiza entidades e componentes;
- o renderer processa a saída visual;
- o audio lida com áudio;
- tools auxiliam o desenvolvimento, mas não substituem o runtime principal.

Se um sistema começar a acumular responsabilidades demais, ele deve ser dividido ou reavaliado.

### 7.3 Backends são extensões, não o núcleo

- backends não devem controlar o fluxo central da engine;
- o núcleo não deve depender de um backend específico para funcionar;
- sinks de saída devem ser substituíveis quando possível;
- integrações externas não devem impor comportamento ao core sem contrato formal.

### 7.4 Direção de dependência

As dependências devem seguir uma direção arquitetural clara.

A direção padrão é:

```text
Plugins → Modules → Core
```

- o Core não depende de módulos;
- módulos não devem depender diretamente entre si sem contrato formal;
- plugins não podem se tornar dependências estruturais;
- dependências reversas são proibidas.

### 7.5 Interfaces públicas e ABI

Interfaces públicas devem ser tratadas como contrato estável.

- ownership deve ser explícito;
- estruturas internas não devem ser expostas sem necessidade;
- lifetime e threading precisam estar claros;
- mudanças incompatíveis devem ser versionadas;
- ABI pública não deve mudar sem justificativa e plano de transição.

---

## 8. Regras de documentação e evolução

### 8.1 Documentos oficiais

- `core_rules.md` — leis globais da engine;
- ADRs — decisões arquiteturais;
- SPECs — contratos por versão de sistema;
- `roadmap.md` — direção e prioridade de evolução;
- `design` — exploração e experimentação.

### 8.2 Mudança de regras

- `core_rules.md` deve mudar raramente;
- uma mudança aqui deve ser tratada como decisão relevante;
- regras instáveis não pertencem a este documento;
- exceções devem ser documentadas de forma explícita.

### 8.3 Exceções e validade temporária

Exceções são permitidas apenas quando:

- há justificativa técnica clara;
- o impacto foi avaliado;
- a exceção está documentada;
- existe um limite definido para a exceção;
- a exceção não contradiz o objetivo central da engine.

Exceções recorrentes devem ser promovidas a regra formal ou removidas.

---

## 9. Qualidade técnica

### 9.1 Clareza antes de sofisticação

- a solução mais simples que satisfaz o contrato deve ser preferida;
- complexidade só é aceita quando traz benefício claro;
- abstrações prematuras devem ser evitadas;
- elegância não deve sacrificar auditabilidade.

### 9.2 Testabilidade

- comportamento relevante deve ser testável;
- invariantes críticas devem poder ser verificadas;
- contratos frágeis devem ser acompanhados por testes;
- a documentação deve facilitar validação, não dificultá-la.

### 9.3 Observabilidade

- sistemas críticos devem ser observáveis;
- métricas, logs e diagnósticos devem existir quando necessários;
- ausência de visibilidade é risco arquitetural;
- os subsistemas devem expor sinais que ajudem a depurar comportamento real.

---

## 10. Anti-padrões

Os seguintes comportamentos são desencorajados na Dynora:

- documento monolítico que tenta explicar tudo;
- sistema central que concentra responsabilidade demais;
- estado global sem contrato;
- dependências circulares desnecessárias;
- thread-safety presumida sem prova;
- bloqueio em caminho crítico sem justificativa;
- decisão arquitetural não registrada;
- SPEC que tenta ser roadmap;
- ADR que tenta ser implementação;
- feature futura tratada como contrato presente.

---

## 11. Resumo normativo

A Dynora Engine deve seguir os seguintes mandamentos:

- manter modularidade explícita;
- evitar bloqueio em hot path;
- preferir memória bounded;
- definir ownership com clareza;
- documentar concorrência de forma explícita;
- separar decisão, contrato e implementação;
- evoluir por etapas pequenas;
- manter observabilidade adequada;
- evitar complexidade sem benefício;
- preservar previsibilidade arquitetural.

---

## 12. Conclusão

Estas são as leis arquiteturais da Dynora Engine.

Qualquer sistema deve obedecer a este documento. Em caso de conflito, esta camada prevalece sobre as demais.
