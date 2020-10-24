#include <string.h>
#include <chip8/cpu.h>

#define TIMER_UPDATE_SPEED 1000 / 60.0
#define IPS_MULT		   9

void chip8_cpu_init(struct chip8_cpu* restrict cpu) {
	cpu->speed.timer_speed = TIMER_UPDATE_SPEED;
	cpu->speed.ips_mult	   = IPS_MULT;

	chip8_cpu_reset(cpu);
}

void chip8_cpu_reset(struct chip8_cpu* restrict cpu) {
	/* Set everything besides speed to 0 */
	memset(cpu + sizeof cpu->speed, 0, sizeof(struct chip8_cpu) - sizeof cpu->speed);
}
