#include <linux/types.h>
#include <asm/addrspace.h>
#include <asm/io.h>

#define AR71XX_APB_BASE		0x18000000
#define AR933X_UART_BASE	(AR71XX_APB_BASE + 0x00020000)

#define DEBUG_LL_UART_ADDR	KSEG1ADDR(AR933X_UART_BASE)

#define AR933X_UART_DATA_REG		0x00
#define AR933X_UART_DATA_TX_RX_MASK	0xff
#define AR933X_UART_DATA_TX_CSR		0x200
#define AR933X_UART_DATA_RX_CSR		0x100

static inline void ar933x_debug_ll_writel(u32 b, int offset)
{
	__raw_writel(b, (u8 *)DEBUG_LL_UART_ADDR + offset);
}

static inline u32 ar933x_debug_ll_readl(int offset)
{
	return __raw_readl((u8 *)DEBUG_LL_UART_ADDR + offset);
}

void putc(char c)
{
	u32 data;

	/* wait transmitter ready */
	data = ar933x_debug_ll_readl(AR933X_UART_DATA_REG);
	while (!(data & AR933X_UART_DATA_TX_CSR))
		data = ar933x_debug_ll_readl(AR933X_UART_DATA_REG);

	data = (c & AR933X_UART_DATA_TX_RX_MASK) | AR933X_UART_DATA_TX_CSR;
	ar933x_debug_ll_writel(data, AR933X_UART_DATA_REG);
}

