# Raspberry Pico 環境配置

<!-- vim-markdown-toc GFM -->

* [基礎配置](#基礎配置)
  - [pico-sdk](#pico-sdk)
  - [範例 pico-examples](#範例-pico-examples)
  - [安裝編譯工具](#安裝編譯工具)
* [Cmake 配置](#cmake-配置)
  - [手動配置 CMakeLists.txt](#手動配置-cmakeliststxt)
  - [自動生成 pico-project-generator](#自動生成-pico-project-generator)
* [測試執行](#測試執行)
  - [編譯並生成 .uf2](#編譯並生成-uf2)
  - [燒入 Raspberry Pico](#燒入-raspberry-pico)
* [ccls + coc.nvim 添加 include 路徑](#ccls--cocnvim-添加-include-路徑)

<!-- vim-markdown-toc -->

## 基礎配置

### [pico-sdk](https://github.com/raspberrypi/pico-sdk) 

- 複製專案

```shell
mkdir pico
cd pico
git clone -b master https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init
cd
```

- 添加環境變量

```shell
echo "export PICO_SDK_PATH=\"\$HOME/pico/pico-sdk\"" >> $HOME/.bashrc
```

- 更新

```shell
cd pico-sdk
git pull
git submodule update
```

### 範例 [pico-examples](https://github.com/raspberrypi/pico-examples) 

- 複製專案

```shell
git clone -b master https://github.com/raspberrypi/pico-examples.git
```

### 安裝編譯工具

```shell
apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential
```

## Cmake 配置

### 手動配置 [CMakeLists.txt](Test/CMakeLists.txt)

```cmake
# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.16)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(blink C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
    test.cpp
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
```

### 自動生成 [pico-project-generator](https://github.com/raspberrypi/pico-project-generator)

- 安裝額外 python 庫

```shell
apt install python3-tk
```

- 複製專案

```shell
git clone https://github.com/raspberrypi/pico-project-generator
```

- 使用

```shell
cd pico-project-generator
./pico_project.py -g
```

## 測試執行

- [test.cpp](Test/test.cpp)

```cpp
#include "pico/stdlib.h"
#include <stdio.h>

int main() {

  // USB 串口輸出
  stdio_init_all();
  printf("Hello, world!\n");

  const uint LED_PIN = 25;
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  while (true) {
    // LED 亮
    gpio_put(LED_PIN, 1);
    printf("LED Up\n");

    // 延遲 1000 毫秒
    sleep_ms(500);

    // LED 暗
    gpio_put(LED_PIN, 0);
    printf("LED Down\n");

    sleep_ms(500);
  }
}
```

### 編譯並生成 .uf2 

```shell
mkdir build
cd build
cmake ..
make
```

### 燒入 Raspberry Pico

按住 `BOOTSEL` 按鈕，插入 `USB` 後將 `.uf2` 放入 完成燒入

## [ccls](https://github.com/MaskRay/ccls) + [coc.nvim](https://github.com/neoclide/coc.nvim) 添加 include 路徑

- [.ccls](Test/.ccls) config

```txt
clang
%h -x c++-header

-I/home/<UserName>/pico/pico-sdk/src/host/hardware_divider/include
-I/home/<UserName>/pico/pico-sdk/src/host/hardware_gpio/include
-I/home/<UserName>/pico/pico-sdk/src/host/hardware_sync/include
-I/home/<UserName>/pico/pico-sdk/src/host/hardware_timer/include
-I/home/<UserName>/pico/pico-sdk/src/host/hardware_uart/include
-I/home/<UserName>/pico/pico-sdk/src/host/pico_multicore/include
-I/home/<UserName>/pico/pico-sdk/src/host/pico_platform/include
-I/home/<UserName>/pico/pico-sdk/src/host/pico_stdio/include

-I/home/<UserName>/pico/pico-sdk/src/common/oot_picoboot/include
-I/home/<UserName>/pico/pico-sdk/src/common/boot_uf2/include
-I/home/<UserName>/pico/pico-sdk/src/common/pico_base/include
-I/home/<UserName>/pico/pico-sdk/src/common/pico_binary_info/include
-I/home/<UserName>/pico/pico-sdk/src/common/pico_bit_ops/include
-I/home/<UserName>/pico/pico-sdk/src/common/pico_divider/include
-I/home/<UserName>/pico/pico-sdk/src/common/pico_stdlib/include
-I/home/<UserName>/pico/pico-sdk/src/common/pico_sync/include
-I/home/<UserName>/pico/pico-sdk/src/common/pico_time/include
-I/home/<UserName>/pico/pico-sdk/src/common/pico_usb_reset_interface/include
-I/home/<UserName>/pico/pico-sdk/src/common/pico_util/include

-I/home/<UserName>/pico/pico-sdk/src/boards/include

-I/home/<UserName>/pico/pico-sdk/src/rp2040/hardware_regs/include
-I/home/<UserName>/pico/pico-sdk/src/rp2040/hardware_structs/include

-I/home/<UserName>/pico/pico-sdk/src/rp2_common/boot_stage2/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/cmsis/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_adc/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_base/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_claim/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_clocks/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_divider/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_dma/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_exception/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_flash/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_gpio/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_i2c/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_interp/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_irq/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_pio/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_pll/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_pwm/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_resets/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_rtc/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_spi/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_sync/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_timer/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_uart/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_vreg/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_watchdog/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/hardware_xosc/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/pico_bootrom/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/pico_double/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/pico_float/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/pico_int64_ops/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/pico_malloc/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/pico_mem_ops/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/pico_multicore/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/pico_platform/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/pico_printf/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/pico_runtime/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/pico_stdio/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/pico_stdio_semihosting/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/pico_stdio_uart/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/pico_stdio_usb/include
-I/home/<UserName>/pico/pico-sdk/src/rp2_common/pico_unique_id/include
```

or

- [compile_commands.json](Test/build/compile_commands.json) 

```shell
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
ln -s ./build/compile_commands.json ./compile_commands.json
```

