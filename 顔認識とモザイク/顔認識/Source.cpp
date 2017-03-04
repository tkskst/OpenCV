#include <stdio.h>
#include <vector>

#include <opencv2/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc.hpp>

#pragma comment(lib, "opencv_world300.lib")

//合成するために必要な関数
void PinP_point(const cv::Mat &srcImg, const cv::Mat &smallImg, const cv::Point2f p0, const cv::Point2f p1)
{
	// 背景画像の作成
	cv::Mat dstImg;
	srcImg.copyTo(dstImg);

	// ３組の対応点を作成
	std::vector<cv::Point2f> src, dst;
	src.push_back(cv::Point2f(0, 0));
	src.push_back(cv::Point2f(smallImg.cols, 0));
	src.push_back(cv::Point2f(smallImg.cols, smallImg.rows));

	dst.push_back(p0);
	dst.push_back(cv::Point2f(p1.x, p0.y));
	dst.push_back(p1);

	// 前景画像の変形行列
	cv::Mat mat = cv::getAffineTransform(src, dst);

	// アフィン変換の実行
	cv::warpAffine(smallImg, dstImg, mat, dstImg.size(), CV_INTER_LINEAR, cv::BORDER_TRANSPARENT);
	imshow("Capture", dstImg);

}


int main()
{
	// カスケードデータの読みこみ
	cv::CascadeClassifier cascade("haarcascade_frontalface_alt.xml");
	//cv::CascadeClassifier cascade("haarcascade_frontalcatface.xml");

	// カメラディバイスを使用
	cv::VideoCapture  capture(0);
	//cv::VideoCapture  capture("filename");//ファイルから入力も可能

	// カメラディバイスオープンエラーの際は終了
	if (!capture.isOpened()) {
		printf("オープンエラー\n");
		return -1;
	}

	// cv::namedWindow関数でキャプチャ画像表示用ウィンドウを作成
	cv::namedWindow("Capture");
	int flag = 0;
	// 指定された条件が満たされるまで、カメラ画像取得→表示を繰り返す
	while (1) {
		// cv::Matクラスの変数frameは、カメラ画像を受け取るためのもの
		cv::Mat frame;
		cv::Mat moza;
		cv::Mat grayImage;//グレイスケール

		// デバイスから画像を取得する。frameに代入	
		capture >> frame;

		std::vector<cv::Rect> faces;
		cv::cvtColor(frame, grayImage, CV_RGB2GRAY);//グレイスケール化
		cascade.detectMultiScale(grayImage, faces, 1.1, 2, 2);//カスケード分類器で顔を認識
		
		// 描画処理を行う
		for (auto it = faces.begin(); it != faces.end(); ++it){
			// 顔と認識した部分をrectangle関数で矩形を描画
			cv::rectangle(frame, it->tl(),it->br() , cv::Scalar(0, 255, 255), 2, 8, 0);
			// 顔と認識した部分をトリミング
			cv::Mat roi(frame, cv::Rect(it->tl(), it->br()));
			
			// モザイク処理をかける
			int n = 8; // 閾値

			// トリミング画像を縮小
			cv::resize(roi, moza, cv::Size(), (double)1 / n, (double)1 / n);
			// トリミング画像を拡大
			cv::resize(moza, moza, cv::Size(), n, n, cv::INTER_NEAREST);
			
			//モザイク処理をした部分と、それいがいの部分を合成する
			PinP_point(frame, moza, it->tl(), it->br());
		}

		// ビデオファイルの場合、再生が終わると画像が取得できないので、
		// 画像が空かどうかをチェックする
		// Webカメラの場合はほぼ必要ない
		if (frame.empty()) {
			capture.set(CV_CAP_PROP_POS_FRAMES, 0);
			continue;
		}		
		
		// cv::waitKey関数で、キー入力を1ミリ秒だけ待つ
		char key = cv::waitKey(1);

		// 入力されたキーがESCキーなら終了
		if (key == 27) {
			break;
		}
	}

	return 0;
}

