#ifndef __KERNEL_DMA_H
#define __KERNEL_DMA_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_PNP.H"
#define DMA_EN 0 //0:memory to per 1:per to memory
extern void DMA_INIT(void);
