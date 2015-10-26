#include <ctime>
#include <sstream>
#include <iostream>
#include <algorithm>

#include <Windows.h>
#include <MMSystem.h>

#include "SOINN.h"

using namespace NN;

void main_old()
{
	/*int scale = 3;
	Mat input(scale * 100, scale * 100, CV_8UC1);
	input = Mat::zeros(input.size(), input.type());
	circle(input, Point(scale * 30, scale * 30), 5 * scale, Scalar(255), 3, CV_AA);
	circle(input, Point(scale * 30, scale * 30), 20 * scale, Scalar(255), 3, CV_AA);
	rectangle(input, Point(scale * 50, scale * 50), Point(scale * 70, scale * 80), Scalar(255), 5, CV_AA);*/

	SOINN soinn(3000, 30);
	for (int learn_num = 0; learn_num < 10; ++learn_num)
	{
		// ノイズ生成
		/*Mat noize(input.size(), input.type());
		noize = Mat::zeros(input.size(), input.type());
		randn(noize, Scalar::all(255), Scalar::all(5E11));*/

		stringstream ss;
		ss << "noize" << learn_num << ".png";
		//imwrite(ss.str(), noize);

		// ノイズと合成した入力画像
		/*Mat noized_input(input.size(), input.type());
		noized_input = Mat::zeros(input.size(), input.type());
		bitwise_xor(input, noize, noized_input);*/

		ss.str("");
		ss << "input" << learn_num << ".png";
		//imwrite(ss.str(), noized_input);

		// 入力ノード
		vector<Node> input_nodes;
		/*for (int i = 0; i < (int)input.rows; ++i)
		{
			for (int j = 0; j < input.cols; ++j)
			{
				if (noized_input.at<uchar>(i, j) == 255)
				{
					Node new_node;
					new_node.position = VectorXd(2);
					new_node.position << i, j;
					input_nodes.push_back(new_node);
				}
			}
		}*/
		random_shuffle(input_nodes.begin(), input_nodes.end());

		// いざ学習
		cout << "all inputs:" << input_nodes.size() << endl;

		clock_t start, end;
		start = clock();
		soinn.learn(input_nodes);
		end = clock();

		cout << "learning finished! time=" << (double)(end - start) / CLOCKS_PER_SEC << endl;

		// 結果画像保存
		/*Mat result(input.size(), input.type());
		result = Mat::zeros(result.size(), result.type());

		vector<Node> learned_result = soinn.getLearnedNode();
		for (int i = 0; i < (int)learned_result.size(); ++i)
		{
			Node r = learned_result.at(i);
			if (!isnan(r.position[0]) && !isnan(r.position[1])
				&& !isinf(r.position[0]) && !isinf(r.position[1])
				&& result.rows >(int)r.position[0]
				&& result.cols > (int)r.position[1])
				result.at<uchar>((int)r.position[0], (int)r.position[1]) = 255;
		}

		ss.str("");
		ss << "result" << learn_num << ".png";
		imwrite(ss.str(), result);*/
	}
}

void main()
{
	HWAVEIN microHandle;
	WAVEHDR waveHeader;

	const int NUMPTS = 22050 * 10;   // 10 seconds
	int sampleRate = 22050;
	short int waveIn[NUMPTS];   // 'short int' is a 16-bit type; I request 16-bit samples below
								// for 8-bit capture, you'd use 'unsigned char' or 'BYTE' 8-bit types

	MMRESULT result = 0;

	WAVEFORMATEX format;
	format.wFormatTag = WAVE_FORMAT_PCM;      // simple, uncompressed format
	format.wBitsPerSample = 8;                //  16 for high quality, 8 for telephone-grade
	format.nChannels = 1;                     //  1=mono, 2=stereo
	format.nSamplesPerSec = sampleRate;       //  22050
	format.nAvgBytesPerSec = format.nSamplesPerSec*format.nChannels*format.wBitsPerSample / 8;
	// = nSamplesPerSec * n.Channels * wBitsPerSample/8
	format.nBlockAlign = format.nChannels*format.wBitsPerSample / 8;
	// = n.Channels * wBitsPerSample/8
	format.cbSize = 0;

	result = waveInOpen(&microHandle, WAVE_MAPPER, &format, 0L, 0L, WAVE_FORMAT_DIRECT);

	if (result)
	{
		cout << "Fail step 1" << endl;
		cout << result << endl;
		Sleep(10000);
		return;
	}

	// Set up and prepare header for input
	waveHeader.lpData = (LPSTR)waveIn;
	waveHeader.dwBufferLength = NUMPTS * 2;
	waveHeader.dwBytesRecorded = 0;
	waveHeader.dwUser = 0L;
	waveHeader.dwFlags = 0L;
	waveHeader.dwLoops = 0L;
	waveInPrepareHeader(microHandle, &waveHeader, sizeof(WAVEHDR));

	// Insert a wave input buffer
	result = waveInAddBuffer(microHandle, &waveHeader, sizeof(WAVEHDR));

	if (result)
	{
		cout << "Fail step 2" << endl;
		cout << result << endl;
		Sleep(10000);
		return;
	}

	result = waveInStart(microHandle);

	if (result)
	{
		cout << "Fail step 3" << endl;
		cout << result << endl;
		Sleep(10000);
		return;
	}

	// Wait until finished recording
	do {} while (waveInUnprepareHeader(microHandle, &waveHeader, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);

	waveInClose(microHandle);
}