// Main
// author: Amir Emad Gerges <amir.kolta.92@gmail.com>
// created: 2013/03/15
// Copyright (C) 2007-13 University of Augsburg, Lab for Human Centered Multimedia
//
// *************************************************************************************************
//
// This file is part of Social Signal Interpretation (SSI) developed at the 
// Lab for Human Centered Multimedia of the University of Augsburg
//
// This library is free software; you can redistribute itand/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or any laterversion.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FORA PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along withthis library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
//*************************************************************************************************
 
#include "ssi.h"
#include "ssidummy.h"
#include "mouse\include\ssimouse.h"

#include "model\include\ssimodel.h"
#include "ssiml\include\ssiml.h"
#include "signal\include\ssisignal.h"

using namespace ssi;

// load libraries
#ifdef _MSC_VER 
#ifdef _DEBUG
#pragma comment(lib, "ssid.lib")
#else
#pragma comment(lib, "ssi.lib")
#endif
#endif

#ifdef USE_SSI_LEAK_DETECTOR
	#include "SSI_LeakWatcher.h"
	#ifdef _DEBUG
		#define new DEBUG_NEW
		#undef THIS_FILE
		static char THIS_FILE[] = __FILE__;
	#endif
#endif

void ex_dummy ();
void challenge(const char* model, const char* data_dir);

int main () {

#ifdef USE_SSI_LEAK_DETECTOR
	{
#endif

	ssi_print ("%s\n\nbuild version: %s\n\n", SSI_COPYRIGHT, SSI_VERSION);

	Factory::RegisterDLL("ssiframe.dll");
	Factory::RegisterDLL("ssigraphic.dll");
	Factory::RegisterDLL("ssiioput.dll");
	Factory::RegisterDLL("ssimouse.dll");
	Factory::RegisterDLL("ssidummy.dll");
	Factory::RegisterDLL("ssisignal.dll");

	ex_dummy ();

	ssi_print ("\n\n\tpress a key to quit\n");
	getchar ();
	
	Factory::Clear ();

#ifdef USE_SSI_LEAK_DETECTOR
	}
	_CrtDumpMemoryLeaks();
#endif
	
	return 0;
}

void ex_dummy () {

	ITheFramework *frame = Factory::GetFramework ();

	Decorator *decorator = ssi_create(Decorator, 0, true);
	decorator->getOptions()->setOrigin(640, 0);
	frame->AddDecorator(decorator);

	/*
	 * Sensor
	 */
	// connect to Xsens
	Mouse *mickey = ssi_create(Mouse, 0, true);
	ITransformable *cursor_t = frame->AddProvider(mickey, SSI_MOUSE_CURSOR_PROVIDER_NAME);
	frame->AddSensor(mickey);

	/*
	 * Transformer
	 */
	//create object
	DummyT *myTransformer = ssi_create (DummyT, 0, true);	
	//set options
	myTransformer->getOptions()->flag = true;
	//add to framework
	ITransformable *dummy_t = frame->AddTransformer(cursor_t, myTransformer, "1");

	/*
	* Consumer
	*/
	//create object
	DummyC *myConsumer = ssi_create(DummyC, 0, true);
	//set options
	myConsumer->getOptions()->flag = true;
	//add to framework
	frame->AddConsumer(cursor_t, myConsumer, "1");

	/*
	 * Output
	 */
	SignalPainter *plot = ssi_create_id(SignalPainter, 0, "plot");
	plot->getOptions ()->setTitle ("Raw Mouse Signal");
	plot->getOptions ()->type = PaintSignalType::SIGNAL;
	plot->getOptions ()->size = 10;
	frame->AddConsumer(cursor_t, plot, "1");

	plot = ssi_create_id(SignalPainter, 0, "plot");
	plot->getOptions()->setTitle("Processed Mouse Signal");
	plot->getOptions()->type = PaintSignalType::SIGNAL;
	plot->getOptions()->size = 10;
	frame->AddConsumer(dummy_t, plot, "1");

	FileWriter *write = ssi_create(FileWriter, 0, true);
	write->getOptions()->setPath("test");
	frame->AddConsumer(dummy_t, write, "1");

	decorator->add("plot*", 2, 1, 0, 0, 800, 200);
	
	/*
	 * Start pipeline
	 */
	frame->Start ();

	ssi_print ("\n\n\tpress enter to continue\n");
	getchar ();

	frame->Stop ();		
	frame->Clear ();
}

void challenge(const char* data_dir, const char* model) {

	Trainer::SetLogLevel(SSI_LOG_LEVEL_DEBUG);

	const float FRAME_SIZE = 2;
	const float DELTA_SIZE = 2;

	// 2x5 matrix
	const int cols = 2;
	const int rows = 5;

	const char* prefix[] = { "Novice", "Expert" };
	const char* suffix[] = { "_au.stream", "_face.stream", "_head.stream", "_skel.stream", "_skeletonconf.stream" };
	const char* suffix_anno = "_vad.csv";

	char file[128];
	Annotation* annos = new Annotation[cols];
	Annotation* annos_c = new Annotation[cols];

	ssi_stream_t** streams = new ssi_stream_t*[cols];
	for (int i = 0; i < cols; i++)
		streams[i] = new ssi_stream_t[rows];

	SampleList** sl = new SampleList*[cols];
	for (int i = 0; i < cols; i++)
		sl[i] = new SampleList[rows];

	SampleList** slt = new SampleList*[cols];
	for (int i = 0; i < cols; i++)
		slt[i] = new SampleList[rows];

	SampleList sltm;

	//load streams and anno
	for (int i = 0; i < cols; i++)
	{
		streams[i] = new ssi_stream_t[rows];

		for (int j = 0; j < rows; j++)
		{
			sprintf(file, "%s\\%s%s", data_dir, prefix[i], suffix[j]);
			FileTools::ReadStreamFile(file, streams[i][j]);
		}

		sprintf(file, "%s\\%s%s", data_dir, prefix[i], suffix_anno);
		annos[i].load(file);
	}

	//create sample lists
	for (int i = 0; i < cols; i++)
	{
		annos[i].convertToFrames(FRAME_SIZE, 0, 0, 0.5);
		std::string name = "user" + i;
		for (int j = 0; j < rows; j++)
			annos[i].extractSamples(streams[i][j], &sl[i][j], name.c_str());
	}


	//transform sample lists
	Functionals* funct = ssi_create(Functionals, 0, true);
	for (int i = 0; i < cols; i++)
		for (int j = 0; j < rows; j++)
			ModelTools::TransformSampleList(sl[i][j], slt[i][j], *funct);

	/*
	* ......
	* compute more features
	* ......
	*/

	//merge sample lists
	SampleList* select[] = {
		&slt[0][0],
		&slt[0][1],
	};
	ModelTools::MergeSampleList(sltm, (sizeof(select) / sizeof(*select)), select);

	//load model
	Trainer train;
	Trainer::Load(train, model);

	Evaluation eval;
	eval.eval(&train, sltm);
	eval.print();
	getchar();
}