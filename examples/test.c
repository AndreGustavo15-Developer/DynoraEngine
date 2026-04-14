#include "log/grape_system_log.h"

// protótipo do backend console
void grape_backend_console_init(void);

int main(void) {
    // 1. Inicializa o logger
    grape_log_init(GRAPE_LEVEL_DEBUG, GRAPE_LOG_ALL);

    // 2. Inicializa o backend coansole
    grape_backend_console_init();
    // 3. Testa logs
    
    GRAPE_LOG_DEBUG(GRAPE_LOG_RENDER, "Shader carregado com sucesso!");
    GRAPE_LOG_INFO(GRAPE_LOG_AUDIO, "Audio system iniciado");
    GRAPE_LOG_WARNING(GRAPE_LOG_PHYSICS, "Objeto fora do mundo físico");
    GRAPE_LOG_ERROR(GRAPE_LOG_ECS, "Falha ao criar entidade");
    GRAPE_LOG_FATAL(GRAPE_LOG_GENERAL, "CAVALO");
    GRAPE_LOG_FATAL(GRAPE_LOG_IO, "Erro crítico: não foi possível salvar o arquivo");
    return 0;
}