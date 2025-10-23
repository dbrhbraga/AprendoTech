# AprendoTech

Projeto do kit de eletrônica **BitDogLab** com **Raspberry Pi Pico (RP2040)** desenvolvido no contexto do Programa de Capacitação Profissional **EmbarcaTech**. Este repositório contém o código-fonte em C/CMake para compilar com o **Pico SDK** e gravar no microcontrolador.

> 📹 **Demo (vídeo curto):** [https://www.youtube.com/shorts/4A8q-varqKg](https://www.youtube.com/shorts/4A8q-varqKg)

---

## ✨ Visão geral

* **Plataforma:** Raspberry Pi Pico (RP2040)
* **Linguagem:** C
* **Build system:** CMake + Pico SDK
* **Objetivo:** Exercitar conceitos de programação embarcada, GPIO, temporização e periféricos do kit BitDogLab.

---

## 🔌 Hardware

* **Raspberry Pi Pico (RP2040)**
* **Kit BitDogLab** 
* Sensores/atuadores usados neste projeto: *(edite conforme necessário)*
  
  * Botões (push-buttons)
  * Buzzer
  * Display (OLED/SSD1306, LCD 16x2 etc.)
  * Outros periféricos do kit

### Mapeamento de pinos (exemplo — ajuste conforme seu código)

| Função          | GPIO Pico | Observações         |
| --------------- | --------- | ------------------- |
| Botão A         | 14        | pull-up interno     |
| Botão B         | 15        | pull-up interno     |
| Buzzer          | 10        | PWM                 |
| I2C SDA/SCL     | 4 / 5     | Display             |


## 🧰 Pré‑requisitos

* **Toolchain C/C++** (gcc-arm-none-eabi)
* **CMake** ≥ 3.13
* **Pico SDK** configurado localmente
* **Git**
* **cmake/ninja** (opcional, mas recomendado)

No Linux/macOS (exemplo):

```bash
# instale dependências conforme sua distro
sudo apt-get update && sudo apt-get install -y cmake gcc-arm-none-eabi build-essential

# clone o Pico SDK
mkdir -p $HOME/pico && cd $HOME/pico
git clone -b master https://github.com/raspberrypi/pico-sdk
cd pico-sdk
git submodule update --init

# exporte PICO_SDK_PATH no shell de desenvolvimento
export PICO_SDK_PATH=$HOME/pico/pico-sdk
```

No Windows, recomenda-se **WSL** ou o ambiente do **Pico SDK** via CMake/Visual Studio.

---

## 🛠️ Compilação

Assumindo que `PICO_SDK_PATH` está definido no seu ambiente:

```bash
# na raiz do repositório
cd aprendotech
mkdir -p build && cd build
cmake ..
cmake --build .
```

Isso deve gerar um arquivo `.uf2` no diretório `build` (por exemplo, `aprendotech.uf2`).

---

## ⬆️ Gravação no Pico

1. Desconecte o Pico do USB.
2. Pressione e mantenha o botão **BOOTSEL**.
3. Conecte o Pico ao USB e solte o botão.
4. O Pico montará como um disco removível (RPI-RP2).
5. Arraste e solte o arquivo `.uf2` gerado para esse disco.

Após a cópia, o Pico reinicia e executa o programa.

---

## 🧪 Testes rápidos

* Conecte o kit e verifique se o LED responde.
* Pressione os botões e confirme a troca de modos.

---

## 📸 Demonstração

* Vídeo curto do funcionamento: [https://www.youtube.com/shorts/4A8q-varqKg](https://www.youtube.com/shorts/4A8q-varqKg

---

## 🙌 Agradecimentos

* Programa **EmbarcaTech**
* Comunidade **BitDogLab**
* Documentação oficial do **Raspberry Pi Pico SDK**

---

## 📎 Referências úteis

* Pico SDK: [https://github.com/raspberrypi/pico-sdk](https://github.com/raspberrypi/pico-sdk)
* Documentação RP2040: [https://www.raspberrypi.com/documentation/microcontrollers/rp2040.html](https://www.raspberrypi.com/documentation/microcontrollers/rp2040.html)
* BitDogLab (materiais da comunidade): [https://github.com/BitDogLab](https://github.com/BitDogLab)

---

> **Dica:** mantenha este README vivo! Sempre que você alterar pinos, adicionar módulos ou mudar o comportamento, atualize as seções correspondentes.


Funcionamento:
https://www.youtube.com/shorts/4A8q-varqKg
