/*
 * Copyright 2003-2016 The Music Player Daemon Project
 * http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "config.h"
#include "mixer/MixerControl.hxx"
#include "mixer/MixerList.hxx"
#include "filter/FilterRegistry.hxx"
#include "pcm/Volume.hxx"
#include "Main.hxx"
#include "event/Loop.hxx"
#include "config/Block.hxx"
#include "util/Error.hxx"
#include "Log.hxx"

#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

const struct filter_plugin *
filter_plugin_by_name(gcc_unused const char *name)
{
	assert(false);
	return NULL;
}

int main(int argc, gcc_unused char **argv)
try {
	int volume;

	if (argc != 2) {
		fprintf(stderr, "Usage: read_mixer PLUGIN\n");
		return EXIT_FAILURE;
	}

	EventLoop event_loop;

	Mixer *mixer = mixer_new(event_loop, alsa_mixer_plugin,
				 *(AudioOutput *)nullptr,
				 *(MixerListener *)nullptr,
				 ConfigBlock());

	mixer_open(mixer);

	volume = mixer_get_volume(mixer);
	mixer_close(mixer);
	mixer_free(mixer);

	assert(volume >= -1 && volume <= 100);

	if (volume < 0) {
		fprintf(stderr, "failed to read volume\n");
		return EXIT_FAILURE;
	}

	printf("%d\n", volume);
	return 0;
} catch (const std::exception &e) {
	LogError(e);
	return EXIT_FAILURE;
}
