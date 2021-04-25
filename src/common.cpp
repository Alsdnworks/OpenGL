#include "common.h"
#include <fstream>
#include <sstream>

std::optional<std::string> LoadTextFile(const std::string& filename) {
    //C++스타일의 파일로딩방식
    std::ifstream fin(filename);
    //is open이 부정이면 none값 리턴
    if (!fin.is_open()) {
        SPDLOG_ERROR("failed to open file: {}", filename);
        return {};
    }
    //스트링스트림에 내용을 넣는다
    std::stringstream text;
    text << fin.rdbuf();
    //스트링으로 변환해준다.
    return text.str();
}