/*
 *  ALSA lib - local header file
 *  Copyright (c) 2000 by Abramo Bagnara <abramo@alsa-project.org>
 *
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details.
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef __LOCAL_H
#define __LOCAL_H

#include "config.h"

#define _snd_config_iterator list_head
#define _snd_interval sndrv_interval
#define _snd_pcm_info sndrv_pcm_info
#define _snd_pcm_hw_params sndrv_pcm_hw_params
#define _snd_pcm_sw_params sndrv_pcm_sw_params
#define _snd_pcm_status sndrv_pcm_status

#define _snd_ctl_card_info sndrv_ctl_card_info
#define _snd_ctl_elem_id sndrv_ctl_elem_id
#define _snd_ctl_elem_list sndrv_ctl_elem_list
#define _snd_ctl_elem_info sndrv_ctl_elem_info
#define _snd_ctl_elem_value sndrv_ctl_elem_value
#define _snd_ctl_event sndrv_ctl_event

#define _snd_rawmidi_info sndrv_rawmidi_info
#define _snd_rawmidi_params sndrv_rawmidi_params
#define _snd_rawmidi_status sndrv_rawmidi_status

#define _snd_hwdep_info sndrv_hwdep_info

#define _snd_seq_queue_tempo sndrv_seq_queue_tempo
#define _snd_seq_client_info sndrv_seq_client_info
#define _snd_seq_port_info sndrv_seq_port_info
#define _snd_seq_system_info sndrv_seq_system_info
#define _snd_seq_queue_info sndrv_seq_queue_info
#define _snd_seq_queue_status sndrv_seq_queue_status
#define _snd_seq_queue_timer sndrv_seq_queue_timer
#define _snd_seq_port_subscribe sndrv_seq_port_subscribe
#define _snd_seq_query_subscribe sndrv_seq_query_subs
#define _snd_seq_client_pool sndrv_seq_client_pool
#define _snd_seq_remove_events sndrv_seq_remove_events
#define _snd_instr_header sndrv_seq_instr_header

#if 0
typedef struct sndrv_seq_addr snd_seq_addr_t;
#define snd_seq_tick_time_t sndrv_seq_tick_time_t
typedef struct sndrv_seq_real_time snd_seq_real_time_t;
typedef union sndrv_seq_timestamp snd_seq_timestamp_t;
typedef struct sndrv_seq_instr snd_seq_instr_t;
typedef struct sndrv_seq_event snd_seq_event_t;
#define snd_seq_instr_cluster_t sndrv_seq_instr_cluster_t
#endif

#define _snd_timer_id sndrv_timer_id
#define _snd_timer_select sndrv_timer_select
#define _snd_timer_info sndrv_timer_info
#define _snd_timer_params sndrv_timer_params
#define _snd_timer_status sndrv_timer_status

#include <sound/asound.h>
#include <sound/asequencer.h>
#include "asoundlib.h"
#include "list.h"

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define SND_LITTLE_ENDIAN
#endif
#if __BYTE_ORDER == __BIG_ENDIAN
#define SND_BIG_ENDIAN
#endif

extern int snd_async_signo;

struct _snd_async_handler {
	enum {
		SND_ASYNC_HANDLER_GENERIC,
		SND_ASYNC_HANDLER_PCM,
		SND_ASYNC_HANDLER_CTL,
	} type;
	int fd;
	union {
		snd_pcm_t *pcm;
		snd_ctl_t *ctl;
	} u;
	snd_async_callback_t callback;
	void *private_data;
	struct list_head glist;
	struct list_head hlist;
};

typedef enum _snd_set_mode {
	SND_CHANGE,
	SND_TRY,
	SND_TEST,
} snd_set_mode_t;

size_t page_align(size_t size);
size_t page_size(void);

int safe_strtol(const char *str, long *val);

#define HAVE_GNU_LD
#define HAVE_ELF
#define HAVE_ASM_PREVIOUS_DIRECTIVE

/* Stolen from libc-symbols.h in GNU glibc */

/* When a reference to SYMBOL is encountered, the linker will emit a
   warning message MSG.  */
#ifdef HAVE_GNU_LD
# ifdef HAVE_ELF

/* We want the .gnu.warning.SYMBOL section to be unallocated.  */
#  ifdef HAVE_ASM_PREVIOUS_DIRECTIVE
#   define __make_section_unallocated(section_string)	\
  asm (".section " section_string "\n\t.previous");
#  elif defined HAVE_ASM_POPSECTION_DIRECTIVE
#   define __make_section_unallocated(section_string)	\
  asm (".pushsection " section_string "\n\t.popsection");
#  else
#   define __make_section_unallocated(section_string)
#  endif

/* Tacking on "\n\t#" to the section name makes gcc put it's bogus
   section attributes on what looks like a comment to the assembler.  */
#  ifdef HAVE_SECTION_QUOTES
#   define link_warning(symbol, msg) \
  __make_section_unallocated (".gnu.warning." #symbol) \
  static const char __evoke_link_warning_##symbol[]	\
    __attribute__ ((section (".gnu.warning." #symbol "\"\n\t#\""))) = msg;
#  else
#   define link_warning(symbol, msg) \
  __make_section_unallocated (".gnu.warning." #symbol) \
  static const char __evoke_link_warning_##symbol[]	\
    __attribute__ ((section (".gnu.warning." #symbol "\n\t#"))) = msg;
#  endif
# else
#  define link_warning(symbol, msg)		\
  asm (".stabs \"" msg "\",30,0,0,0\n\t"	\
       ".stabs \"" __SYMBOL_PREFIX #symbol "\",1,0,0,0\n");
# endif
#else
/* We will never be heard; they will all die horribly.  */
# define link_warning(symbol, msg)
#endif

#endif
