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
	
	POINT Point;//�\����
	cv::Mat temp;//�e���v���[�g�摜�p
	int dis_x, dis_y;//�f�B�X�v���C�T�C�Y�p

	//�f�B�X�v���C�T�C�Y���擾
	dis_x = GetSystemMetrics(SM_CXSCREEN);
	dis_y = GetSystemMetrics(SM_CYSCREEN);
	//�}�E�X�̈ʒu���擾
	GetCursorPos(&Point);
	printf("%d %d\n", Point.x, Point.y);

	//�e���v���[�g�摜�̓ǂݍ���
	temp = cv::imread("temp2.jpg");
	//Web�J��������ǂݍ���
	cv::VideoCapture capture(0);
	//�G���[����
	if (!capture.isOpened()) {
		printf("�I�[�v���G���[\n");
		return -1;
	}
	//�E�B���h�E�쐬
	cv::namedWindow("��ڐG�^�C���^�[�t�F�C�X");

	//�}�E�X�|�C���^�̈ʒu�𒆐S��
	SetCursorPos(dis_x / 2,dis_y / 2);
	printf("�L�[���͂�ҋ@��\n");
	char start = cv::waitKey(0);

	if (start == 's'){
		printf("���s�J�n\n");
	}

	while (1){
		cv::Mat frame;//���͉摜�p
		cv::Mat gauss;//������
		cv::Mat match;//�e���v���[�g�}�b�`�p
		cv::Point pt;//���W�L�^�p

		//�L���v�`���摜��frame��
		capture >> frame;

		//������
		cv::GaussianBlur(frame, gauss, cv::Size(5, 5), 0);
		//�e���v���[�g�}�b�`�J�n�A���ʂ�match��
		cv::matchTemplate(gauss, temp, match, CV_TM_CCOEFF_NORMED);

		//�ł��}�b�`�����̈�̍��W���擾
		cv::minMaxLoc(match, 0, 0, 0, &pt);

		//�ł��}�b�`�����̈���l�p�`�ň͂�
		//(�摜,�J�n���_(X,Y),�I�����_(X,Y),�F,����,�g���̎��,shift)
		cv::rectangle(frame, pt, cv::Point(pt.x + temp.cols, pt.y + temp.cols), cv::Scalar(0, 0, 255), 2, 8, 0);

		//�摜��`��
		cv::imshow("��ڐG�^�C���^�[�t�F�C�X", frame);
		//�}�E�X�̈ʒu��ύX
		//X���͍��W�𔽓]����
		SetCursorPos(dis_x - pt.x * 3, pt.y * 2);

		//�L�[���͂�҂�
		char key = cv::waitKey(1);
		if (key == 27){
			printf("�I��\n");
			break;
		}
		else if (key == 32){
			SetCursorPos(dis_x / 2, dis_y / 2);
		}
	}

	return 0;
}