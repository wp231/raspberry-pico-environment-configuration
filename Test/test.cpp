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
