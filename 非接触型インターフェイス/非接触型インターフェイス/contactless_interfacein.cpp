#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <vector>

#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>

#pragma comment(lib, "opencv_world300.lib")

int main(){
	
	POINT Point;//構造体
	cv::Mat temp;//テンプレート画像用
	int dis_x, dis_y;//ディスプレイサイズ用

	//ディスプレイサイズを取得
	dis_x = GetSystemMetrics(SM_CXSCREEN);
	dis_y = GetSystemMetrics(SM_CYSCREEN);
	//マウスの位置を取得
	GetCursorPos(&Point);
	printf("%d %d\n", Point.x, Point.y);

	//テンプレート画像の読み込み
	temp = cv::imread("temp2.jpg");
	//Webカメラから読み込み
	cv::VideoCapture capture(0);
	//エラー処理
	if (!capture.isOpened()) {
		printf("オープンエラー\n");
		return -1;
	}
	//ウィンドウ作成
	cv::namedWindow("非接触型インターフェイス");

	//マウスポインタの位置を中心に
	SetCursorPos(dis_x / 2,dis_y / 2);
	printf("キー入力を待機中\n");
	char start = cv::waitKey(0);

	if (start == 's'){
		printf("実行開始\n");
	}

	while (1){
		cv::Mat frame;//入力画像用
		cv::Mat gauss;//平滑化
		cv::Mat match;//テンプレートマッチ用
		cv::Point pt;//座標記録用

		//キャプチャ画像をframeに
		capture >> frame;

		//平滑化
		cv::GaussianBlur(frame, gauss, cv::Size(5, 5), 0);
		//テンプレートマッチ開始、結果をmatchに
		cv::matchTemplate(gauss, temp, match, CV_TM_CCOEFF_NORMED);

		//最もマッチした領域の座標を取得
		cv::minMaxLoc(match, 0, 0, 0, &pt);

		//最もマッチした領域を四角形で囲む
		//(画像,開始頂点(X,Y),終了頂点(X,Y),色,太さ,枠線の種類,shift)
		cv::rectangle(frame, pt, cv::Point(pt.x + temp.cols, pt.y + temp.cols), cv::Scalar(0, 0, 255), 2, 8, 0);

		//画像を描写
		cv::imshow("非接触型インターフェイス", frame);
		//マウスの位置を変更
		//X軸は座標を反転する
		SetCursorPos(dis_x - pt.x * 3, pt.y * 2);

		//キー入力を待つ
		char key = cv::waitKey(1);
		if (key == 27){
			printf("終了\n");
			break;
		}
		else if (key == 32){
			SetCursorPos(dis_x / 2, dis_y / 2);
		}
	}

	return 0;
}