/*
 * Common sbus abstraction layer interface for apollo wireless driver
 * *
 * Copyright (c) 2016, altobeam
 * Author:
 *
 * Based on apollo code
 * Copyright (c) 2010, ST-Ericsson
 * Author: Dmitry Tarnyagin <dmitry.tarnyagin@stericsson.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef ATBM_APOLLO_SBUS_H
#define ATBM_APOLLO_SBUS_H

/*
 * sbus priv forward definition.
 * Implemented and instantiated in particular modules.
 */
struct sbus_priv;

typedef void (*sbus_irq_handler)(void *priv);
typedef void (*sbus_callback_handler)(void *priv,void * arg);


struct sbus_ops {
#ifndef SPI_BUS
	int (*sbus_memcpy_fromio)(struct sbus_priv *self, unsigned int addr,void *dst, int count);/*rx queue mode*/
	int (*sbus_memcpy_toio)(struct sbus_priv *self, unsigned int addr,const void *src, int count);/*tx queue mode*/
	int (*sbus_read_async)(struct sbus_priv *self, unsigned int addr,void *dst, int count,sbus_callback_handler hander);/*rx queue mode*/
	int (*sbus_write_async)(struct sbus_priv *self, unsigned int addr,const void *src, int count,sbus_callback_handler hander);/*tx queue mode*/
	int (*sbus_read_sync)(struct sbus_priv *self, unsigned int addr, void *dst, int len);/*read register,download firmware,len <=256*/
	int (*sbus_write_sync)(struct sbus_priv *self, unsigned int addr, const void *src, int len);/*write register,download firmware,len <=256*/
	void (*lock)(struct sbus_priv *self);
	void (*unlock)(struct sbus_priv *self);	
	int (*irq_subscribe)(struct sbus_priv *self, sbus_irq_handler handler,
				void *priv);
	int (*irq_unsubscribe)(struct sbus_priv *self);
	int (*reset)(struct sbus_priv *self);
	u32 (*align_size)(struct sbus_priv *self, u32 size);
	int (*power_mgmt)(struct sbus_priv *self, bool suspend);
	int (*set_block_size)(struct sbus_priv *self, u32 size);
	void (*wtd_wakeup)( struct sbus_priv *self);
	#ifdef ATBM_USB_RESET
	int (*usb_reset)(struct sbus_priv *self);
	#endif
	int (*bootloader_debug_config)(struct sbus_priv *self,u16 enable);	
	int (*lmac_start)(struct sbus_priv *self);
	int (*ep0_cmd)(struct sbus_priv *self);
	int (*sbus_reset_chip)(struct sbus_priv *self);
	int (*abort)(struct sbus_priv *self);
#else
	u32 (*align_size)(struct sbus_priv *self, u32 size);
	int (*set_block_size)(struct sbus_priv *self, u32 size);
	int (*irq_subscribe)(struct sbus_priv *self, sbus_irq_handler handler,
				void *priv);
	int (*irq_unsubscribe)(struct sbus_priv *self);
	int (*reset)(struct sbus_priv *self);
	void (*lock)(struct sbus_priv *self);
	void (*unlock)(struct sbus_priv *self);
	int (*power_mgmt)(struct sbus_priv *self, bool suspend);

	int (*sbus_read_data)(struct sbus_priv *self, void *rx, size_t rx_len);
	int (*sbus_write_data)(struct sbus_priv *self,const void *tx, size_t tx_len);
	int (*sbus_write_firmware)(struct sbus_priv *self,unsigned int addr, const void *src, int count);
	int (*sbus_read_status)(struct sbus_priv *self, u32 *status, size_t tx_len);
	int (*sbus_read_channelflag)(struct sbus_priv *self, u32 *channelflag);
	int (*sbus_update_channelflag)(struct sbus_priv *self);
	int (*sbus_read_ready)(struct sbus_priv *self, u32 *ready);
	int (*sbus_reset_cpu)(struct sbus_priv *self);
	int (*sbus_shutdown_wlan)(struct sbus_priv *self);
	int (*sbus_reset_chip)(struct sbus_priv *self);
	int (*bootloader_debug_config)(struct sbus_priv *self,u16 enable);	
#endif
};

#endif /* ATBM_APOLLO_SBUS_H */
