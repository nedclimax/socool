#define SOKOL_IMPL

#if defined(_WIN32)
	#define SOKOL_D3D11
#elif defined(__APPLE__)
	#define SOKOL_METAL
#else
	#define SOKOL_GLCORE33
#endif

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_log.h"
#include "sokol_audio.h"
#include "sokol_time.h"
#include "sokol_glue.h"

#include "linmath.h"

#include "shaders/bin/default.glsl.h"

sg_pipeline pipeline;
sg_bindings bindings;
sg_pass_action pass_action;

void init(void) {
	sg_setup(&(sg_desc) {
		.context = sapp_sgcontext(),
		.logger = slog_func
	});

	float vertices[] = {
		-1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 1.0,
		 1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 1.0,
		 1.0,  1.0, -1.0, 1.0, 0.0, 0.0, 1.0,
		-1.0,  1.0, -1.0, 1.0, 0.0, 0.0, 1.0,

		-1.0, -1.0,  1.0, 0.0, 1.0, 0.0, 1.0,
		 1.0, -1.0,  1.0, 0.0, 1.0, 0.0, 1.0,
		 1.0,  1.0,  1.0, 0.0, 1.0, 0.0, 1.0,
		-1.0,  1.0,  1.0, 0.0, 1.0, 0.0, 1.0,

		-1.0, -1.0, -1.0, 0.0, 0.0, 1.0, 1.0,
		-1.0,  1.0, -1.0, 0.0, 0.0, 1.0, 1.0,
		-1.0,  1.0,  1.0, 0.0, 0.0, 1.0, 1.0,
		-1.0, -1.0,  1.0, 0.0, 0.0, 1.0, 1.0,

		 1.0, -1.0, -1.0, 1.0, 0.5, 0.0, 1.0,
		 1.0,  1.0, -1.0, 1.0, 0.5, 0.0, 1.0,
		 1.0,  1.0,  1.0, 1.0, 0.5, 0.0, 1.0,
		 1.0, -1.0,  1.0, 1.0, 0.5, 0.0, 1.0,

		-1.0, -1.0, -1.0, 0.0, 0.5, 1.0, 1.0,
		-1.0, -1.0,  1.0, 0.0, 0.5, 1.0, 1.0,
		 1.0, -1.0,  1.0, 0.0, 0.5, 1.0, 1.0,
		 1.0, -1.0, -1.0, 0.0, 0.5, 1.0, 1.0,

		-1.0, 1.0,  -1.0, 1.0, 0.0, 0.5, 1.0,
		-1.0, 1.0,   1.0, 1.0, 0.0, 0.5, 1.0,
		 1.0, 1.0,   1.0, 1.0, 0.0, 0.5, 1.0,
		 1.0, 1.0, - 1.0, 1.0, 0.0, 0.5, 1.0
	};

	sg_buffer vertex_buffer = sg_make_buffer(&(sg_buffer_desc) {
		.type = SG_BUFFERTYPE_VERTEXBUFFER,
		.data = SG_RANGE(vertices),
		.usage = SG_USAGE_IMMUTABLE,
		.label = "cube_vertex_buffer"
	});

	uint16_t indices[] = {
		0, 1, 2, 0, 2, 3,
		6, 5, 4, 7, 6, 4,
		8, 9, 10, 8, 10, 11,
		14, 13, 12, 15, 14, 12,
		16, 17, 18, 16, 18, 19,
		22, 21, 20, 23, 22, 20
	};

	sg_buffer index_buffer = sg_make_buffer(&(sg_buffer_desc) {
		.type = SG_BUFFERTYPE_INDEXBUFFER,
		.data = SG_RANGE(indices),
		.label = "cube_index_buffer"
	});

	sg_shader shader = sg_make_shader(cube_shader_desc(sg_query_backend()));

	pipeline = sg_make_pipeline(&(sg_pipeline_desc) {
		.layout = {
			.buffers[0].stride = 28,
			.attrs = {
				[ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3,
				[ATTR_vs_color0].format = SG_VERTEXFORMAT_FLOAT4
			}
		},
		.shader = shader,
		.index_type = SG_INDEXTYPE_UINT16,
		.cull_mode = SG_CULLMODE_BACK,
		.depth = {
			.write_enabled = true,
			.compare = SG_COMPAREFUNC_LESS_EQUAL
		},
		.label = "cube_pipeline"
	});

	pass_action = (sg_pass_action) {
		.colors[0] = { .load_action=SG_LOADACTION_CLEAR, .clear_value={1.0f, 0.0f, 0.0f, 1.0f} }
	};

	bindings = (sg_bindings) {
		.vertex_buffers[0] = vertex_buffer,
		.index_buffer = index_buffer
	};
}

void update(void) {
	const float width = sapp_widthf();
	const float height = sapp_heightf();
	const float frame_time = (float)(sapp_frame_duration() * 60.0);

	sg_begin_default_pass(&pass_action, (int)width, (int)height);
	sg_end_pass();
	sg_commit();
}

void on_event(const sapp_event* e) {
}

void cleanup(void) {
	sg_shutdown();
}

sapp_desc sokol_main(int argc, char** argv) {
	return (sapp_desc) {
		.init_cb = init,
		.frame_cb = update,
		.event_cb = on_event,
		.cleanup_cb = cleanup,
		.width = 800,
		.height = 600,
		.window_title = "sktest",
		.logger = slog_func
	};
}