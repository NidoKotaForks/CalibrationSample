
// for cURL
#include "curl/curl.h"

// for OpenCV2
#include <opencv2/opencv.hpp>

// receive buffer for cURL
#define BUF_SIZE	(512 * 1024)
char recv_buf[BUF_SIZE + 1];
int  recv_buf_size = 0;

// data receive callback function
size_t recv_data(void* buffer, size_t size, size_t nmemb, void* userdata)
{
	printf("recv_data\n");

	size_t segsize = size * nmemb;

	if (recv_buf_size + segsize > BUF_SIZE) {
		*(int*)userdata = 1;
		return 0;
	}

	memcpy((recv_buf + recv_buf_size), buffer, segsize);
	recv_buf_size += segsize;
	recv_buf[recv_buf_size] = 0;

	return segsize;
}

cv::Mat get_jpeg_image(const char* url)
{
	CURL* curl = NULL;
	CURLcode res;
	int      err = 0;

	curl = curl_easy_init();
	if (!curl) {
		printf("curl_easy_init() failed...\n");
		return cv::Mat();
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&err); // userdata
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, recv_data); //

	res = curl_easy_perform(curl);
	printf("res=%d (write_error = %d)\n", res, err);

	if (res == CURLE_OK) 
	{
		/* extract the content-type */
		char* ct = NULL;
		res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
		if (!res && ct) {
			printf("Content-Type: %s\n", ct);
		}
	}
	else 
	{
		printf("curl_easy_perform() failed...%s\n", curl_easy_strerror(res));
		return cv::Mat();
	}

	curl_easy_cleanup(curl);

	cv::Mat recv_data;
	recv_data.create(cv::Size(recv_buf_size, 1), CV_8UC1);
	memcpy(recv_data.data, recv_buf, recv_buf_size);

	return recv_data;
}

int mainmainmain(int argc, char* argv[])
{
	cv::Mat recv_data;

	// HTTPでJPEG画像を取得する
	recv_data = get_jpeg_image("https://i.pinimg.com/564x/c8/d2/c7/c8d2c76d9785431a7026d3be400c469a.jpg");
	if (recv_data.empty()) {
		printf("get_jpeg_image() failed...\n");
		return -1;
	}

	// JPEGデータをビットマップにデコードする
	//cv::Mat raw_jpeg_data(1, recv_buf_size, CV_8UC1, recv_buf);
	cv::Mat image = cv::imdecode(recv_data, -1);

	// 表示
	cv::imshow("jpeg image", image);
	cv::waitKey(0);

	return 0;
}