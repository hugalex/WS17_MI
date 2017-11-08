#include "MouseFeatures.h"


MouseFeatures::MouseFeatures(const ssi_char_t *file) : _file (0)
{
	if (file) {
		if (!OptionList::LoadXML(file, &_options)) {
			OptionList::SaveXML(file, &_options);
		}
		_file = ssi_strcpy(file);
	}
}

MouseFeatures::~MouseFeatures()
{
	if (_file) {
		OptionList::SaveXML(_file, &_options);
		delete[] _file;
	}
}

void MouseFeatures::transform_enter(ssi_stream_t &stream_in, ssi_stream_t &stream_out, ssi_size_t xtra_stream_in_num, ssi_stream_t xtra_stream_in[])
{
	//todo
}

void MouseFeatures::transform(ITransformer::info info, ssi_stream_t &stream_in, ssi_stream_t &stream_out, ssi_size_t xtra_stream_in_num, ssi_stream_t xtra_stream_in[])
{
	// test for valid input
	if (stream_in.dim != 2)
		ssi_wrn("unexpected stream dimension");

	// get input
	float *in = ssi_pcast(float, stream_in.ptr);

	// prepare output	
	float *out = ssi_pcast(float, stream_out.ptr);

	// variables
	float frame_min_x = 1000;
	float frame_max_x = 0;
	float frame_average_x = 0;

	float frame_min_y = 1000;
	float frame_max_y = 0;
	float frame_average_y = 0;

	static float global_min_x = 0;
	static float global_max_x = 0;
	static float global_average_x = 0;
	static float global_average_x_counter = 0;

	static float global_min_y = 0;
	static float global_max_y = 0;
	static float global_average_y = 0;
	static float global_average_y_counter = 0;

	float distance = 0;
	float velocity = 0;
	float variability = 0;
	float standard_deviation = 0;

	// populate output stream
	int out_index = 0;

	if (_options.frame_min_x) {
		for (int j = 0; j < stream_in.num; j++) {
			if (frame_min_x > in[j*stream_in.dim]) {
				frame_min_x = in[j*stream_in.dim];
			}
		}
		out[out_index++] = frame_min_x;
	}	
			
	if (_options.frame_max_x) {
		for (int j = 0; j < stream_in.num; j++) {
			if (frame_max_x < in[j*stream_in.dim]) {
				frame_max_x = in[j*stream_in.dim];
			}
		}
		out[out_index++] = frame_max_x;
	}

	if (_options.frame_average_x) {
		for (int j = 0; j < stream_in.num; j++) {
			frame_average_x += in[j*stream_in.dim];
		}
		out[out_index++] = frame_average_x /stream_in.num;
	}

	if (_options.frame_min_y) {
		for (int j = 0; j < stream_in.num; j++) {
			if (frame_min_y > in[j*stream_in.dim + 1]) {
				frame_min_y = in[j*stream_in.dim + 1];
			}
		}
		out[out_index++] = frame_min_y;
	}

	if (_options.frame_max_y) {
		for (int j = 0; j < stream_in.num; j++) {
			if (frame_max_y < in[j*stream_in.dim + 1]) {
				frame_max_y = in[j*stream_in.dim + 1];
			}
		}
		out[out_index++] = frame_max_y;
	}

	if (_options.frame_average_y) {
		for (int j = 0; j < stream_in.num; j++) {
			frame_average_y += in[j*stream_in.dim + 1];
		}
		out[out_index++] = frame_average_y / stream_in.num;
	}

	if (_options.global_min_x) {
		for (int j = 0; j < stream_in.num; j++) {
			if (global_min_x > in[j*stream_in.dim]) {
				global_min_x = in[j*stream_in.dim];
			}
		}
		out[out_index++] = global_min_x;
	}

	if (_options.global_max_x) {
		for (int j = 0; j < stream_in.num; j++) {
			if (global_max_x < in[j*stream_in.dim]) {
				global_max_x = in[j*stream_in.dim];
			}
		}
		out[out_index++] = global_max_x;
	}

	if (_options.global_average_x) {
		for (int j = 0; j < stream_in.num; j++) {
			global_average_x += in[j*stream_in.dim];
			global_average_x_counter++;
		}
		out[out_index++] = global_average_x / global_average_x_counter;
	}

	if (_options.global_min_y) {
		for (int j = 0; j < stream_in.num; j++) {
			if (global_min_y > in[j*stream_in.dim + 1]) {
				global_min_y = in[j*stream_in.dim + 1];
			}
		}
		out[out_index++] = global_min_y;
	}

	if (_options.global_max_y) {
		for (int j = 0; j < stream_in.num; j++) {
			if (global_max_y < in[j*stream_in.dim + 1]) {
				global_max_y = in[j*stream_in.dim + 1];
			}
		}
		out[out_index++] = global_max_y;
	}

	if (_options.global_average_y) {
		for (int j = 0; j < stream_in.num; j++) {
			global_average_y += in[j*stream_in.dim + 1];
			global_average_y_counter++;
		}
		out[out_index++] = global_average_y / global_average_y_counter;
	}

	if (_options.distance) {
		float buf_value_x = in[0];
		float buf_value_y = in[1];

		for (int j = 1; j < stream_in.num; j++) {
			distance += sqrt(pow(buf_value_x - in[j*stream_in.dim], 2) + pow(buf_value_y - in[j*stream_in.dim + 1], 2));
			buf_value_x = in[j*stream_in.dim];
			buf_value_y = in[j*stream_in.dim + 1];
		}
		out[out_index++] = distance;
	}

	if (_options.velocity) {
		float buf_value_x = in[0];
		float buf_value_y = in[1];
		float velocity_distance = 0;
		float velocity_time = 1 / stream_in.sr * stream_in.num;

		for (int j = 1; j < stream_in.num; j++) {
			velocity_distance += sqrt(pow(buf_value_x - in[j*stream_in.dim], 2) + pow(buf_value_y - in[j*stream_in.dim + 1], 2));
			buf_value_x = in[j*stream_in.dim];
			buf_value_y = in[j*stream_in.dim + 1];
		}

		velocity = velocity_distance / velocity_time;

		out[out_index++] = velocity;
	}

	if (_options.variability) {
		float variability_x = 0;
		float variability_y = 0;
		float average_x = 0;
		float average_y = 0;

		for (int j = 0; j < stream_in.num; j++) {
			average_x += in[j*stream_in.dim];
			average_y += in[j*stream_in.dim + 1];
			variability_x += pow(in[j*stream_in.dim], 2) / stream_in.num;
			variability_y += pow(in[j*stream_in.dim + 1], 2) / stream_in.num;
		}
		average_x /= stream_in.num;
		average_y /= stream_in.num;

		variability_x += pow(average_x, 2);
		variability_y += pow(average_y, 2);

		out[out_index++] = variability_x;
		out[out_index++] = variability_y;
	}

	if (_options.standard_deviation) {
		float variability_x = 0;
		float variability_y = 0;
		float average_x = 0;
		float average_y = 0;
		float standard_deviation_x = 0;
		float standard_deviation_y = 0;

		for (int j = 0; j < stream_in.num; j++) {
			average_x += in[j*stream_in.dim];
			average_y += in[j*stream_in.dim + 1];
			variability_x += pow(in[j*stream_in.dim], 2) / stream_in.num;
			variability_y += pow(in[j*stream_in.dim + 1], 2) / stream_in.num;
		}
		average_x /= stream_in.num;
		average_y /= stream_in.num;

		variability_x += pow(average_x, 2);
		variability_y += pow(average_y, 2);

		standard_deviation_x = sqrt(variability_x);
		standard_deviation_y = sqrt(variability_y);

		out[out_index++] = standard_deviation_x;
		out[out_index++] = standard_deviation_y;
	}
}

void MouseFeatures::transform_flush(ssi_stream_t &stream_in, ssi_stream_t &stream_out, ssi_size_t xtra_stream_in_num, ssi_stream_t xtra_stream_in[])
{
	//todo
}