# 🎮 Projeto: Controle de Joystick com Display OLED e LEDs PWM

## 📌 Unidade 4 - Capítulo 8: Conversores A/D

## 🎯 Objetivos

- Compreender o funcionamento do conversor analógico-digital (**ADC**) no **RP2040**.
- Utilizar o **PWM** para controlar a intensidade de dois **LEDs RGB** com base nos valores do joystick.
- Representar a posição do **joystick** no **display SSD1306** por meio de um quadrado móvel.
- Aplicar o protocolo de comunicação **I2C** na integração com o display.

---

## 🛠️ Descrição do Projeto

O **joystick** fornecerá valores analógicos correspondentes aos eixos **X e Y**, que serão utilizados para:

### 📟 **Controle do Display OLED**

- Exibir um **quadrado de 8x8 pixels**, inicialmente centralizado, que se moverá proporcionalmente aos valores do joystick.
- O **botão do joystick** modificará a borda do display para indicar quando foi pressionado, alternando entre diferentes estilos de borda.

### 💡 **Controle dos LEDs RGB via PWM**

- **LED Azul:** Ajusta seu brilho conforme o valor do eixo **Y** do joystick.
  - **Posição central (2048):** LED **apagado**.
  - **Movendo para cima/baixo:** Brilho aumenta progressivamente até atingir a intensidade máxima nos extremos **(0 e 4095).**
- **LED Vermelho:** Segue o mesmo princípio, mas de acordo com o eixo **X** do joystick.
  - **Posição central (2048):** LED **apagado**.
  - **Movendo para esquerda/direita:** Brilho aumenta nos extremos **(0 e 4095).**
- **LED Verde:** Alterna seu estado a cada acionamento do botão do joystick.

### 🔘 **Botões e suas funcionalidades**

- **Botão do Joystick (GPIO 22):** Alterna o LED verde e modifica a borda do display.
- **Botão A (GPIO 5):** Ativa/desativa os LEDs PWM a cada acionamento.

---

## ✅ **Requisitos do Projeto**

1. **Uso de interrupções (IRQ):** Todas as funcionalidades dos botões devem ser implementadas via **interrupção**.
2. **Debouncing via software:** Deve ser implementado para evitar leituras erradas nos botões.
3. **Utilização do Display 128x64:** Para demonstrar o entendimento do **protocolo I2C**.
4. **Organização do código:** O código deve ser **bem estruturado e comentado**.

---

## 🖥️ **Configuração do Ambiente**

### 📦 **Instalações Necessárias**

- **Pico SDK** configurado.
- **CMake e GNU ARM Toolchain** instalados.
- **Visual Studio Code** com extensões:
  - C/C++
  - CMake Tools
  - Raspberry Pi Pico
  - Wokwi Simulator

### 🔌 **Conexões de Hardware**

| **Componente**         | **Pino GPIO** |
| ---------------------- | ------------- |
| **I2C SDA**            | 14            |
| **I2C SCL**            | 15            |
| **Joystick X**         | 27            |
| **Joystick Y**         | 26            |
| **Botão Joystick**     | 22            |
| **Botão A**            | 5             |
| **LED Verde (PWM)**    | 11            |
| **LED Azul (PWM)**     | 12            |
| **LED Vermelho (PWM)** | 13            |

---

## 🗁 **Estrutura do Código**

```
📂 C:\tarefa1-aula-sincrona-10-02-conversor-a-d\tarefa1-aula-sincrona-10-02-conversor-a-d
│── 📄 tarefa1-aula-sincrona-10-02-conversor-a-d.c  # Código principal
│── 📄 CMakeLists.txt   # Configuração para compilação no Pico SDK
│── 📂 lib/             # Biblioteca SSD1306 para controle do display OLED
│── 📄 wokwi.toml       # Arquivo de configuração para o simulador Wokwi
│── 📄 README.md        # Documentação do projeto
```

---

## 📸 **Demonstração e Entrega**

### **📝 Código-fonte**

O código do projeto deve ser enviado em um **repositório GitHub**, contendo todos os arquivos necessários para sua execução.

Repositório GitHub: [Clique aqui](https://github.com/demyshow/tarefa1-aula-sincrona-10-02-conversor-a-d)

### **🎥 Vídeo de demonstração**

O vídeo deve ter até **2 minutos**, mostrando:

1. O **aluno demonstrando** o funcionamento do projeto.
2. **Breve explicação** das funcionalidades implementadas.
3. **Execução do projeto** na placa BitDogLab.
4. O link do vídeo deve ser disponibilizado via **YouTube ou Google Drive**.

📹 [Assista à demonstração do projeto](https://drive.google.com/file/d/1_u4lbmd2xpB1GaAip_uXO_BMA7DMgT1u/view?usp=drive_link)

---

## 📦 **Como Clonar e Executar o Projeto**

```sh
git clone https://github.com/demyshow/tarefa1-aula-sincrona-10-02-conversor-a-d.git
cd C:\tarefa1-aula-sincrona-10-02-conversor-a-d
```

Compile e execute o código usando o **Pico SDK**.

Após clonar o projeto, ele deve ser importado no Raspberry Pi via o seguinte caminho:

```
C:\tarefa1-aula-sincrona-10-02-conversor-a-d\tarefa1-aula-sincrona-10-02-conversor-a-d
```

---

Este documento resume o projeto, apresentando seus objetivos, funcionamento e implementação, garantindo que qualquer interessado possa replicá-lo de maneira clara e objetiva.