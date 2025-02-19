/*
 * Projeto: Controle de Joystick com Display OLED e LEDs PWM
 * Autor: Edemir de Carvalho Rodrigues
 * Descrição:
 *   - Controle de display OLED 128x64 com joystick analógico
 *   - LEDs PWM controlados pelo joystick
 *   - Botões para controle de borda e modo BOOTSEL
 *   - Uso de interrupções (IRQ) e debounce via software
 *   - Compatível com Wokwi e hardware físico
 */

 #include <stdio.h>
 #include "pico/stdlib.h"
 #include "hardware/adc.h"
 #include "hardware/i2c.h"
 #include "hardware/pwm.h"
 #include "hardware/gpio.h"
 #include "lib/ssd1306.h"
 #include "pico/bootrom.h"
 
 // Definições de hardware
 #define I2C_PORT i2c1  // Definição da porta I2C utilizada
 #define I2C_SDA 14      // Pino SDA do I2C
 #define I2C_SCL 15      // Pino SCL do I2C
 #define DISPLAY_ADDR 0x3C  // Endereço I2C do display OLED
 
 // Definições dos pinos do joystick e botões
 #define JOYSTICK_X 27
 #define JOYSTICK_Y 26
 #define JOYSTICK_BTN 22
 #define BUTTON_A 5
 #define BUTTON_B 6
 
 // Definições dos pinos dos LEDs
 #define LED_GREEN 11
 #define LED_BLUE 12
 #define LED_RED 13
 
 // Definições do display
 #define WIDTH 128
 #define HEIGHT 64
 
 // Centro e zona morta do joystick
 #define JOYSTICK_CENTER_X 1929
 #define JOYSTICK_CENTER_Y 2019
 #define DEADZONE 100
 
 // Instância do display OLED
 ssd1306_t ssd;
 
 // Variáveis globais de controle
 bool pwm_enabled = true;
 int border_style = 1;
 static uint32_t last_interrupt_time = 0;
 
 // Função para configurar o PWM em um pino
 void setup_pwm(uint pin) {
     gpio_set_function(pin, GPIO_FUNC_PWM);  // Configura o pino para função PWM
     uint slice = pwm_gpio_to_slice_num(pin);
     pwm_set_wrap(slice, 4095);  // Define o valor máximo do PWM
     pwm_set_enabled(slice, true);  // Habilita o PWM
     pwm_set_gpio_level(pin, 0);  // Inicializa com nível baixo
 }
 
 // Função de callback para interrupções dos botões
 void button_callback(uint gpio, uint32_t events) {
     uint32_t current_time = to_ms_since_boot(get_absolute_time());
     if (current_time - last_interrupt_time < 50) return; // Debounce via software
     last_interrupt_time = current_time;
 
     if (gpio == JOYSTICK_BTN) {
         static bool green_led_state = false;
         green_led_state = !green_led_state;
         pwm_set_gpio_level(LED_GREEN, green_led_state ? 2047 : 0);  // Alterna o LED verde
         border_style = (border_style == 1) ? 2 : 1;  // Alterna o estilo da borda
         printf("[BOTÃO] Bordas: %d\n", border_style);
     } else if (gpio == BUTTON_A) {
         pwm_enabled = !pwm_enabled;  // Alterna o estado do PWM
         printf("[PWM] Estado: %s\n", pwm_enabled ? "Ativado" : "Desativado");
     } else if (gpio == BUTTON_B) {
         printf("[SISTEMA] Entrando em modo BOOTSEL\n");
         reset_usb_boot(0, 0);  // Reinicia para modo BOOTSEL
     }
 }
 
 // Função para configurar as interrupções dos botões
 void setup_interrupts() {
     gpio_set_irq_enabled_with_callback(JOYSTICK_BTN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
     gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);
     gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &button_callback);
 }
 
 int main() {
     stdio_init_all();  // Inicializa a comunicação serial
 
     // Configuração dos botões como entrada com pull-up
     gpio_init(BUTTON_A);
     gpio_set_dir(BUTTON_A, GPIO_IN);
     gpio_pull_up(BUTTON_A);
     
     gpio_init(BUTTON_B);
     gpio_set_dir(BUTTON_B, GPIO_IN);
     gpio_pull_up(BUTTON_B);
     
     gpio_init(JOYSTICK_BTN);
     gpio_set_dir(JOYSTICK_BTN, GPIO_IN);
     gpio_pull_up(JOYSTICK_BTN);
 
     // Inicializa o ADC e configura os pinos do joystick
     adc_init();
     adc_gpio_init(JOYSTICK_X);
     adc_gpio_init(JOYSTICK_Y);
 
     // Configura os LEDs PWM
     setup_pwm(LED_RED);
     setup_pwm(LED_BLUE);
     setup_pwm(LED_GREEN);
 
     // Configura a comunicação I2C
     i2c_init(I2C_PORT, 400 * 1000);
     gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
     gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
     gpio_pull_up(I2C_SDA);
     gpio_pull_up(I2C_SCL);
 
     // Inicializa o display OLED
     ssd1306_init(&ssd, WIDTH, HEIGHT, false, DISPLAY_ADDR, I2C_PORT);
     ssd1306_config(&ssd);
     ssd1306_fill(&ssd, false);
     ssd1306_send_data(&ssd);
 
     setup_interrupts();  // Configura as interrupções
     
     int x_pos = 59, y_pos = 29;  // Posição inicial do cursor
 
     while (true) {
         // Leitura do joystick
         adc_select_input(0);
         uint16_t x_val = adc_read();
         adc_select_input(1);
         uint16_t y_val = adc_read();
 
         int adjusted_x = x_val - JOYSTICK_CENTER_X;
         int adjusted_y = y_val - JOYSTICK_CENTER_Y;
 
         // Ajusta a posição com base no joystick
         if (abs(adjusted_y) > DEADZONE)
             x_pos += (adjusted_y * 5) / 2048;
         if (abs(adjusted_x) > DEADZONE)
             y_pos -= (adjusted_x * 5) / 2048;
 
         // Limita a posição dentro da tela
         if (x_pos < 0) x_pos = 0;
         if (x_pos > WIDTH - 8) x_pos = WIDTH - 8;
         if (y_pos < 0) y_pos = 0;
         if (y_pos > HEIGHT - 8) y_pos = HEIGHT - 8;
 
         // Inverte coordenadas para exibição correta no display
         int inverted_x_pos = y_pos;
         int inverted_y_pos = x_pos;
 
         // Atualiza o display
         ssd1306_fill(&ssd, false);
         ssd1306_rect(&ssd, inverted_x_pos, inverted_y_pos, 8, 8, true, true);
         ssd1306_send_data(&ssd);
 
         // Atualiza os LEDs PWM
         if (pwm_enabled) {
             pwm_set_gpio_level(LED_RED, adjusted_y);
             pwm_set_gpio_level(LED_BLUE, adjusted_x);
         }
         
        sleep_ms(20);  // Pequena pausa para suavizar a leitura
     }
 }
 