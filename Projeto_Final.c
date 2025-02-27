#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"

// Definição dos pinos dos LEDs RGB
#define LED_GREEN 11
#define LED_YELLOW 12 
#define LED_RED 13

// Definição dos pinos dos botões e buzzer
#define BTN_A_PIN 5
#define BTN_B_PIN 6
#define BUZZER_PIN 21

// Definição da interface I2C e pinos correspondentes
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO 0x3C // Endereço do display SSD1306

// Variáveis globais
ssd1306_t ssd; // Estrutura para o display OLED
uint8_t tempo_definido = 0; // Tempo definido pelo usuário (em minutos)
uint8_t tempo_restante = 0; // Tempo restante (em segundos)
bool contador_ativo = false; // Estado do contador (ativo ou inativo)
bool contador_pausado = false; // Estado de pausa do contador

// Função para acionar o buzzer por uma determinada duração em ms
void acionar_buzzer(int duracao_ms) {
    int tempo_restante = duracao_ms;
    while (tempo_restante > 0) {
        gpio_put(BUZZER_PIN, true);  // Liga o buzzer
        sleep_ms(1);                 // Aguarda 1 ms
        tempo_restante--;
        gpio_put(BUZZER_PIN, false); // Desliga o buzzer
        sleep_ms(1);                 // Aguarda mais 1 ms
        tempo_restante--;
    }
}

// Inicializa os pinos dos LEDs e botões
void init_pins() {
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_put(LED_RED, false);

    gpio_init(LED_YELLOW);  // Alterado para LED_YELLOW
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_put(LED_YELLOW, false);

    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_put(LED_GREEN, false);

    gpio_init(BTN_A_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_pull_up(BTN_A_PIN);

    gpio_init(BTN_B_PIN);
    gpio_set_dir(BTN_B_PIN, GPIO_IN);
    gpio_pull_up(BTN_B_PIN);

    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
}

// Função para verificar o botão B com debounce melhorado
bool verificar_botao_b() {
    static bool estado_ultimo = true; // O botão começa como "não pressionado"
    bool estado_atual = gpio_get(BTN_B_PIN);

    if (estado_atual == false && estado_ultimo == true) {
        // O botão foi pressionado
        estado_ultimo = false;
        return true;
    } else if (estado_atual == true) {
        // O botão foi solto
        estado_ultimo = true;
    }

    return false;
}

// Função para atualizar o LED RGB conforme o tempo
// Função para atualizar os LEDs conforme o tempo restante
void atualizar_led_rgb() {
    gpio_put(LED_GREEN, 0);
    gpio_put(LED_RED, 0);
    gpio_put(LED_YELLOW, 0);  // Desliga todos os LEDs antes de definir o estado correto

    if (tempo_restante > (tempo_definido * 60 / 2)) {
        // Verde: tempo inicial
        gpio_put(LED_GREEN, 1);
    } else if (tempo_restante > 5) {
        // Amarelo: apenas o LED amarelo (no Wokwi, um LED separado)
        gpio_put(LED_YELLOW, 1);  // Alterado para LED_YELLOW
    } else {
        // Vermelho: últimos 5 segundos
        gpio_put(LED_RED, 1);
    }
}

// Função para exibir o tempo no display
void exibir_tempo() {
    char buffer[16];
    uint8_t minutos = tempo_restante / 60;
    uint8_t segundos = tempo_restante % 60;
    snprintf(buffer, sizeof(buffer), "%02d:%02d", minutos, segundos);

    ssd1306_fill(&ssd, false); // Limpa o display
    ssd1306_draw_string(&ssd, buffer, 40, 28); // Exibe o tempo no centro
    ssd1306_send_data(&ssd);
}

// Configuração e inicialização do display OLED SSD1306
void init_display() {
    // Inicializa o barramento I2C
    i2c_init(I2C_PORT, 400 * 1000);             // Configura a taxa de 400kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);  // Configura o pino SDA
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);  // Configura o pino SCL
    gpio_pull_up(I2C_SDA);                      // Habilita pull-up para SDA
    gpio_pull_up(I2C_SCL);                      // Habilita pull-up para SCL

    // Inicializa o SSD1306
    ssd1306_init(&ssd, 128, 64, false, ENDERECO, I2C_PORT);
    ssd1306_config(&ssd);       // Configuração do display
    ssd1306_send_data(&ssd);    // Envia dados para o display
    ssd1306_fill(&ssd, false);  // Preenche com cor (falso para limpar)
    ssd1306_send_data(&ssd);    // Atualiza a tela
}

// Função para emitir som com o buzzer
void emitir_som(int frequencia_hz, int duracao_ms) {
    // Frequência de operação (em Hz) e duração (em ms) para o buzzer
    int tempo_restante = duracao_ms;
    while (tempo_restante > 0) {
        gpio_put(BUZZER_PIN, true);  // Liga o buzzer
        sleep_us(1000000 / frequencia_hz);  // Aguarda o tempo correspondente à frequência
        gpio_put(BUZZER_PIN, false); // Desliga o buzzer
        sleep_us(1000000 / frequencia_hz);  // Aguarda o tempo correspondente à frequência
        tempo_restante -= 1000 / frequencia_hz;  // Decrementa a duração do som
    }
}

// Função principal
int main() {
    stdio_init_all();
    init_pins();
    init_display();

    // Exibe uma mensagem inicial no display
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Defina o Tempo", 10,15);
    ssd1306_send_data(&ssd);
    sleep_ms(2000); // Aguarda 2 segundos

    while (1) {
        // Verifica se o Botão A foi pressionado (definir tempo)
        if (!gpio_get(BTN_A_PIN)) {
            tempo_definido = (tempo_definido + 1) % 21; // Incrementa o tempo (1 a 20 minutos)
            tempo_restante = tempo_definido * 60; // Converte para segundos
            exibir_tempo();
            sleep_ms(300); // Debounce
        }

        // Verifica o estado do botão B (iniciar/pausar/retomar)
        if (verificar_botao_b()) {
            if (!contador_ativo) {
                // Se o contador não estava ativo, inicia
                contador_ativo = true;
                contador_pausado = false;
                printf("Contador iniciado.\n");
            } else {
                // Se o contador já estava ativo, pausa ou retoma
                contador_pausado = !contador_pausado;
                if (contador_pausado) {
                    printf("Contador pausado.\n");
                } else {
                    printf("Contador retomado.\n");
                }
            }
            sleep_ms(300); // Debounce adicional
        }

        // Contagem regressiva (se o contador estiver ativo e não pausado)
        if (contador_ativo && !contador_pausado && tempo_restante > 0) {
            exibir_tempo();
            atualizar_led_rgb();
            sleep_ms(1000); // Aguarda 1 segundo
            tempo_restante--;  // Decrementa o tempo
        }

        // Tempo esgotado
        if (contador_ativo && tempo_restante == 0) {
            contador_ativo = false;
            contador_pausado = false;
            tempo_definido = 0; // Reinicia o tempo definido para 0
            exibir_tempo(); // Exibe 00:00 no display
            sleep_ms(500); // Espera meio segundo para garantir que o display seja atualizado
            emitir_som(500, 2000); // Toca um som de 2 kHz por 500 ms
            ssd1306_fill(&ssd, false);
            ssd1306_draw_string(&ssd, "TEMPO ESGOTADO!", 10, 28);
            ssd1306_send_data(&ssd);
            sleep_ms(2000); // Exibe a mensagem por 2 segundos
        }
    }

    return 0;
}
