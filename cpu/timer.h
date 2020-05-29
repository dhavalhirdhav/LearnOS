#define TIMER_CONTROL_PORT  0x43
#define TIMER_DATA_PORT 0x40

void initTimer(uint16_t count);
__attribute__ ((interrupt)) void timer_handler(struct interrupt_frame *frame);
void sleep(int milliseconds);
