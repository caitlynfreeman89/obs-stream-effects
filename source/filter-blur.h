/*
 * Modern effects for a modern Streamer
 * Copyright (C) 2017 Michael Fabian Dirks
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#pragma once
#include "plugin.h"
#include "gs-helper.h"
#include "gs-effect.h"
#include "gs-texture.h"
#include <memory>

#define S_FILTER_BLUR					"Filter.Blur"
#define S_FILTER_BLUR_TYPE				"Filter.Blur.Type"
#define S_FILTER_BLUR_TYPE_BOX				"Filter.Blur.Type.Box"
#define S_FILTER_BLUR_TYPE_GAUSSIAN			"Filter.Blur.Type.Gaussian"
#define S_FILTER_BLUR_TYPE_BILATERAL			"Filter.Blur.Type.Bilateral"
#define S_FILTER_BLUR_SIZE				"Filter.Blur.Size"

// Bilateral Blur
#define S_FILTER_BLUR_BILATERAL_SMOOTHING		"Filter.Blur.Bilateral.Smoothing"
#define S_FILTER_BLUR_BILATERAL_SHARPNESS		"Filter.Blur.Bilateral.Sharpness"

// Advanced
#define S_FILTER_BLUR_COLORFORMAT			"Filter.Blur.ColorFormat"

namespace Filter {
	class Blur {
		public:
		Blur();
		~Blur();

		static const char *get_name(void *);
		static void get_defaults(obs_data_t *);
		static obs_properties_t *get_properties(void *);
		static bool modified_properties(obs_properties_t *,
			obs_property_t *, obs_data_t *);

		static void *create(obs_data_t *, obs_source_t *);
		static void destroy(void *);
		static uint32_t get_width(void *);
		static uint32_t get_height(void *);
		static void update(void *, obs_data_t *);
		static void activate(void *);
		static void deactivate(void *);
		static void video_tick(void *, float);
		static void video_render(void *, gs_effect_t *);

		enum Type : int64_t {
			Box,
			Gaussian,
			Bilateral,
		};

		private:
		obs_source_info m_sourceInfo;

		private:
		class Instance {
			public:
			Instance(obs_data_t*, obs_source_t*);
			~Instance();

			void update(obs_data_t*);
			uint32_t get_width();
			uint32_t get_height();
			void activate();
			void deactivate();
			void show();
			void hide();
			void video_tick(float);
			void video_render(gs_effect_t*);
			bool apply_shared_param(gs_texture_t* input,
				float texelX, float texelY);
			bool apply_bilateral_param();
			bool apply_gaussian_param();

			private:
			obs_source_t *m_source;
			gs_texrender_t *m_primaryRT, *m_secondaryRT;
			gs_texrender_t *m_rtHorizontal, *m_rtVertical;
			std::shared_ptr<gs::effect> m_effect;

			// Blur
			Type m_type;
			uint64_t m_size;

			// Bilateral
			double_t m_bilateralSmoothing;
			double_t m_bilateralSharpness;

			// Advanced
			bool m_errorLogged = false;
			uint64_t m_colorFormat;
		};
	};
}
