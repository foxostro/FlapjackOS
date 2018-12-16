void hlt(void)
{
    // do nothing
}

__attribute__((noreturn))
void halt_forever(void)
{
    while(1) hlt();
}
