#include <stdio.h>
#include <vector>

#include <opencv2/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc.hpp>

#pragma comment(lib, "opencv_world300.lib")

//�������邽�߂ɕK�v�Ȋ֐�
void PinP_point(const cv::Mat &srcImg, const cv::Mat &smallImg, const cv::Point2f p0, const cv::Point2f p1)
{
	// �w�i�摜�̍쐬
	cv::Mat dstImg;
	srcImg.copyTo(dstImg);

	// �R�g�̑Ή��_���쐬
	std::vector<cv::Point2f> src, dst;
	src.push_back(cv::Point2f(0, 0));
	src.push_back(cv::Point2f(smallImg.cols, 0));
	src.push_back(cv::Point2f(smallImg.cols, smallImg.rows));

	dst.push_back(p0);
	dst.push_back(cv::Point2f(p1.x, p0.y));
	dst.push_back(p1);

	// �O�i�摜�̕ό`�s��
	cv::Mat mat = cv::getAffineTransform(src, dst);

	// �A�t�B���ϊ��̎��s
	cv::warpAffine(smallImg, dstImg, mat, dstImg.size(), CV_INTER_LINEAR, cv::BORDER_TRANSPARENT);
	imshow("Capture", dstImg);

}


int main()
{
	// �J�X�P�[�h�f�[�^�̓ǂ݂���
	cv::CascadeClassifier cascade("haarcascade_frontalface_alt.xml");
	//cv::CascadeClassifier cascade("haarcascade_frontalcatface.xml");

	// �J�����f�B�o�C�X���g�p
	cv::VideoCapture  capture(0);
	//cv::VideoCapture  capture("filename");//�t�@�C��������͂��\

	// �J�����f�B�o�C�X�I�[�v���G���[�̍ۂ͏I��
	if (!capture.isOpened()) {
		printf("�I�[�v���G���[\n");
		return -1;
	}

	// cv::namedWindow�֐��ŃL���v�`���摜�\���p�E�B���h�E���쐬
	cv::namedWindow("Capture");
	int flag = 0;
	// �w�肳�ꂽ���������������܂ŁA�J�����摜�擾���\�����J��Ԃ�
	while (1) {
		// cv::Mat�N���X�̕ϐ�frame�́A�J�����摜���󂯎�邽�߂̂���
		cv::Mat frame;
		cv::Mat moza;
		cv::Mat grayImage;//�O���C�X�P�[��

		// �f�o�C�X����摜���擾����Bframe�ɑ��	
		capture >> frame;

		std::vector<cv::Rect> faces;
		cv::cvtColor(frame, grayImage, CV_RGB2GRAY);//�O���C�X�P�[����
		cascade.detectMultiScale(grayImage, faces, 1.1, 2, 2);//�J�X�P�[�h���ފ�Ŋ��F��
		
		// �`�揈�����s��
		for (auto it = faces.begin(); it != faces.end(); ++it){
			// ��ƔF������������rectangle�֐��ŋ�`��`��
			cv::rectangle(frame, it->tl(),it->br() , cv::Scalar(0, 255, 255), 2, 8, 0);
			// ��ƔF�������������g���~���O
			cv::Mat roi(frame, cv::Rect(it->tl(), it->br()));
			
			// ���U�C�N������������
			int n = 8; // 臒l

			// �g���~���O�摜���k��
			cv::resize(roi, moza, cv::Size(), (double)1 / n, (double)1 / n);
			// �g���~���O�摜���g��
			cv::resize(moza, moza, cv::Size(), n, n, cv::INTER_NEAREST);
			
			//���U�C�N���������������ƁA���ꂢ�����̕�������������
			PinP_point(frame, moza, it->tl(), it->br());
		}

		// �r�f�I�t�@�C���̏ꍇ�A�Đ����I���Ɖ摜���擾�ł��Ȃ��̂ŁA
		// �摜���󂩂ǂ������`�F�b�N����
		// Web�J�����̏ꍇ�͂قڕK�v�Ȃ�
		if (frame.empty()) {
			capture.set(CV_CAP_PROP_POS_FRAMES, 0);
			continue;
		}		
		
		// cv::waitKey�֐��ŁA�L�[���͂�1�~���b�����҂�
		char key = cv::waitKey(1);

		// ���͂��ꂽ�L�[��ESC�L�[�Ȃ�I��
		if (key == 27) {
			break;
		}
	}

	return 0;
}

