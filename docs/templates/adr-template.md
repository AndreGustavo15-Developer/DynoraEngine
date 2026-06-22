# ADR-XXXX — <Título da decisão>

**Status:** Proposed | Accepted | Superseded | Deprecated  
**Date:** YYYY-MM-DD  
**Deciders:** <quem participou da decisão>  
**Context Version:** <opcional: versão do sistema / release>

---

# 1. Context

Qual problema estamos resolvendo?

- situação atual
- dor / limitação
- requisitos técnicos ou de negócio relevantes
- constraints (performance, custo, escala, etc)

---

# 2. Decision

O que foi decidido?

- decisão clara e direta (sem justificativa aqui)
- abordagem escolhida
- escopo da decisão

---

# 3. Tradeoffs Accepted

Quais compromissos estamos aceitando conscientemente?

- o que estamos priorizando;
- o que NÃO estamos tentando otimizar;
- limitações aceitas;
- complexidades aceitas;
- riscos assumidos.

Exemplo:

- menor flexibilidade em troca de ABI estável;
- memória bounded em vez de crescimento ilimitado;
- menor ergonomia para reduzir alocação em hot path.

---

# 4. Consequences

O que muda com essa decisão?

## Positive
- benefícios esperados

## Negative
- trade-offs
- custos / complexidade / riscos

---

# 5. Operational Impact

Impactos técnicos e operacionais esperados.

- impacto em memória;
- impacto em threading;
- impacto em build time;
- impacto em debugging;
- impacto em profiling;
- impacto em ABI;
- impacto em tooling;
- impacto em runtime behavior.

---

# 6. Alternatives Considered

O que foi considerado e não escolhido?

- opção A + por que foi rejeitada
- opção B + por que foi rejeitada

---

# 7. Invariants (opcional, mas recomendado em sistemas críticos)

Regras que nunca podem ser violadas:

- regra 1
- regra 2

---

# 8. Failure Modes (opcional)

O que acontece quando dá errado?

- comportamento em falha
- fallback
- degradação aceitável

---

# 9. References

- links para issues, PRs, spikes, benchmarks, etc

---

# Attribution

This ADR template is based on the Architecture Decision Record concept originally popularized by Michael Nygard and the ADR community.

Source: https://github.com/architecture-decision-record/architecture-decision-record
