#include<iostream>
#include<fstream>
#include"feat/feature-mfcc.h"
#include"feat/wave-reader.h"
#include"base/kaldi-math.h"
#include"matrix/kaldi-matrix-inl.h"
#include"matrix/kaldi-vector.h"

int main(int argc, char * argv[]) {
	try {
		using namespace kaldi;   //Ҫ��סʹ��namespace kaldi
		const char * Usage =
			"my_kaldi.exe [wav_filename] [mfcc_filename] \n";

		ParseOptions po(Usage);
		po.Read(argc, argv);

		if (po.NumArgs() != 2) {
			std::cerr << "argc:" << argc<<std::endl;
			for (int i = 0; i < argc; i++) {
				std::cerr << "argv[" << i<<"]:"<< argv[i]<<std::endl;
			}
			po.PrintUsage();
			exit(1);
		}
		std::string wav_filename = po.GetArg(1);
		std::string mfcc_filename = po.GetArg(2);

		/*��ȡwav�ļ�*/
		std::ifstream wav_file;
		wav_file.open(wav_filename, std::ios_base::binary);
		WaveData wave;
		wave.Read(wav_file);
		SubVector<BaseFloat> waveform(wave.Data(), 0);//��wav�ļ����ݷŵ�waveform��

		/*mfcc��������*/
		MfccOptions mfcc_opts;
		mfcc_opts.frame_opts.samp_freq = 16000;
		mfcc_opts.frame_opts.frame_length_ms = 25;
		mfcc_opts.frame_opts.frame_shift_ms = 10;
		mfcc_opts.frame_opts.preemph_coeff = 0.95;

		/*��ȡmfcc����*/
		Matrix<BaseFloat> mfcc_feature;
		Mfcc feat(mfcc_opts);
		feat.ComputeFeatures(waveform, wave.SampFreq(), 1.0, &mfcc_feature);

		/*д�뵽�ļ���*/
		WriteKaldiObject(mfcc_feature, mfcc_filename, false);
		//kaldi�ж�����ʹ��WriteKaldiObject��дʹ�õ���kaldi����
	}

	catch (const std::exception &e) {
		std::cerr << e.what();
		return -1;
	}
	return 0;
}
