#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

// configuração dos pinos I2C e botões
const uint I2C_SDA = 14;
const uint I2C_SCL = 15;
const uint BUTTON_B = 6;
const uint BUTTON_A = 5;
const uint JOYDOWN = 22;

// definição do teclado
const char keyboard[3][10] = {
    {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', '\0'},
    {'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', '\0'},
    {'Q', 'R', 'S', 'T', 'U', 'V', 'X', 'Z', ' ', '\0'}
};

char palavra[30] = "";
int palavra_index = 0;

// função para exibir feedback da resposta
void correcao_da_resposta(uint8_t* ssd, struct render_area* frame_area, const char* mensagem) {
    memset(ssd, 0, ssd1306_buffer_length); 
    ssd1306_draw_string(ssd, 0, 0, mensagem);
    render_on_display(ssd, frame_area); 
    sleep_ms(1500); 
}

// função para verificar se a palavra é um estado válido
int is_valid_state(const char* word) {
    const char* states[] = {
        "ACRE", "ALAGOAS", "AMAPA", "AMAZONAS", "BAHIA", "CEARA",
        "DISTRITO FEDERAL", "ESPIRITO SANTO", "GOIAS", "MARANHAO",
        "MATO GROSSO", "MATO GROSSO DO SUL", "MINAS GERAIS", "PARA",
        "PARAIBA", "PARANA", "PERNAMBUCO", "PIAUI", "RIO DE JANEIRO",
        "RIO GRANDE DO NORTE", "RIO GRANDE DO SUL", "RONDONIA", "RORAIMA",
        "SANTA CATARINA", "SAO PAULO", "SERGIPE", "TOCANTINS" };
    int num_states = sizeof(states) / sizeof(states[0]);

    for (int i = 0; i < num_states; i++) {
        if (strcmp(word, states[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int main() {
    stdio_init_all();

    // inicialização do I2C
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // inicialização do ADC e GPIO
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    gpio_init(JOYDOWN);
    gpio_set_dir(JOYDOWN, GPIO_IN);
    gpio_pull_up(JOYDOWN);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // inicialização do display
    ssd1306_init();

    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    // mensagem de boas-vindas
    ssd1306_draw_string(ssd, 20, 20, "AprendoTech");
    render_on_display(ssd, &frame_area);
    sleep_ms(1800);
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    ssd1306_draw_string(ssd, 30, 20, "iniciar");
    render_on_display(ssd, &frame_area);
    sleep_ms(1500);
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    ssd1306_draw_string(ssd, 5, 10, "digite o nome");
    ssd1306_draw_string(ssd, 7, 30, "de um estado");
    ssd1306_draw_string(ssd, 10, 40, " brasileiro");

    render_on_display(ssd, &frame_area);
    sleep_ms(2500);
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    int cursor_x = 0, cursor_y = 0;
    static int button_pressed = 0;
    static int button_a_pressed = 0;
    static int button_b_pressed = 0;

    while (true) {
        adc_select_input(0);
        uint adc_x_raw = adc_read();
        adc_select_input(1);
        uint adc_y_raw = adc_read();

        int new_x = cursor_x;
        int new_y = cursor_y;

        // movimentação do joystick
        if (adc_x_raw < 1000) new_x = (cursor_x > 0) ? cursor_x - 1 : cursor_x;
        if (adc_x_raw > 3000) new_x = (cursor_x < 9) ? cursor_x + 1 : cursor_x;
        if (adc_y_raw < 1000) new_y = (cursor_y > 0) ? cursor_y - 1 : cursor_y;
        if (adc_y_raw > 3000) new_y = (cursor_y < 2) ? cursor_y + 1 : cursor_y;

        if (new_x != cursor_x || new_y != cursor_y) {
            cursor_x = new_x;
            cursor_y = new_y;

            memset(ssd, 0, ssd1306_buffer_length);

            if (palavra_index > 0) {
                ssd1306_draw_string(ssd, 0, 0, palavra);
            }

            // desenha o teclado na tela
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 10; col++) {
                    char str[2] = {keyboard[row][col], '\0'};
                    int x_pos = col * 12 + 3;
                    int y_pos = (row + 1) * 18;
                    if (row == cursor_y && col == cursor_x) {
                        ssd1306_draw_string(ssd, x_pos, y_pos, " ");
                        ssd1306_draw_string(ssd, x_pos + 10, y_pos, str);
                        ssd1306_draw_string(ssd, x_pos + 20, y_pos, "");
                    } else {
                        ssd1306_draw_string(ssd, x_pos + 3, y_pos, str);
                    }
                }
            }
            render_on_display(ssd, &frame_area);
        }

        // seleciona letra com o joystick
        if (gpio_get(JOYDOWN) == 0 && !button_pressed) {
            if (palavra_index < sizeof(palavra) - 1) {
                palavra[palavra_index++] = keyboard[cursor_y][cursor_x];
                palavra[palavra_index] = '\0';
            }
            button_pressed = 1;
        } else if (gpio_get(JOYDOWN) == 1) {
            button_pressed = 0;
        }

        // apagar a última letra com o botão A
        if (gpio_get(BUTTON_A) == 0 && !button_a_pressed) {
            if (palavra_index > 0) {
                palavra[--palavra_index] = '\0';
            }
            button_a_pressed = 1;
        } else if (gpio_get(BUTTON_A) == 1) {
            button_a_pressed = 0;
        }

        // lógica do botão B com debounce e feedback em tela
        if (gpio_get(BUTTON_B) == 0 && !button_b_pressed) {
            button_b_pressed = 1;
            sleep_ms(15); // Debounce

            if (is_valid_state(palavra)) {
                correcao_da_resposta(ssd, &frame_area, "correto!");
                memset(palavra, 0, sizeof(palavra));
                palavra_index = 0;
                // atualiza o display para refletir que a palavra foi apagada
                memset(ssd, 0, ssd1306_buffer_length);
                render_on_display(ssd, &frame_area);
            } else {
                correcao_da_resposta(ssd, &frame_area, "TENTE NOVAMENTE");
            }
        } else if (gpio_get(BUTTON_B) == 1) {
            button_b_pressed = 0;
        }

        // espera um curto período para evitar múltiplas chamadas
        sleep_ms(200);
    }

    return 0;
}