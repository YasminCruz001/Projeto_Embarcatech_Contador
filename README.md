# Indicador de Concentração e Produtividade
Yasmin Damasceno Cruz - TIC370101610  
Projeto Final - Sistemas Embarcados

Este projeto implementa um temporizador utilizando o microcontrolador RP2040, com exibição do tempo restante em um display OLED SSD1306 e indicação visual através de um LED RGB. O buzzer é utilizado para alertar o término do tempo.

## Componentes Utilizados

- 1x **BitDogLab (RP2040)**
- 1x **Display OLED SSD1306 (I2C - GPIO 14 e 15)**
- 1x **LED RGB (GPIOs 11, 12 e 13)**
- 1x **Buzzer (GPIO 21)**
- 2x **Botões (GPIOs 5 e 6)**

## Configuração Inicial

1. Clone o repositório para sua máquina local:
   ```bash
   git clone <https://github.com/YasminCruz001/Projeto_Embarcatech_Contador.git>
   ```
2. Abra o projeto no VS Code.
3. Compile e carregue o código na placa **BitDogLab**.
    4. Ou inicie a simulação no **Wokwi**.

## Funcionalidades do Projeto

### 1. Definição do Tempo
- O botão **A** permite definir o tempo desejado (1 a 20 minutos).
- O tempo é exibido no **display OLED SSD1306**.

### 2. Início e Pausa da Contagem
- O botão **B** inicia ou pausa o temporizador.
- O tempo restante é atualizado no **display OLED**.

### 3. Indicação Visual com LED RGB
- **Verde**: Tempo inicial (mais da metade restante).
- **Amarelo**: Metade do tempo.
- **Vermelho**: Últimos 5 segundos.

### 4. Alarme Sonoro com Buzzer
- Ao término do tempo, o **buzzer** emite um alerta sonoro.
- O display exibe a mensagem **"TEMPO ESGOTADO!"**.

## Execução

1. Compile e carregue o código na **BitDogLab**.
2. Inicie a simulação no **Wokwi**.
3. Defina o tempo com o botão **A**.
4. Inicie ou pause a contagem com o botão **B**.
5. Observe a mudança de cor do **LED RGB** e a atualização do **display**.
6. Aguarde o alarme sonoro ao final do tempo.

## Vídeo Demonstrativo