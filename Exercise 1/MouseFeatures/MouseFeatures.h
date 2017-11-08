#ifndef _MOUSEFEATURES_H
#define _MOUSEFEATURES_H

#include "base/IFeature.h"
#include "ioput/option/OptionList.h"
#include <math.h>

using namespace ssi;

class MouseFeatures : public IFeature {

public:

	class Options : public OptionList {

	public:

		Options() {
			frame_min_x = true;
			frame_max_x = true;
			frame_average_x = true;

			frame_min_y = true;
			frame_max_y = true;
			frame_average_y = true;

			global_min_x = true;
			global_max_x = true;
			global_average_x = true;

			global_min_y = true;
			global_max_y = true;
			global_average_y = true;

			distance = true;
			velocity = true;
			variability = true;
			standard_deviation = true;

			addOption("frame_min_x", &frame_min_x, 1, SSI_BOOL, "compute frame_min_x");
			addOption("frame_max_x", &frame_max_x, 1, SSI_BOOL, "compute frame_max_x");
			addOption("frame_avaerage_x", &frame_average_x, 1, SSI_BOOL, "compute frame_average_x");

			addOption("frame_min_y", &frame_min_y, 1, SSI_BOOL, "compute frame_min_y");
			addOption("frame_max_y", &frame_max_y, 1, SSI_BOOL, "compute frame_max_y");
			addOption("frame_avaerage_y", &frame_average_y, 1, SSI_BOOL, "compute frame_average_y");

			addOption("global_min_x", &global_min_x, 1, SSI_BOOL, "compute global_min_x");
			addOption("global_max_x", &global_max_x, 1, SSI_BOOL, "compute global_max_x");
			addOption("global_average_x", &global_average_x, 1, SSI_BOOL, "compute global_average_x");

			addOption("global_min_y", &global_min_y, 1, SSI_BOOL, "compute global_min_y");
			addOption("global_max_y", &global_max_y, 1, SSI_BOOL, "compute global_max_y");
			addOption("global_average_y", &global_average_y, 1, SSI_BOOL, "compute global_average_y");

			addOption("distance", &distance, 1, SSI_BOOL, "compute distance");
			addOption("velocity", &velocity, 1, SSI_BOOL, "compute velocity");
			addOption("variability", &variability, 1, SSI_BOOL, "compute variability");
			addOption("standard_deviation", &standard_deviation, 1, SSI_BOOL, "compute standard_deviation");
		};

		bool frame_min_x;
		bool frame_max_x;
		bool frame_average_x;

		bool frame_min_y;
		bool frame_max_y;
		bool frame_average_y;

		bool global_min_x;
		bool global_max_x;
		bool global_average_x;

		bool global_min_y;
		bool global_max_y;
		bool global_average_y;

		bool distance;
		bool velocity;
		bool variability;
		bool standard_deviation;
	};

public:
	static const ssi_char_t *GetCreateName() { return "MouseFeatures"; };
	static IObject *Create(const ssi_char_t *file) { return new MouseFeatures(file); };
	~MouseFeatures();
	const ssi_char_t *getName() { return GetCreateName(); };
	const ssi_char_t *getInfo() { return "MouseFeatures Solution Info"; }; 

	MouseFeatures::Options *getOptions() { return &_options; }; 

	void transform_enter(ssi_stream_t &stream_in, ssi_stream_t &stream_out, ssi_size_t xtra_stream_in_num, ssi_stream_t xtra_stream_in[]);
	void transform(ITransformer::info info, ssi_stream_t &stream_in, ssi_stream_t &stream_out, ssi_size_t xtra_stream_in_num, ssi_stream_t xtra_stream_in[]);
	void transform_flush(ssi_stream_t &stream_in, ssi_stream_t &stream_out, ssi_size_t xtra_stream_in_num, ssi_stream_t xtra_stream_in[]);

	// TODO
	ssi_size_t getSampleDimensionOut(ssi_size_t sample_dimension_in) {
		sample_dimension_in = 0;
		if (_options.frame_min_x) {
			sample_dimension_in++;
		}
		if (_options.frame_max_x) {
			sample_dimension_in++;
		}
		if (_options.frame_average_x) {
			sample_dimension_in++;
		}
		if (_options.frame_min_y) {
			sample_dimension_in++;
		}
		if (_options.frame_max_y) {
			sample_dimension_in++;
		}
		if (_options.frame_average_y) {
			sample_dimension_in++;
		}
		if (_options.global_min_x) {
			sample_dimension_in++;
		}
		if (_options.global_max_x) {
			sample_dimension_in++;
		}
		if (_options.global_average_x) {
			sample_dimension_in++;
		}
		if (_options.global_min_y) {
			sample_dimension_in++;
		}
		if (_options.global_max_y) {
			sample_dimension_in++;
		}
		if (_options.global_average_y) {
			sample_dimension_in++;
		}
		if (_options.distance) {
			sample_dimension_in++;
		}
		if (_options.velocity) {
			sample_dimension_in++;
		}
		if (_options.variability) {
			sample_dimension_in += 2;
		}
		if (_options.standard_deviation) {
			sample_dimension_in += 2;
		}
		return sample_dimension_in; //what sample dimesnion does your component return? --> hängt von ausgewählten Optionen ab
	};
	ssi_size_t getSampleBytesOut(ssi_size_t sample_bytes_in) {
		return sample_bytes_in; //what sample bytes does your component return?
	}
	ssi_type_t getSampleTypeOut(ssi_type_t sample_type_in) {
		return sample_type_in; //what sample type does your component return?
	}

protected:

	MouseFeatures(const ssi_char_t *file = 0);
	MouseFeatures::Options _options;
	ssi_char_t * _file;
	
};

#endif
