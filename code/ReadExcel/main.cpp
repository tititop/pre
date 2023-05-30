#include <iostream>

#include "adapt_glog.h"
#include "File.h"

const std::string file_path = "C:\\Users\\22619\\Desktop\\QRNGSN _dev_excel.xlsx";



void Process() {
	char* file_content;
	size_t file_len = 0;
	file_len = MFile::Instance().get_file_content(&file_content, file_path);



	MFile::Instance().drop_file_content(&file_content, file_len);
}

int main() {
	Process();
	MXT_KEY_LOGGER << "hello read excel";
	return 0;
}
