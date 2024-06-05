// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/hugetlb.h>
#include <linux/mman.h>
#include <linux/mmzone.h>
#include <linux/proc_fs.h>
#include <linux/percpu.h>
#include <linux/seq_file.h>
#include <linux/swap.h>
#include <linux/vmstat.h>
#include <linux/atomic.h>
#include <linux/vmalloc.h>
#ifdef CONFIG_CMA
#include <linux/cma.h>
#endif
#include <asm/page.h>
#include "internal.h"
#include <trace/hooks/mm.h>

#define MEM_PAGESIZE_1G 262144
#define MEM_PAGESIZE_2G 524288
#define MEM_PAGESIZE_3G 786432
#define MEM_PAGESIZE_4G 1048576
#define MEM_PAGESIZE_6G 1572864
#define MEM_PAGESIZE_8G 2097152
#define MEM_PAGESIZE_12G 3145728 

static void show_val_kb(struct seq_file *m, const char *s, unsigned long num)
{
	unsigned long size = 0;

	if (num > 0 && num < MEM_PAGESIZE_1G) {
		size = 1;
	} else if (num > MEM_PAGESIZE_1G && num < MEM_PAGESIZE_2G) {
		size = 2;
	} else if (num > MEM_PAGESIZE_2G && num < MEM_PAGESIZE_3G) {
		size = 3;
	}else if (num > MEM_PAGESIZE_3G && num < MEM_PAGESIZE_4G){
		size = 4;
	}else if (num > MEM_PAGESIZE_4G && num < MEM_PAGESIZE_6G){
		size = 6;
	}else if (num > MEM_PAGESIZE_6G && num < MEM_PAGESIZE_8G){
                size = 8;
        }else if (num > MEM_PAGESIZE_8G && num < MEM_PAGESIZE_12G){
                size = 12;
        }

	seq_put_decimal_ull_width(m, s, size, 1);
	seq_write(m, "GB\n", 3);
}

static int memcap_proc_show(struct seq_file *m, void *v)
{
	struct sysinfo i;

	si_meminfo(&i);
	si_swapinfo(&i);

	show_val_kb(m, "", i.totalram);

	return 0;
}

static int __init proc_memcap_init(void)
{
	proc_create_single("mem_capacity", 0, NULL, memcap_proc_show);
	return 0;
}
fs_initcall(proc_memcap_init);
