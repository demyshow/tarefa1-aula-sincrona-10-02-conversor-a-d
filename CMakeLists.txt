# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# Generated CMake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Detecta se é Windows ou Linux/macOS
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)

# Verifica se o arquivo pico-vscode.cmake existe e o inclui
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

# Define a placa utilizada (Pico W)
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Importa o Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)

# Define o projeto
project(tarefa1-aula-sincrona-10-02-conversor-a-d C CXX ASM)

# Inicializa o Raspberry Pi Pico SDK
pico_sdk_init()

# Adiciona o executável principal e os arquivos necessários
add_executable(tarefa1-aula-sincrona-10-02-conversor-a-d 
    tarefa1-aula-sincrona-10-02-conversor-a-d.c
    lib/ssd1306.c  # Inclui a implementação do SSD1306
)

# Configurações do programa
pico_set_program_name(tarefa1-aula-sincrona-10-02-conversor-a-d "tarefa1-aula-sincrona-10-02-conversor-a-d")
pico_set_program_version(tarefa1-aula-sincrona-10-02-conversor-a-d "0.1")

# Habilita saída via UART e USB
pico_enable_stdio_uart(tarefa1-aula-sincrona-10-02-conversor-a-d 1)
pico_enable_stdio_usb(tarefa1-aula-sincrona-10-02-conversor-a-d 1)

# Adiciona bibliotecas necessárias para o build
target_link_libraries(tarefa1-aula-sincrona-10-02-conversor-a-d
    pico_stdlib
    hardware_adc
    hardware_gpio
    hardware_pwm
    hardware_i2c
)

# Inclui a pasta lib para acessar os headers corretamente
target_include_directories(tarefa1-aula-sincrona-10-02-conversor-a-d PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/lib  # Inclui a pasta 'lib' para headers como ssd1306.h e font.h
)

# Gera arquivos binários extras (.uf2, .bin, etc.)
pico_add_extra_outputs(tarefa1-aula-sincrona-10-02-conversor-a-d)
