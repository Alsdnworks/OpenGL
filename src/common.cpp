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

glm::vec3 GetAttenuationCoeff(float distance) {
  const auto linear_coeff = glm::vec4(
    8.4523112e-05, 4.4712582e+00, -1.8516388e+00, 3.3955811e+01
  );
  const auto quad_coeff = glm::vec4(
    -7.6103583e-04, 9.0120201e+00, -1.1618500e+01, 1.0000464e+02
  );

  float kc = 1.0f;
  float d = 1.0f / distance;
  auto dvec = glm::vec4(1.0f, d, d*d, d*d*d);
  float kl = glm::dot(linear_coeff, dvec);
  float kq = glm::dot(quad_coeff, dvec);
  //왜??????9-4강??2816

  return glm::vec3(kc, glm::max(kl, 0.0f), glm::max(kq*kq, 0.0f));
}